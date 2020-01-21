#ifndef GEOMETRY_SHAPE_HPP
#define GEOMETRY_SHAPE_HPP

#include <glm/glm.hpp>
#include <memory>

namespace geometry {

class AABB;
class Circle;

class Shape {
public:
  using ptr = std::shared_ptr<Shape>;

  [[nodiscard]] virtual bool test_collision_dd(const glm::vec2& transform, const Shape& other,
                                               const glm::vec2& other_transform) const noexcept = 0;
  [[nodiscard]] virtual bool test_collision(const glm::vec2& transform, const AABB& other,
                                            const glm::vec2& other_transform) const noexcept = 0;
  [[nodiscard]] virtual bool test_collision(const glm::vec2& transform, const Circle& other,
                                            const glm::vec2& other_transform) const noexcept = 0;
};
}

#endif