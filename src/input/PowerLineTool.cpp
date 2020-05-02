#include "PowerLineTool.hpp"

namespace input {

const float PowerLineTool::POLE_COLLISION_RADIUS = 0.6f;
const float PowerLineTool::POLE_SNAPPING_RADIUS = POLE_COLLISION_RADIUS * 0.5f;
const geometry::Circle::ptr PowerLineTool::pole_shape = std::make_shared<geometry::Circle>(POLE_COLLISION_RADIUS);
const geometry::Circle::ptr PowerLineTool::snapping_shape = std::make_shared<geometry::Circle>(POLE_SNAPPING_RADIUS);
const geometry::Collidable::layer_key PowerLineTool::pole_colliders = collides_with(data::CollisionLayer::POWER_LINES);

const glm::vec4 PowerLineTool::BRUSH_COLOR_VALID_BASE = glm::vec4(1, 1, 0, 0.05f);
const glm::vec4 PowerLineTool::BRUSH_COLOR_VALID_BORDER = glm::vec4(1, 1, 0, 0.4f);
const glm::vec4 PowerLineTool::BRUSH_COLOR_INVALID_BASE = glm::vec4(1, 0, 0, 0.05f);
const glm::vec4 PowerLineTool::BRUSH_COLOR_INVALID_BORDER = glm::vec4(1, 0, 0, 0.4f);

PowerLineTool::PowerLineTool(input::WindowHandler& window_handler, world::World& world,
                             const rendering::ModelManager& model_manager, geometry::Geometry& geometry,
                             std::shared_ptr<Brush>& current_brush) noexcept
    : state(START), pole(glm::vec2(0, 0)), window_handler(window_handler), world(world), geometry(geometry),
      state_brush_ptr(current_brush) {
  this->pole_model = std::make_shared<rendering::Model>(model_manager.get_model("power-line-pole"));
  this->pole_model_invalid = std::make_shared<rendering::Model>(model_manager.get_model("power-line-pole-invalid"));
  this->cable_model = std::make_shared<rendering::Model>(model_manager.get_model("power-line-cable"));
  this->cable_model_invalid = std::make_shared<rendering::Model>(model_manager.get_model("power-line-cable-invalid"));
  this->brush_ptr = std::make_shared<Brush>(glm::vec2(0, 0), pole_shape->get_radius(), 0.05f);
  this->mark_valid(true);
}

void PowerLineTool::update() noexcept {
  glm::vec3 hitpoint;

  if (!geometry.hitGround(window_handler.getMousePositionNormalized(), hitpoint)) {
    this->mark_valid(false);
    this->visible = false;
    return;
  }

  // Snapping
  const glm::vec2 new_translation(hitpoint.x, hitpoint.z);
  data::PowerLinePole::ptr snap_to = snap_point(new_translation);

  bool snapping_to_starting_pole = false;
  if (this->state == EXTEND) {
    snapping_to_starting_pole = (snap_to != nullptr && *snap_to == *(this->starting_pole));
  }

  if (snap_to != nullptr && !snapping_to_starting_pole) {
    if (this->snap_to == nullptr || *snap_to != *this->snap_to) {
      this->start_snapping(snap_to);
    }
    return;
  } else if (this->snapping) {
    this->stop_snapping();
  }

  const data::Position<float> position(new_translation);
  if (!world.getMap().chunkExists(position.getChunk())) {
    this->mark_valid(false);
    this->visible = false;
    return;
  }

  auto collidable_ptr = std::make_shared<geometry::Collidable>(data::CollisionLayer::POWER_LINES, pole_colliders,
                                                               pole_shape, new_translation);

  if (world.get_collision_space().if_collides(*collidable_ptr)) {
    this->mark_valid(false);
  } else {
    this->mark_valid(true);
  }

  // Rotate new pole towards the previous one
  if (state == EXTEND) {
    float end_pole_rotation = compute_rotation_towards(pole.get_position(), this->starting_pole->get_position());
    pole.set_rotation(end_pole_rotation);
  }

  this->pole.set_translation(new_translation);
  if (this->extension_cables) {
    this->extension_cables->update_transform();
  }
  this->brush_ptr->set_center(position);
  this->visible = true;
}

void PowerLineTool::pre_render(rendering::WorldRenderer& renderer) noexcept {
  if (!visible && state_brush_ptr) {
    state_brush_ptr.reset();
    brush_dirty = true;
  } else if (visible && !state_brush_ptr) {
    state_brush_ptr = brush_ptr;
    brush_dirty = true;
  }

  if (brush_dirty) {
    renderer.mark_brush_dirty();
    brush_dirty = false;
  }
}

void PowerLineTool::render(rendering::SceneRenderer& renderer) noexcept {
  if (!visible) {
    return;
  }

  // TODO(kantoniak): Support for color blending with materials
  if (valid) {
    if (!this->snapping) {
      rendering::Object pole_object(*(this->pole_model), pole.get_transform());
      renderer.draw_single(pole_object);
    }

    if (this->extension_cables) {
      renderer.draw_instanced(*(this->cable_model), extension_cables->get_transforms());
    }

  } else {
    if (!this->snapping) {
      rendering::Object pole_object(*(this->pole_model_invalid), pole.get_transform());
      renderer.draw_single(pole_object);
    }

    if (this->extension_cables) {
      renderer.draw_instanced(*(this->cable_model_invalid), extension_cables->get_transforms());
    }
  }
}

void PowerLineTool::on_mouse_button(int button, int action, int) noexcept {
  if (!valid) {
    return;
  }

  if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
    auto collidable_ptr = std::make_shared<geometry::Collidable>(data::CollisionLayer::POWER_LINES, pole_colliders,
                                                                 pole_shape, pole.get_position().getGlobal());

    data::PowerLinePole::ptr ending_pole;
    if (!this->snapping) {
      const auto obstacles = world.get_collision_space().find_colliding_with(*collidable_ptr);
      this->delete_obstacles(obstacles);

      ending_pole = std::make_shared<data::PowerLinePole>(pole);
      collidable_ptr->set_user_data(ending_pole.get());
      world.get_collision_space().insert(collidable_ptr);
      world.getMap().add_power_pole(ending_pole);
    } else {
      ending_pole = this->snap_to;
    }

    switch (this->state) {
    case START: {
      this->starting_pole = ending_pole;
      this->state = EXTEND;
      this->extension_cables = std::make_shared<data::PowerLineCable>(*(this->starting_pole), pole);
      return;
    }
    case EXTEND: {
      // Add power line cable and set neighbors
      world.getMap().add_power_cable(std::make_shared<data::PowerLineCable>(*(this->starting_pole), *ending_pole));
      this->starting_pole->add_neighbor(ending_pole);
      ending_pole->add_neighbor(this->starting_pole);

      // Rotate previous pole to average between all neigbors
      const auto& starting_pole_neighbors = this->starting_pole->get_neigbors();
      float avg_rotation = this->compute_rotation(this->starting_pole, starting_pole_neighbors);
      this->starting_pole->set_rotation(avg_rotation);

      // Switch to a new pole
      if (this->snapping) {
        this->starting_pole = nullptr;
        this->extension_cables = nullptr;
        this->stop_snapping();

        // Update ending pole rotation
        float end_rotation = this->compute_rotation(ending_pole, ending_pole->get_neigbors());
        ending_pole->set_rotation(end_rotation);

        // Change state back to START
        this->state = START;
      } else {
        this->starting_pole = ending_pole;
        this->extension_cables = std::make_shared<data::PowerLineCable>(*(this->starting_pole), pole);
      }
      break;
    }
    }
  }

