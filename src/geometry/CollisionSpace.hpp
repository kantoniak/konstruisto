#ifndef GEOMETRY_COLLISIONSPACE_HPP
#define GEOMETRY_COLLISIONSPACE_HPP

#include <algorithm>
#include <memory>
#include <vector>

#include "Collidable.hpp"
#include "Shape.hpp"

namespace geometry {

class CollisionSpace {
public:
  void insert(std::shared_ptr<Collidable> collidable) noexcept;

  [[nodiscard]] bool if_collides(const Collidable& collidable) const noexcept;

private:
  std::vector<std::shared_ptr<Collidable>> collidables;

  template <typename T, typename U>
  static bool test_collision(const T& a, const U& b, glm::vec2 a_transform, glm::vec2 b_transform) noexcept {
    return true;
  }
};
}

#endif