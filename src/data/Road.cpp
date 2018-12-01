#include "Road.hpp"

#include <utility>

namespace data {
Road::Road(std::vector<Position> tiles) : tiles(std::move(tiles)) {
}

const std::vector<Position> Road::getTiles() const {
  return this->tiles;
}
}