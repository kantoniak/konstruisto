#include "MapState.hpp"

namespace states {

MapState::MapState(engine::Engine& engine)
    : GameState(engine), renderer(engine, world), saveFileHandler(engine), pauseState(engine, world, renderer) {
  suspended = false;
};

void MapState::init() {

  world::PerspectiveState initialPerspective;
  initialPerspective.fovy = 45.f;
  initialPerspective.aspect = engine.getWindowHandler().getViewportRatio();
  initialPerspective.zNear = 0.1f;
  initialPerspective.zFar = 10000.f;

  data::CameraState initialCamera;
  initialCamera.lookAt = glm::vec3();
  initialCamera.distance = 80;
  initialCamera.rotationAroundX = M_PI / 4.f;
  initialCamera.rotationAroundY = 0;

  world.getCamera().init(initialPerspective, initialCamera);
  world.init();

  city.name = "Warsaw";
  city.people = 57950;
  city.money = 445684;
  world.getMap().setCurrentCity(&city);

  createNewWorld();

  geometry.init(engine, world);
  init_brushes();

  tree_shape = std::make_shared<geometry::Circle>(0.20f);

  if (!renderer.init()) {
    engine.stop();
    return;
  }

  setCurrentAction(MapStateAction::PLACE_BUILDING);

  world.getTimer().start();
};

void MapState::init_brushes() noexcept {
  tree_brush =
      std::make_unique<input::Brush>(data::Position<float>(), TREE_BRUSH_RADIUS_NORMAL, TREE_BRUSH_BORDER_WIDTH);
  tree_brush->set_base_colors(glm::vec4(1, 1, 1, 0.05f), glm::vec4(1, 1, 1, 0.4f));
  tree_brush->set_active_colors(glm::vec4(1, 1, 0, 0.1f), glm::vec4(1, 1, 0, 0.4f));
}

void MapState::cleanup() {
  renderer.cleanup();
  world.cleanup();
}

void MapState::update(std::chrono::milliseconds delta) {
  if (rmbPressed) {
    handleMapDragging(delta);
  }
  if (rotatingClockwise) {
    handleRotatingAroundY(delta, true);
  }
  if (rotatingCounterClockwise) {
    handleRotatingAroundY(delta, false);
  }
  if (rotatingUpwards) {
    handleRotatingAroundX(delta, true);
  }
  if (rotatingDownwards) {
    handleRotatingAroundX(delta, false);
  }

  glm::ivec2 selectionEnd;
  if (geometry.hitField(engine.getWindowHandler().getMousePositionNormalized(), selectionEnd)) {
    if (!selection->isSelecting()) {
      selection->from(selectionEnd);
    }
    selection->to(selectionEnd);
  }

  if (selection->isSelecting() && MapStateAction::PLACE_BUILDING == currentAction) {

    glm::vec2 selection_size = selection->getTo() - selection->getFrom() + glm::ivec2(1, 1);
    auto to_add_shape = std::make_shared<geometry::AABB>(glm::vec2(), selection_size);
    auto colliders = collides_with(data::CollisionLayer::BUILDINGS);
    geometry::Collidable to_add(data::CollisionLayer::BUILDINGS, colliders, to_add_shape, selection->getFrom());
    if (!world.get_collision_space().if_collides(to_add)) {
      selection->markValid();
    } else {
      selection->markInvalid();
    }
  }

  if (MapStateAction::PLACE_ROAD == currentAction && selection->isSelecting()) {
    data::Position from(selection->getFrom());
    data::Position to(selection->getTo());

    // FIXME(kantoniak): What if chunk in the middle is not active in the scene? Is it allowed.
    bool chunk_exists = world.getMap().chunkExists(from.getChunk()) && world.getMap().chunkExists(to.getChunk());

    if (!chunk_exists) {
      selection->markInvalid();
    } else {
      geometry::Collidable::ptr candidate = selection_to_AABB(*selection, data::CollisionLayer::ROADS);
      if (!world.get_collision_space().if_collides(*candidate)) {
        selection->markValid();
      } else {
        selection->markInvalid();
      }
    }
  }

  if (currentAction == MapStateAction::PLACE_TREES) {
    glm::vec3 hitpoint;
    if (geometry.hitGround(engine.getWindowHandler().getMousePositionNormalized(), hitpoint)) {
      tree_brush->set_center(glm::vec2(hitpoint.x, hitpoint.z));

      if (tree_brush->get_radius() > TREE_BRUSH_RADIUS_SINGLE && tree_brush->is_active()) {
        if (glm::distance(saved_tree_brush_hitpoint, tree_brush->get_center().getGlobal()) > 2.f) {
          saved_tree_brush_hitpoint = tree_brush->get_center().getGlobal();
          insert_trees_from_brush();
        }
      }
    }
  }

  world.update(delta);
};

void MapState::render() {
  renderer.prepareFrame();

  renderer.renderWorld(*selection, current_brush);

#ifdef _DEBUG
  renderer.renderDebug();
#endif
  engine.getDebugInfo().onRenderWorldEnd();

  engine.getUI().startFrame();
  renderer.renderUI();
#ifdef _DEBUG
  renderer.renderDebugUI();
#endif
  engine.getUI().endFrame();

  renderer.sendFrame();
};

void MapState::onKey(int key, int, int action, int mods) {
  // TODO(kantoniak): Refactor MapState::onKey by the end of march. If it's April you can start laughing now :)

  if (key == GLFW_KEY_MINUS && action != GLFW_RELEASE) {
    world.getCamera().zoom(-5);
  }

  if (key == GLFW_KEY_EQUAL && action != GLFW_RELEASE) {
    world.getCamera().zoom(5);
  }

  // Rotation around Y
  if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
    rotatingClockwise = true;
  } else if (key == GLFW_KEY_Q && action == GLFW_RELEASE) {
    rotatingClockwise = false;
  }

