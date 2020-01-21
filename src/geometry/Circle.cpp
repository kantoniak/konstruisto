#include "Circle.hpp"

namespace geometry {
Circle::Circle(float radius) noexcept : radius(radius) {
}

float Circle::get_radius() const noexcept {
  return radius;
}

bool Circle::test_collision_dd(const glm::vec2& transform, const Shape& other, const glm::vec2& other_transform) const
    noexcept {
  return other.test_collision(other_transform, *this, transform);
}

bool Circle::test_collision(const glm::vec2& transform, const Circle& other, const glm::vec2& other_transform) const
    noexcept {
  const glm::vec2 delta = glm::abs(transform - other_transform);
  const float radius_sum = radius + other.radius;
  return !(radius_sum * radius_sum < (delta.x * delta.x + delta.y * delta.y));
}
}