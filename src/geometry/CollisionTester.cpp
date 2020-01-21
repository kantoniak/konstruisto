#include "CollisionTester.hpp"

#include "AABB.hpp"
#include "Circle.hpp"

namespace geometry {

bool CollisionTester::point_in_rect(glm::vec2 point, glm::vec2 from, glm::vec2 to) noexcept {
  return (from.x <= point.x && point.x <= to.x && from.y <= point.y && point.y <= to.y);
}

template <>
bool CollisionTester::test_collision<AABB, AABB>(const AABB& a, const glm::vec2& a_transform, const AABB& b,
                                                 const glm::vec2& b_transform) noexcept {
  const glm::vec2 a1 = a.get_from() + a_transform;
  const glm::vec2 a2 = a.get_to() + a_transform;
  const glm::vec2 b1 = b.get_from() + b_transform;
  const glm::vec2 b2 = b.get_to() + b_transform;
  return (a1.x < b2.x && a1.y < b2.y && b1.x < a2.x && b1.y < a2.y);
}

template <>
bool CollisionTester::test_collision<Circle, Circle>(const Circle& a, const glm::vec2& a_transform, const Circle& b,
                                                     const glm::vec2& b_transform) noexcept {
  const glm::vec2 delta = glm::abs(a_transform - b_transform);
  const float radius_sum = a.get_radius() + b.get_radius();
  return !(radius_sum * radius_sum < (delta.x * delta.x + delta.y * delta.y));
}

template <>
bool CollisionTester::test_collision<AABB, Circle>(const AABB& aabb, const glm::vec2& aabb_transform,
                                                   const Circle& circle, const glm::vec2& circle_center) noexcept {
  assert(aabb.get_from().x < aabb.get_to().x && aabb.get_from().y < aabb.get_to().y);
  const glm::vec2 aabb_from = aabb.get_from() + aabb_transform;
  const glm::vec2 aabb_to = aabb.get_to() + aabb_transform;

  if (point_in_rect(circle_center, aabb_from, aabb_to)) {
    return true;
  }

  // Following https://developer.mozilla.org/en-US/docs/Games/Techniques/3D_collision_detection
  const glm::vec2 delta = glm::max(aabb_from, glm::min(circle_center, aabb_to));
  const float dist_sq = delta.x * delta.x + delta.y * delta.y;
  const float radius = circle.get_radius();
  return !(radius * radius < dist_sq);
}

template <>
bool CollisionTester::test_collision<Circle, AABB>(const Circle& circle, const glm::vec2& circle_transform,
                                                   const AABB& aabb, const glm::vec2& aabb_transform) noexcept {
  return test_collision<>(aabb, aabb_transform, circle, circle_transform);
}

}