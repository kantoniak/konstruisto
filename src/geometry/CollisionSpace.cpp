#include "CollisionSpace.hpp"

namespace geometry {

void CollisionSpace::insert(Collidable::ptr collidable_ptr) noexcept {
  collidables.push_back(collidable_ptr);
}

bool CollisionSpace::remove(const Collidable& collidable) noexcept {
  auto to_remove = std::find_if(collidables.begin(), collidables.end(),
                                [&](const Collidable::ptr& ptr) { return *ptr == collidable; });
  if (to_remove == collidables.end()) {
    return false;
  }

  collidables.erase(to_remove);
  return true;
}

void CollisionSpace::clear() noexcept {
  collidables.clear();
}

bool CollisionSpace::if_collides(const Collidable& collidable) const noexcept {
  return std::any_of(collidables.begin(), collidables.end(), [collidable](auto& other_collidable_ptr) {
    const Collidable& other = *other_collidable_ptr;

    // Skip objects in different layers
    if ((other.get_layer_key() & collidable.get_colliding_layers()) == 0) {
      return false;
    }

    return collidable.test_collision(other);
  });
}

std::vector<Collidable::ptr> CollisionSpace::collisions_with(const Collidable& collidable,
                                                             const geometry::Collidable::layer_key layers) const
    noexcept {
  std::vector<Collidable::ptr> result;
  std::copy_if(collidables.begin(), collidables.end(), std::back_inserter(result),
               [collidable, layers](auto& other_collidable_ptr) {
                 const Collidable& other = *other_collidable_ptr;

                 // Skip objects in different layers
                 if ((other.get_layer_key() & layers) == 0) {
                   return false;
                 }

                 return collidable.test_collision(other);
               });
  return result;
}

std::vector<Collidable::ptr> CollisionSpace::find_colliding_with(const Collidable& collidable) const noexcept {
  std::vector<Collidable::ptr> result;
  std::copy_if(collidables.begin(), collidables.end(), std::back_inserter(result),
               [collidable](auto& other_collidable_ptr) {
                 const Collidable& other = *other_collidable_ptr;

                 // Skip objects in different layers
                 if ((other.get_colliding_layers() & collidable.get_layer_key()) == 0) {
                   return false;
                 }

                 return collidable.test_collision(other);
               });
  return result;
}

const std::vector<Collidable::ptr>& CollisionSpace::get_collidables() const noexcept {
  return collidables;
}
}