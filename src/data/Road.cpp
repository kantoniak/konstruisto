#include "Road.hpp"

namespace data {
Road::Road(const std::vector<Position>& tiles) : tiles(tiles) {
}

const std::vector<Position> Road::getTiles() const {
  return this->tiles;
}
}