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
#include "Brush.hpp"
#include "Tool.hpp"

namespace input {

class PowerLineTool : public Tool {

public:
  PowerLineTool(input::WindowHandler& window_handler, world::World& world, const rendering::ModelManager& model_manager,
                geometry::Geometry& geometry, std::shared_ptr<Brush>& current_brush) noexcept;
  void update() noexcept override;
  void pre_render(rendering::WorldRenderer& renderer) noexcept override;
  void render(rendering::SceneRenderer& renderer) noexcept override;

  void on_mouse_button(int button, int action, int mods) noexcept override;
  [[nodiscard]] std::shared_ptr<Brush> get_brush_ptr() const noexcept;

private:
  const static geometry::Circle::ptr pole_shape;
  const static geometry::Collidable::layer_key pole_colliders;
  const static glm::vec4 BRUSH_COLOR_VALID_BASE;
  const static glm::vec4 BRUSH_COLOR_VALID_BORDER;
  const static glm::vec4 BRUSH_COLOR_INVALID_BASE;
  const static glm::vec4 BRUSH_COLOR_INVALID_BORDER;

  bool valid = false;
  bool visible = true;
  data::PowerLinePole pole;
  std::shared_ptr<rendering::Model> pole_model;
  std::shared_ptr<rendering::Model> pole_model_invalid;
  std::shared_ptr<Brush> brush_ptr;

  input::WindowHandler& window_handler;
  world::World& world;
  geometry::Geometry& geometry;

  bool brush_dirty = true;
  void mark_valid(bool is_valid) noexcept;
  void set_visible(bool is_visible) noexcept;

  std::shared_ptr<Brush>& state_brush_ptr;

  void update_brush_color() noexcept;
  void delete_obstacles(const std::vector<geometry::Collidable::ptr>& collidables) noexcept;
};
}

#endif
