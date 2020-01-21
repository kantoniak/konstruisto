#ifndef GEOMETRY_SHAPE_HPP
#define GEOMETRY_SHAPE_HPP

#include <cassert>

namespace geometry {

class Circle;

class Shape {
public:
  [[nodiscard]] virtual bool test_collision_dd(const glm::vec2& transform, const Shape& other,
                                               const glm::vec2& other_transform) const noexcept = 0;
  [[nodiscard]] virtual bool test_collision(const glm::vec2& transform, const Circle& other,
                                            const glm::vec2& other_transform) const noexcept = 0;
};
}

#endif