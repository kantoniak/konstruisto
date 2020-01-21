#ifndef GEOMETRY_CIRCLE_HPP
#define GEOMETRY_CIRCLE_HPP

#include <glm/glm.hpp>

#include "Shape.hpp"

namespace geometry {

class Circle : public Shape {
public:
  Circle(float radius) noexcept;
  virtual ~Circle() = default;

  [[nodiscard]] float get_radius() const noexcept;

  [[nodiscard]] virtual bool test_collision_dd(const glm::vec2& transform, const Shape& other,
                                               const glm::vec2& other_transform) const noexcept;
  [[nodiscard]] virtual bool test_collision(const glm::vec2& transform, const Circle& other,
                                            const glm::vec2& other_transform) const noexcept;

private:
  using Shape::test_collision;
  float radius;
};
}

#endif