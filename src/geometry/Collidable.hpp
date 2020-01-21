#ifndef GEOMETRY_COLLIDABLE_HPP
#define GEOMETRY_COLLIDABLE_HPP

#include <memory>

#include <glm/glm.hpp>

#include "Shape.hpp"

namespace geometry {

class Collidable {
public:
  using ptr = std::shared_ptr<Collidable>;

  Collidable(Shape::ptr shape, glm::vec2 transform) noexcept;

  bool operator==(const Collidable& other) const noexcept;

  [[nodiscard]] const Shape& get_shape() const noexcept;
  [[nodiscard]] const glm::vec2& get_transform() const noexcept;

  [[nodiscard]] bool test_collision(const Collidable& other) const noexcept;

private:
  Shape::ptr shape;

  /// Object transform. Currently only translation in 2D world.
  glm::vec2 transform;
};
}
#endif