#include "PowerLineTool.hpp"

namespace input {

PowerLineTool::PowerLineTool(input::WindowHandler& window_handler, const rendering::ModelManager& model_manager,
                             geometry::Geometry& geometry) noexcept
    : pole(glm::vec2(0, 0)), window_handler(window_handler), geometry(geometry) {
  this->pole_model = std::make_shared<rendering::Model>(model_manager.get_model("power-line-pole"));
}

void PowerLineTool::update() noexcept {
  glm::vec3 hitpoint;
  if (geometry.hitGround(window_handler.getMousePositionNormalized(), hitpoint)) {
    const glm::vec2 new_translation(hitpoint.x, hitpoint.z);
    this->pole.set_translation(new_translation);
  }
}

void PowerLineTool::render(rendering::SceneRenderer& renderer) noexcept {
  rendering::Object pole_object(*(this->pole_model), pole.get_transform());
  renderer.draw_single(pole_object);
}

}