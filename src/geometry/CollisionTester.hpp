#ifndef GEOMETRY_COLLISIONTESTER_HPP
#define GEOMETRY_COLLISIONTESTER_HPP

#include <glm/glm.hpp>

namespace geometry {

class CollisionTester {
public:
  static bool point_in_rect(glm::vec2 point, glm::vec2 from, glm::vec2 to) noexcept;

  template <typename T, typename U>
  [[nodiscard]] static bool test_collision(const T& a, const glm::vec2& a_transform, const U& b,
                                           const glm::vec2& b_transform) noexcept;
};
}

#endif