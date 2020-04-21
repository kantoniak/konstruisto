#include "PowerLineTool.hpp"

namespace input {

const geometry::Circle::ptr PowerLineTool::pole_shape = std::make_shared<geometry::Circle>(.6f);
const geometry::Collidable::layer_key PowerLineTool::pole_colliders = collides_with(data::CollisionLayer::POWER_LINES);

const glm::vec4 PowerLineTool::BRUSH_COLOR_VALID_BASE = glm::vec4(1, 1, 0, 0.05f);
const glm::vec4 PowerLineTool::BRUSH_COLOR_VALID_BORDER = glm::vec4(1, 1, 0, 0.4f);
const glm::vec4 PowerLineTool::BRUSH_COLOR_INVALID_BASE = glm::vec4(1, 0, 0, 0.05f);
const glm::vec4 PowerLineTool::BRUSH_COLOR_INVALID_BORDER = glm::vec4(1, 0, 0, 0.4f);

PowerLineTool::PowerLineTool(input::WindowHandler& window_handler, world::World& world,
                             const rendering::ModelManager& model_manager, geometry::Geometry& geometry,
                             std::shared_ptr<Brush>& current_brush) noexcept
    : pole(glm::vec2(0, 0)), window_handler(window_handler), world(world), geometry(geometry),
      state_brush_ptr(current_brush) {
  this->pole_model = std::make_shared<rendering::Model>(model_manager.get_model("power-line-pole"));
  this->pole_model_invalid = std::make_shared<rendering::Model>(model_manager.get_model("power-line-pole-invalid"));
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

  const glm::vec2 new_translation(hitpoint.x, hitpoint.z);
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

  this->pole.set_translation(new_translation);
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
    rendering::Object pole_object(*(this->pole_model), pole.get_transform());
    renderer.draw_single(pole_object);
  } else {
    rendering::Object pole_object(*(this->pole_model_invalid), pole.get_transform());
    renderer.draw_single(pole_object);
  }
}

void PowerLineTool::on_mouse_button(int button, int action, int) noexcept {
  if (button != GLFW_MOUSE_BUTTON_LEFT || action != GLFW_PRESS || !valid) {
    return;
  }

  auto collidable_ptr = std::make_shared<geometry::Collidable>(data::CollisionLayer::POWER_LINES, pole_colliders,
                                                               pole_shape, pole.get_position().getGlobal());

  const auto obstacles = world.get_collision_space().find_colliding_with(*collidable_ptr);
  this->delete_obstacles(obstacles);

  data::PowerLinePole::ptr new_pole_ptr = std::make_shared<data::PowerLinePole>(pole);
  collidable_ptr->set_user_data(new_pole_ptr.get());
  world.get_collision_space().insert(collidable_ptr);
  world.getMap().add_power_pole(new_pole_ptr);
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

}