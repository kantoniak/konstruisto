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
  void insert(Collidable::ptr collidable_ptr) noexcept;
  bool remove(const Collidable& collidable) noexcept;
  void clear() noexcept;

  [[nodiscard]] bool if_collides(const Collidable& collidable) const noexcept;
  [[nodiscard]] std::vector<Collidable::ptr> collisions_with(const Collidable& collidable,
                                                             const geometry::Collidable::layer_key layers) const
      noexcept;
  [[nodiscard]] std::vector<Collidable::ptr> find_colliding_with(const Collidable& collidable) const noexcept;

private:
  std::vector<Collidable::ptr> collidables;

  template <typename T, typename U>
  static bool test_collision(const T& a, const U& b, glm::vec2 a_transform, glm::vec2 b_transform) noexcept;
};
}

#endif