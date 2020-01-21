#ifndef GEOMETRY_COLLIDABLE_HPP
#define GEOMETRY_COLLIDABLE_HPP

#include <memory>

#include <glm/glm.hpp>

#include "Shape.hpp"

namespace geometry {

class Collidable {
public:
  Collidable(std::shared_ptr<Shape> shape, glm::vec2 transform) noexcept;

  [[nodiscard]] const std::shared_ptr<Shape>& get_shape() const noexcept;
  [[nodiscard]] const glm::vec2& get_transform() const noexcept;

  [[nodiscard]] bool test_collision(const Collidable& other) const noexcept;

private:
  std::shared_ptr<Shape> shape;

  /// Object transform. Currently only translation in 2D world.
  glm::vec2 transform;
};
}
#endif