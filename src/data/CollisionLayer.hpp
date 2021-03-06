#ifndef DATA_COLLISION_LAYER_HPP
#define DATA_COLLISION_LAYER_HPP

#include "../geometry/Collidable.hpp"

namespace data {

enum CollisionLayer { NONE = 0, BUILDINGS = 0b1, ROADS = 0b10, TREES = 0b100, POWER_LINES = 0b1000 };

geometry::Collidable::layer_key collides_with(CollisionLayer layer) noexcept;
geometry::Collidable::layer_key collides_with(const geometry::Collidable& collidable) noexcept;
}

#endif