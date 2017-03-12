#include "MapState.hpp"

namespace states {

MapState::MapState(engine::Engine& engine)
    : GameState(engine), renderer(engine, world, selection), pauseState(engine, world, renderer) {
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

  geometry.init(engine, world);
  createRandomWorld();

  if (!renderer.init()) {
    engine.stop();
    return;
  }

  selection.setColors(glm::vec4(1, 1, 0.f, 0.4f), glm::vec4(1, 1, 0.f, 0.4f), glm::vec4(1, 0, 0, 0.4f));

  world.getTimer().start();
};

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
    if (!selection.isSelecting()) {
      selection.from(selectionEnd);
    }
    selection.to(selectionEnd);
  }

  if (selection.isSelecting()) {
    glm::ivec2 size = selection.getTo() - selection.getFrom() + glm::ivec2(1, 1);
    data::buildings::Building toAdd;
    toAdd.width = size.x;
    toAdd.length = size.y;
    toAdd.level = newBuildingHeight;
    toAdd.x = selection.getFrom().x;
    toAdd.y = selection.getFrom().y;
    if (!geometry.checkCollisions(toAdd)) {
      selection.markValid();
    } else {
      selection.markInvalid();
    }
  }

  world.update(delta);
};

void MapState::render() {
  renderer.prepareFrame();

  renderer.renderWorld(renderGrid, true);
#ifdef DEBUG_CONFIG
  renderer.renderDebug(renderNormals);
#endif
  engine.getDebugInfo().onRenderWorldEnd();

  engine.getUI().startFrame();
  renderer.renderUI();
#ifdef DEBUG_CONFIG
  renderer.renderDebugUI();
#endif
  engine.getUI().endFrame();

  renderer.sendFrame();
};

void MapState::onKey(int key, int scancode, int action, int mods) {
  // TODO(kantoniak): Refactor MapState::onKey by the end of march. If it's April you can start laughing now :)
  scancode = 0;

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
    renderNormals = !renderNormals;
  }

  if (key == GLFW_KEY_G && action == GLFW_PRESS) {
    renderGrid = !renderGrid;
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
}

void MapState::onMouseButton(int button, int action, int mods) {
  mods = 0;
  if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
    selection.start(selection.getFrom());
  } else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
    selection.stop();

    glm::ivec2 size = selection.getTo() - selection.getFrom() + glm::ivec2(1, 1);
    data::buildings::Building toAdd;
    toAdd.width = size.x;
    toAdd.length = size.y;
    toAdd.level = newBuildingHeight;
    toAdd.x = selection.getFrom().x;
    toAdd.y = selection.getFrom().y;
    if (!geometry.checkCollisions(toAdd)) {
      world.getMap().addBuilding(toAdd);
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
}

void MapState::onScroll(double xoffset, double yoffset) {
  xoffset = 0;
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

rendering::Renderer& MapState::getRenderer() {
  return renderer;
}

void MapState::createRandomWorld() {

  city.name = "Warsaw";
  city.people = 57950;
  city.money = 445684;
  world.getMap().setCurrentCity(&city);

  const glm::ivec2 mapSize = glm::ivec2(2, 2);
  for (int x = 0; x < mapSize.x; x++) {
    for (int y = 0; y < mapSize.y; y++) {
      world.getMap().createChunk(glm::ivec2(x, y));
    }
  }

  const unsigned int buildingCount =
      (mapSize.x * mapSize.y) * (data::Chunk::SIDE_LENGTH * data::Chunk::SIDE_LENGTH / 4) * 0.05f;
  for (unsigned int i = 0; i < buildingCount; i++) {
    data::buildings::Building test;
    for (int i = 0; i < 20; i++) {
      test.width = rand() % 4 + 2;
      test.length = rand() % 4 + 2;
      test.level = rand() % 6 + 1;
      test.x = rand() % (data::Chunk::SIDE_LENGTH * mapSize.x - test.width + 1);
      test.y = rand() % (data::Chunk::SIDE_LENGTH * mapSize.y - test.length + 1);
      if (!geometry.checkCollisions(test)) {
        world.getMap().addBuilding(test);
        break;
      }
    }
  }

  world.getCamera().move(glm::vec3(data::Chunk::SIDE_LENGTH, 0, data::Chunk::SIDE_LENGTH));
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