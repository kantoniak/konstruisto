#include "CollisionLayer.hpp"

namespace data {

geometry::Collidable::layer_key collides_with(CollisionLayer layer) noexcept {
  switch (layer) {
  case BUILDINGS:
    return BUILDINGS | ROADS;
  case ROADS:
    return BUILDINGS;
  case TREES:
    return BUILDINGS | ROADS | TREES;
  default:
    return 0;
  }
}

geometry::Collidable::layer_key collides_with(const geometry::Collidable& collidable) noexcept {
  return collides_with(static_cast<CollisionLayer>(collidable.get_layer_key()));
}

}