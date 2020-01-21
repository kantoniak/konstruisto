#ifndef DATA_ROAD_HPP
#define DATA_ROAD_HPP

#include <vector>

#include "../data/Position.hpp"
#include "../geometry/Collidable.hpp"

namespace data {

class Road {

public:
  explicit Road(std::vector<Position<int32_t>> tiles, geometry::Collidable::ptr body) noexcept;
  [[nodiscard]] const std::vector<Position<int32_t>> getTiles() const;

private:
  std::vector<Position<int32_t>> tiles;
  geometry::Collidable::ptr body;
};
}

#endif
