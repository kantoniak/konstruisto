#ifndef INPUT_POWERLINETOOL_HPP
#define INPUT_POWERLINETOOL_HPP

#include <memory>

#include <glm/glm.hpp>

#include "../data/CollisionLayer.hpp"
#include "../data/Position.hpp"
#include "../data/PowerLinePole.hpp"
#include "../geometry/Circle.hpp"
#include "../geometry/Collidable.hpp"
#include "../geometry/Geometry.hpp"
#include "../input/WindowHandler.hpp"
#include "../rendering/Model.hpp"
#include "../rendering/ModelManager.hpp"
#include "../rendering/Object.hpp"
#include "../world/World.hpp"
#include "Tool.hpp"

namespace input {

class PowerLineTool : public Tool {

public:
  PowerLineTool(input::WindowHandler& window_handler, world::World& world, const rendering::ModelManager& model_manager,
                geometry::Geometry& geometry) noexcept;
  void update() noexcept override;
  void render(rendering::SceneRenderer& renderer) noexcept override;

  void on_mouse_button(int button, int action, int mods) noexcept override;

protected:
  const static geometry::Circle::ptr pole_shape;
  const static geometry::Collidable::layer_key pole_colliders;

  bool valid = false;
  data::PowerLinePole pole;
  std::shared_ptr<rendering::Model> pole_model;

  input::WindowHandler& window_handler;
  world::World& world;
  geometry::Geometry& geometry;
};
}

#endif
