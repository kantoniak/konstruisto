#ifndef DATA_ROADGRAPH_HPP
#define DATA_ROADGRAPH_HPP

#include <vector>

#include "Road.hpp"

namespace data {

class RoadGraph {

public:
  void addRoad(Road road);
  const std::vector<Road>& getRoads();

private:
  std::vector<Road> roads;
};
}

#endif