  if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
    this->state = START;
    this->extension_cables.reset();
  }
}

std::shared_ptr<Brush> PowerLineTool::get_brush_ptr() const noexcept {
  return brush_ptr;
}

void PowerLineTool::mark_valid(bool is_valid) noexcept {
  if (is_valid != this->valid) {
    this->valid = is_valid;
    update_brush_color();
    this->brush_dirty = true;
  }
}

void PowerLineTool::update_brush_color() noexcept {
  if (valid) {
    this->brush_ptr->set_base_colors(BRUSH_COLOR_VALID_BASE, BRUSH_COLOR_VALID_BORDER);
    this->brush_ptr->set_active_colors(BRUSH_COLOR_VALID_BASE, BRUSH_COLOR_VALID_BORDER);
  } else {
    this->brush_ptr->set_base_colors(BRUSH_COLOR_INVALID_BASE, BRUSH_COLOR_INVALID_BORDER);
    this->brush_ptr->set_active_colors(BRUSH_COLOR_INVALID_BASE, BRUSH_COLOR_INVALID_BORDER);
  }
}

void PowerLineTool::delete_obstacles(const std::vector<geometry::Collidable::ptr>& collidables) noexcept {
  for (auto& collidable : collidables) {

    if (collidable->get_user_data() != nullptr) {
      switch (collidable->get_layer_key()) {
      case data::CollisionLayer::BUILDINGS: {
        const data::Building& building = *static_cast<data::Building*>(collidable->get_user_data());
        world.getMap().remove_building(building);
        break;
      }

      case data::CollisionLayer::TREES: {
        const data::Tree& tree = *static_cast<data::Tree*>(collidable->get_user_data());
        world.getMap().remove_tree(tree);
        break;
      }
      }
    }

    world.get_collision_space().remove(*collidable);
  }
}

