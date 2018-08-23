#include "RoadGraph.hpp"

namespace data {

RoadGraph::RoadGraph(unsigned int sideLength) : Layer(sideLength) {
  this->layerData[sideLength * sideLength / 2 + sideLength / 2] = 'A';
}
}
