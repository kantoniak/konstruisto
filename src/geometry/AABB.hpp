#ifndef GEOMETRY_AABB_HPP
#define GEOMETRY_AABB_HPP

#include <glm/glm.hpp>

#include "Circle.hpp"
#include "CollisionTester.hpp"
#include "Shape.hpp"

namespace geometry {

class AABB : public Shape {
public:
  AABB(glm::vec2 from, glm::vec2 to) noexcept;
  virtual ~AABB() = default;

  [[nodiscard]] const glm::vec2& get_from() const noexcept;
  [[nodiscard]] const glm::vec2& get_to() const noexcept;

  [[nodiscard]] bool test_collision_dd(const glm::vec2& transform, const Shape& other,
                                               const glm::vec2& other_transform) const noexcept override;
  [[nodiscard]] bool test_collision(const glm::vec2& transform, const AABB& other,
                                            const glm::vec2& other_transform) const noexcept override;
  [[nodiscard]] bool test_collision(const glm::vec2& transform, const Circle& other,
                                            const glm::vec2& other_transform) const noexcept override;

private:
  using Shape::test_collision;
  glm::vec2 from;
  glm::vec2 to;
};
}

#endif