  if (key == GLFW_KEY_E && action == GLFW_PRESS) {
    rotatingCounterClockwise = true;
  } else if (key == GLFW_KEY_E && action == GLFW_RELEASE) {
    rotatingCounterClockwise = false;
  }

  // Rotation around X
  if (key == GLFW_KEY_HOME && action == GLFW_PRESS) {
    rotatingUpwards = true;
  } else if (key == GLFW_KEY_HOME && action == GLFW_RELEASE) {
    rotatingUpwards = false;
  }

  if (key == GLFW_KEY_END && action == GLFW_PRESS) {
    rotatingDownwards = true;
  } else if (key == GLFW_KEY_END && action == GLFW_RELEASE) {
    rotatingDownwards = false;
  }

  if (key == GLFW_KEY_N && action == GLFW_PRESS) {
    engine.getSettings().rendering.renderNormals = !engine.getSettings().rendering.renderNormals;
    engine.getLogger().debug(std::string("Normals view: %s"),
                             (engine.getSettings().rendering.renderNormals ? "on" : "off"));
  }

  if (key == GLFW_KEY_M && action == GLFW_PRESS) {
    engine.getLogger().debug("Road nodes debug: %s",
                             (engine.getSettings().rendering.renderRoadNodesAsMarkers ? "on" : "off"));
    engine.getSettings().rendering.renderRoadNodesAsMarkers = !engine.getSettings().rendering.renderRoadNodesAsMarkers;
    renderer.markTileDataForUpdate();
  }

  if (key == GLFW_KEY_G && action == GLFW_PRESS) {
    engine.getSettings().world.showGrid = !engine.getSettings().world.showGrid;
  }

  if (mods == GLFW_MOD_CONTROL) {
    if (GLFW_KEY_1 <= key && key <= GLFW_KEY_9 && action == GLFW_PRESS) {
      newBuildingHeight = key - GLFW_KEY_1 + 1;
    }
  } else {
    if (GLFW_KEY_1 <= key && key <= GLFW_KEY_1 + world.getTimer().getMaxSpeed() - 1 && action == GLFW_PRESS) {
      world.getTimer().setSpeed(key - GLFW_KEY_1 + 1);
      world.getTimer().start();
    }
  }

