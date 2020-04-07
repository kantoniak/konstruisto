#include "PowerLineTool.hpp"

namespace input {

const geometry::Circle::ptr PowerLineTool::pole_shape = std::make_shared<geometry::Circle>(.5f);
const geometry::Collidable::layer_key PowerLineTool::pole_colliders = collides_with(data::CollisionLayer::POWER_LINES);

PowerLineTool::PowerLineTool(input::WindowHandler& window_handler, world::World& world,
                             const rendering::ModelManager& model_manager, geometry::Geometry& geometry) noexcept
    : pole(glm::vec2(0, 0)), window_handler(window_handler), world(world), geometry(geometry) {
  this->pole_model = std::make_shared<rendering::Model>(model_manager.get_model("power-line-pole"));
}

void PowerLineTool::update() noexcept {
  glm::vec3 hitpoint;

  if (!geometry.hitGround(window_handler.getMousePositionNormalized(), hitpoint)) {
    valid = false;
    return;
  }

  const glm::vec2 new_translation(hitpoint.x, hitpoint.z);
  const data::Position<float> position(new_translation);
  if (!world.getMap().chunkExists(position.getChunk())) {
    valid = false;
    return;
  }

  auto collidable_ptr = std::make_shared<geometry::Collidable>(data::CollisionLayer::POWER_LINES, pole_colliders,
                                                               pole_shape, new_translation);

  if (world.get_collision_space().if_collides(*collidable_ptr)) {
    valid = false;
    return;
  }

  this->pole.set_translation(new_translation);
  valid = true;
}

void PowerLineTool::render(rendering::SceneRenderer& renderer) noexcept {
  if (!valid) {
    return;
  }

  rendering::Object pole_object(*(this->pole_model), pole.get_transform());
  renderer.draw_single(pole_object);
}

void PowerLineTool::on_mouse_button(int button, int action, int) noexcept {
  if (button != GLFW_MOUSE_BUTTON_LEFT || action != GLFW_PRESS || !valid) {
    return;
  }

  auto collidable_ptr = std::make_shared<geometry::Collidable>(data::CollisionLayer::POWER_LINES, pole_colliders,
                                                               pole_shape, pole.get_position().getGlobal());

  if (!world.get_collision_space().if_collides(*collidable_ptr)) {
    data::PowerLinePole::ptr new_pole_ptr = std::make_shared<data::PowerLinePole>(pole);
    collidable_ptr->set_user_data(new_pole_ptr.get());
    world.get_collision_space().insert(collidable_ptr);
    world.getMap().add_power_pole(new_pole_ptr);
  }
}

}