#include "Road.hpp"

#include <utility>

namespace data {
Road::Road(std::vector<Position<int32_t>> tiles) : tiles(std::move(tiles)) {
}

const std::vector<Position<int32_t>> Road::getTiles() const {
  return this->tiles;
}
}