  if (key == GLFW_KEY_GRAVE_ACCENT && action == GLFW_PRESS) {
    world.getTimer().pause();
  }

  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    switchToPauseState();
  }

  if (key == GLFW_KEY_Z && action == GLFW_PRESS) {
    setCurrentAction(MapStateAction::PLACE_BUILDING);
  }

  if (key == GLFW_KEY_X && action == GLFW_PRESS) {
    setCurrentAction(MapStateAction::PLACE_ZONE);
  }

  if (key == GLFW_KEY_C && action == GLFW_PRESS) {
    setCurrentAction(MapStateAction::PLACE_ROAD);
  }

  if (key == GLFW_KEY_V && action == GLFW_PRESS) {
    setCurrentAction(MapStateAction::PLACE_TREES);
  }

  if (key == GLFW_KEY_B && action == GLFW_PRESS) {
    setCurrentAction(MapStateAction::BULDOZE);
  }

  if (key == GLFW_KEY_N && action == GLFW_RELEASE && mods == GLFW_MOD_CONTROL) {
    this->createNewWorld();
  }

  if (key == GLFW_KEY_S && action == GLFW_RELEASE && mods == GLFW_MOD_CONTROL) {
    saveFileHandler.createSave(world);
  }

  if (key == GLFW_KEY_L && action == GLFW_RELEASE && mods == GLFW_MOD_CONTROL) {
    world.get_collision_space().clear();
    world.getMap().cleanup();
    saveFileHandler.loadSave(world);
    renderer.markTileDataForUpdate();
    renderer.markBuildingDataForUpdate();
  }

  if (currentAction == MapStateAction::PLACE_TREES) {
    switch (action) {
    case GLFW_PRESS: {
      if (key == GLFW_KEY_LEFT_ALT) {
        tree_brush->set_radius(TREE_BRUSH_RADIUS_SINGLE);
        renderer.mark_brush_dirty();
      }
      if (key == GLFW_KEY_LEFT_CONTROL) {
        tree_brush->set_radius(TREE_BRUSH_RADIUS_SMALL);
        renderer.mark_brush_dirty();
      }
      if (key == GLFW_KEY_LEFT_SHIFT) {
        tree_brush->set_radius(TREE_BRUSH_RADIUS_BIG);
        renderer.mark_brush_dirty();
      }
      break;
    }

    case GLFW_RELEASE: {
      if (key != GLFW_KEY_LEFT_ALT && key != GLFW_KEY_LEFT_SHIFT && key != GLFW_KEY_LEFT_CONTROL) {
        break;
      }
      if (tree_brush->get_radius() != TREE_BRUSH_RADIUS_NORMAL) {
        tree_brush->set_radius(TREE_BRUSH_RADIUS_NORMAL);
        renderer.mark_brush_dirty();
      }
      break;
    }
    }
  }
}

void MapState::onMouseButton(int button, int action, int) {

  if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
    selection->start(selection->getFrom());
  } else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
    selection->stop();

    if (MapStateAction::PLACE_BUILDING == currentAction) {
      glm::ivec2 size = selection->getTo() - selection->getFrom() + glm::ivec2(1, 1);
      data::buildings::Building toAdd;
      toAdd.width = size.x;
      toAdd.length = size.y;
      toAdd.level = newBuildingHeight;
      toAdd.x = selection->getFrom().x;
      toAdd.y = selection->getFrom().y;
      auto to_add_shape = std::make_shared<geometry::AABB>(glm::vec2(), glm::vec2(size.x, size.y));
      auto colliders = collides_with(data::CollisionLayer::BUILDINGS);
      toAdd.body = std::make_shared<geometry::Collidable>(data::CollisionLayer::BUILDINGS, colliders, to_add_shape,
                                                          selection->getFrom());
      if (!world.get_collision_space().if_collides(*toAdd.body)) {
        world.get_collision_space().insert(toAdd.body);
        world.getMap().addBuilding(toAdd);
        renderer.markBuildingDataForUpdate();
      }
    }

    if (MapStateAction::PLACE_ROAD == currentAction && selection->isValid()) {
      geometry::Collidable::ptr candidate = selection_to_AABB(*selection, data::CollisionLayer::ROADS);
      if (!world.get_collision_space().if_collides(*candidate)) {
        auto* s = static_cast<input::LineSelection*>(selection.get());
        const std::vector<input::LineSelection> selections = s->divideByChunk();
        std::vector<data::Road> roads;
        for (auto& selection : selections) {
          geometry::Collidable::ptr road_body = selection_to_AABB(selection, data::CollisionLayer::ROADS);
          world.get_collision_space().insert(road_body);
          roads.emplace_back(selection.getSelected(), road_body);
        }
        world.getMap().addRoads(roads);
        renderer.markTileDataForUpdate();
      }
    }

    if (MapStateAction::BULDOZE == currentAction) {
      for (data::buildings::Building b : geometry.getBuildings(selection->getFrom(), selection->getTo())) {
        world.getMap().removeBuilding(b);
      }
      renderer.markBuildingDataForUpdate();
    }
  }
  if (button == GLFW_MOUSE_BUTTON_RIGHT) {
    if (action == GLFW_PRESS) {
      rmbPressed = true;
      dragStart = engine.getWindowHandler().getMousePosition();
    } else {
      rmbPressed = false;
    }
  }

  if (current_brush && button == GLFW_MOUSE_BUTTON_LEFT) {
    if (action == GLFW_PRESS) {
      current_brush->set_active(true);

      glm::vec3 hitpoint;
      if (geometry.hitGround(engine.getWindowHandler().getMousePositionNormalized(), hitpoint)) {
        saved_tree_brush_hitpoint = glm::vec2(hitpoint.x, hitpoint.z);

        if (current_brush->get_radius() == TREE_BRUSH_RADIUS_SINGLE) {
          insert_trees_around(saved_tree_brush_hitpoint, {glm::vec2()});
        } else {
          insert_trees_from_brush();
        }
      }

    } else if (action == GLFW_RELEASE) {
      current_brush->set_active(false);
    }
    renderer.mark_brush_dirty();
  }
}

