#include "CollisionSpace.hpp"

namespace geometry {

void CollisionSpace::insert(std::shared_ptr<Collidable> collidable) noexcept {
  collidables.push_back(collidable);
}

bool CollisionSpace::if_collides(const Collidable& collidable) const noexcept {
  return std::any_of(collidables.begin(), collidables.end(), [collidable](auto& other_collidable_ptr) {
    return collidable.test_collision(*other_collidable_ptr);
  });
}
}