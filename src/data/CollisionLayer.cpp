#include "CollisionLayer.hpp"

namespace data {

geometry::Collidable::layer_key collides_with(CollisionLayer layer) noexcept {
  switch (layer) {
  case BUILDINGS:
    return BUILDINGS | ROADS | POWER_LINES;
  case ROADS:
    return BUILDINGS | POWER_LINES;
  case TREES:
    return BUILDINGS | ROADS | TREES | POWER_LINES;
  case POWER_LINES:
    return BUILDINGS | ROADS | POWER_LINES;
  default:
    return 0;
  }
}

geometry::Collidable::layer_key collides_with(const geometry::Collidable& collidable) noexcept {
  return collides_with(static_cast<CollisionLayer>(collidable.get_layer_key()));
}

}