void MapState::onScroll(double, double yoffset) {
  world.getCamera().zoom(yoffset * 5);
}

void MapState::onWindowFocusChange(int focused) {
  if (!focused) {
    switchToPauseState();
  }
}

void MapState::onWindowResize(int width, int height) {
  world.getCamera().updateAspect(width / (float)height);
}

geometry::Collidable::ptr MapState::selection_to_AABB(const input::Selection& selection,
                                                      data::CollisionLayer layer) const noexcept {
  glm::vec2 selection_size = selection.getTo() - selection.getFrom() + glm::ivec2(1, 1);
  auto to_add_shape = std::make_shared<geometry::AABB>(glm::vec2(), selection_size);
  geometry::Collidable::layer_key colliders = collides_with(static_cast<data::CollisionLayer>(layer));
  return std::make_shared<geometry::Collidable>(layer, colliders, to_add_shape, selection.getFrom());
}

data::Tree MapState::create_random_tree(const data::Position<float>& position,
                                        geometry::Collidable::ptr tree_body) noexcept {
  const auto type = static_cast<data::Tree::Type>(rand() % data::Tree::TREE_TYPE_COUNT);
  const float age = 100.f * static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
  const float rotation = 2.f * M_PI * static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
  return data::Tree(type, position, rotation, age, tree_body);
}

void MapState::insert_trees_from_brush() noexcept {
  static const float tree_density = 2.5f;
  const float brush_radius = current_brush->get_radius();
  size_t point_count = tree_density * brush_radius * brush_radius;
  std::vector<glm::vec2> points = geometry.distribute_in_circle(point_count, brush_radius, 3.f);
  insert_trees_around(saved_tree_brush_hitpoint, points);
}

void MapState::insert_trees_around(const glm::vec2& center, const std::vector<glm::vec2>& points) noexcept {
  for (auto& point : points) {
    const glm::vec2 tree_center(center + point);
    auto colliders = collides_with(data::CollisionLayer::TREES);
    auto collidable_ptr =
        std::make_shared<geometry::Collidable>(data::CollisionLayer::TREES, colliders, tree_shape, tree_center);
    if (!world.get_collision_space().if_collides(*collidable_ptr)) {
      world.get_collision_space().insert(collidable_ptr);
      world.getMap().add_tree(create_random_tree(tree_center, collidable_ptr));
    }
  }
}

void MapState::createNewWorld() {

  world.getMap().cleanup();

  const glm::ivec2 mapSize = glm::ivec2(1, 1);
  for (int x = 0; x < mapSize.x; x++) {
    for (int y = 0; y < mapSize.y; y++) {
      world.getMap().createChunk(glm::ivec2(x, y));
    }
  }
}

