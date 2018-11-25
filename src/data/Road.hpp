#ifndef DATA_ROAD_HPP
#define DATA_ROAD_HPP

#include <vector>

#include "../data/Position.hpp"

namespace data {

class Road {

public:
  Road(const std::vector<Position>& tiles);
  const std::vector<Position> getTiles() const;

private:
  std::vector<Position> tiles;
};
}

#endif
