#include "AABB.hpp"

namespace geometry {
AABB::AABB(glm::vec2 from, glm::vec2 to) noexcept : from(from), to(to) {
}

const glm::vec2& AABB::get_from() const noexcept {
  return from;
}

const glm::vec2& AABB::get_to() const noexcept {
  return to;
}

bool AABB::test_collision_dd(const glm::vec2& transform, const Shape& other, const glm::vec2& other_transform) const
    noexcept {
  return other.test_collision(other_transform, *this, transform);
}

bool AABB::test_collision(const glm::vec2& transform, const AABB& other, const glm::vec2& other_transform) const
    noexcept {
  return CollisionTester::test_collision<>(*this, transform, other, other_transform);
}
bool AABB::test_collision(const glm::vec2& transform, const Circle& other, const glm::vec2& other_transform) const
    noexcept {
  return CollisionTester::test_collision<>(*this, transform, other, other_transform);
}
}