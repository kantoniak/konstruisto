#include "Road.hpp"

#include <utility>

namespace data {
Road::Road(std::vector<Position<int32_t>> tiles, geometry::Collidable::ptr body) noexcept
    : tiles(std::move(tiles)), body(std::move(body)) {
}

const std::vector<Position<int32_t>> Road::getTiles() const {
  return this->tiles;
}
}