#ifndef DATA_ROAD_HPP
#define DATA_ROAD_HPP

#include <vector>

#include "../data/Position.hpp"

namespace data {

class Road {

public:
  explicit Road(std::vector<Position> tiles);
  [[nodiscard]] const std::vector<Position> getTiles() const;

private:
  std::vector<Position> tiles;
};
}

#endif