void MapState::createRandomWorld() {

  /*const glm::ivec2 mapSize = glm::ivec2(2, 2);
  for (int x = 0; x < mapSize.x; x++) {
    for (int y = 0; y < mapSize.y; y++) {
      world.getMap().createChunk(glm::ivec2(x, y));
    }
  }

  for (int x = 0; x < mapSize.x; x++) {
    for (int y = 0; y < mapSize.y; y++) {
      auto chunkPos = glm::ivec2(x, y);
      int lotX = 0;
      for (int i = 2; i < 11; i++) {
        data::Lot lot;
        lot.position.setLocal(glm::ivec2(lotX + 2, 2), chunkPos);
        lot.size = glm::ivec2(i, 6);
        switch (i % 4) {
        case 0:
          lot.direction = data::Direction::N;
          break;
        case 1:
          lot.direction = data::Direction::S;
          break;
        case 2:
          lot.direction = data::Direction::W;
          break;
        case 3:
          lot.direction = data::Direction::E;
          break;
        }
        world.getMap().addLot(lot);
        lotX += 1 + i;
      }
    }
  }

  // TEST road division
  renderer.markTileDataForUpdate();

  constexpr unsigned int minBuildingSide = 2;
  constexpr unsigned int maxBuildingSideDifference = 3;
  constexpr unsigned int minBuildingHeight = 1;
  constexpr unsigned int maxBuildingHeightDifference = 6;
  constexpr unsigned int maxCollisionTries = 20;
  const unsigned int buildingCount =
      (mapSize.x * mapSize.y) * (data::Chunk::SIDE_LENGTH * data::Chunk::SIDE_LENGTH / 4) * 0.02f;
  for (unsigned int i = 0; i < buildingCount; i++) {
    data::buildings::Building test;
    for (unsigned int i = 0; i < maxCollisionTries; i++) {
      test.width = rand() % maxBuildingSideDifference + minBuildingSide;
      test.length = rand() % maxBuildingSideDifference + minBuildingSide;
      test.level = rand() % maxBuildingHeightDifference + minBuildingHeight;
      test.x = rand() % (data::Chunk::SIDE_LENGTH * mapSize.x - test.width + 1);
      test.y = rand() % (data::Chunk::SIDE_LENGTH * mapSize.y - test.length + 1);
      if (!geometry.checkCollisions(test)) {
        world.getMap().addBuilding(test);
        break;
      }
    }
  }*/
}

void MapState::setCurrentAction(MapStateAction action) {
  currentAction = action;
  renderer.setLeftMenuActiveIcon(currentAction - MapStateAction::PLACE_BUILDING);

  // Reset brush and selection
  engine.getSettings().rendering.renderSelection = false;
  current_brush = nullptr;
  renderer.mark_brush_dirty();

  switch (action) {
  case MapStateAction::PLACE_BUILDING:
    engine.getSettings().rendering.renderSelection = true;
    selection = std::make_unique<input::Selection>();
    selection->setColors(glm::vec4(1, 1, 0.f, 0.4f), glm::vec4(1, 1, 0.f, 0.4f), glm::vec4(1, 0, 0, 0.4f));
    break;
  case MapStateAction::PLACE_ZONE:
    break;
  case MapStateAction::PLACE_ROAD:
    engine.getSettings().rendering.renderSelection = true;
    selection = std::make_unique<input::LineSelection>(1);
    selection->setColors(glm::vec4(1, 1, 0.f, 0.4f), glm::vec4(1, 1, 0.f, 0.4f), glm::vec4(1, 0, 0, 0.4f));
    break;
  case MapStateAction::PLACE_TREES:
    current_brush = tree_brush;
    tree_brush->set_center(glm::vec2(10, 10));
    renderer.mark_brush_dirty();
    break;
  case MapStateAction::BULDOZE:
    engine.getSettings().rendering.renderSelection = true;
    selection = std::make_unique<input::Selection>();
    selection->setColors(glm::vec4(1, 0.f, 0.f, 0.2f), glm::vec4(1, 0.f, 0.f, 0.6f), glm::vec4(1, 0, 0, 1.f));
    break;
  }
}

void MapState::handleMapDragging(std::chrono::milliseconds delta) {
  const glm::vec2 mousePosition = engine.getWindowHandler().getMousePosition();
  const glm::vec2 dragDelta = mousePosition - dragStart;

  float dragSpeed = 0.2f * delta.count() / 1000.f;
  world.getCamera().move(dragSpeed * glm::vec3(dragDelta.x, 0.f, dragDelta.y));
}

void MapState::handleRotatingAroundY(std::chrono::milliseconds delta, bool clockwise) {
  float rotationDelta = M_PI * delta.count() / 1000.f;
  float modifier = (clockwise ? 1.f : -1.f);
  world.getCamera().rotateAroundY(rotationDelta * modifier);
}

void MapState::handleRotatingAroundX(std::chrono::milliseconds delta, bool upwards) {
  float rotationDelta = M_PI * delta.count() / 1000.f;
  float modifier = (upwards ? 1.f : -1.f);
  world.getCamera().rotateAroundX(rotationDelta * modifier);
}

void MapState::switchToPauseState() {
  engine.pushState(pauseState);
}
}