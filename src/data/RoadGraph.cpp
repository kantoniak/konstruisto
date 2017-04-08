#include "RoadGraph.hpp"

namespace data {

void RoadGraph::addRoad(Road road) {
  roads.push_back(road);
}

const std::vector<Road>& RoadGraph::getRoads() {
  return roads;
}
}