float PowerLineTool::compute_rotation_towards(data::Position<float> object_pos, data::Position<float> point_pos) const
    noexcept {
  glm::vec2 object = object_pos.getGlobal();
  glm::vec2 point = point_pos.getGlobal();
  glm::vec2 delta = object - point;

  float rotation = atan2(delta.x, delta.y);
  if (rotation < 0) {
    rotation += 2 * M_PI;
  }

  return rotation;
}

float PowerLineTool::compute_rotation(data::PowerLinePole::ptr pole,
                                      const std::set<data::PowerLinePole::ptr>& neighbors) const noexcept {
  data::Position<float> pole_pos = pole->get_position();

  float rotation_sum = 0;
  size_t neighbor_count = 0;
  for (const auto& neighbor : neighbors) {
    neighbor_count++;
    data::Position<float> other_pos = neighbor->get_position();
    rotation_sum += compute_rotation_towards(pole_pos, other_pos);
  }

  if (neighbor_count == 0) {
    return 0;
  }

  float avg_rotation = rotation_sum / static_cast<float>(neighbor_count);
  if (neighbor_count > 1) {
    return fmod(avg_rotation + M_PI_2, 2 * M_PI);
  } else {
    return avg_rotation;
  }
}

data::PowerLinePole::ptr PowerLineTool::snap_point(const glm::vec2& point) const noexcept {
  // Find all poles colliding in snapping radius
  auto collidable_ptr =
      std::make_shared<geometry::Collidable>(data::CollisionLayer::POWER_LINES, pole_colliders, snapping_shape, point);
  const auto& collides_with =
      world.get_collision_space().collisions_with(*collidable_ptr, data::CollisionLayer::POWER_LINES);
  if (collides_with.empty()) {
    return nullptr;
  }

  // Take the closest one
  float min_distance = std::numeric_limits<float>::max();
  data::PowerLinePole::ptr closest_pole;
  for (const auto& other : collides_with) {
    glm::vec2 other_pos = other->get_transform();
    float distance = glm::distance(point, other_pos);
    if (distance < min_distance) {
      assert(other->get_layer_key() == data::CollisionLayer::POWER_LINES);
      min_distance = distance;
      const data::PowerLinePole& other_pole = *(static_cast<data::PowerLinePole*>(other->get_user_data()));
      closest_pole = world.getMap().find_power_line_pole_ptr(other_pole);
    }
  }

  return closest_pole;
}

void PowerLineTool::start_snapping(data::PowerLinePole::ptr snap_to) noexcept {
  if (this->snapping) {
    this->stop_snapping();
  }

  // Fix position
  this->snapping = true;
  this->snap_to = snap_to;
  this->brush_ptr->set_center(snap_to->get_position());
  this->pole.set_translation(snap_to->get_position());
  this->mark_valid(true);
  this->visible = true;

  // Update rotations
  this->previous_rotation = snap_to->get_rotation();

  switch (this->state) {
  case START: {
    this->pole.set_rotation(this->previous_rotation);
    break;
  }
  case EXTEND: {
    auto snap_to_neighbors = std::set<data::PowerLinePole::ptr>(snap_to->get_neigbors());
    snap_to_neighbors.insert(this->starting_pole);
    float avg_rotation = compute_rotation(snap_to, snap_to_neighbors);
    snap_to->set_rotation(avg_rotation);
    this->pole.set_rotation(avg_rotation);
    this->extension_cables->update_transform();
    break;
  }
  }
}

void PowerLineTool::stop_snapping() noexcept {
  this->snapping = false;

  // Restore rotation
  this->snap_to->set_rotation(this->previous_rotation);
  this->snap_to = nullptr;
}

}
