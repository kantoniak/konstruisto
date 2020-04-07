#ifndef INPUT_POWERLINETOOL_HPP
#define INPUT_POWERLINETOOL_HPP

#include <memory>

#include <glm/glm.hpp>

#include "../data/PowerLinePole.hpp"
#include "../geometry/Geometry.hpp"
#include "../input/WindowHandler.hpp"
#include "../rendering/Model.hpp"
#include "../rendering/ModelManager.hpp"
#include "../rendering/Object.hpp"
#include "Tool.hpp"

namespace input {

class PowerLineTool : public Tool {

public:
  PowerLineTool(input::WindowHandler& window_handler, const rendering::ModelManager& model_manager,
                geometry::Geometry& geometry) noexcept;
  void update() noexcept override;
  void render(rendering::SceneRenderer& renderer) noexcept override;

protected:
  data::PowerLinePole pole;
  std::shared_ptr<rendering::Model> pole_model;

  input::WindowHandler& window_handler;
  geometry::Geometry& geometry;
};
}

#endif
