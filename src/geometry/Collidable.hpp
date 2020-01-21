#ifndef GEOMETRY_COLLIDABLE_HPP
#define GEOMETRY_COLLIDABLE_HPP

#include <memory>

#include <glm/glm.hpp>

#include "Shape.hpp"

namespace geometry {

class Collidable {
public:
  using ptr = std::shared_ptr<Collidable>;
  using layer_key = uint8_t;

  Collidable(layer_key layer, layer_key colliding_layers, Shape::ptr shape, glm::vec2 transform) noexcept;

  bool operator==(const Collidable& other) const noexcept;

  [[nodiscard]] const layer_key get_layer_key() const noexcept;
  [[nodiscard]] const layer_key get_colliding_layers() const noexcept;
  [[nodiscard]] const Shape& get_shape() const noexcept;
  [[nodiscard]] const glm::vec2& get_transform() const noexcept;

  [[nodiscard]] bool test_collision(const Collidable& other) const noexcept;

private:
  layer_key layer;
  layer_key colliding_layers;
  Shape::ptr shape;

  /// Object transform. Currently only translation in 2D world.
  glm::vec2 transform;
};
}
#endif