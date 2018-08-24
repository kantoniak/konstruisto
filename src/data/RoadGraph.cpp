#include "RoadGraph.hpp"

#include <iostream>

namespace data {

RoadGraph::RoadGraph(unsigned int sideLength) : Layer(sideLength) {
}

void RoadGraph::addRoad(const data::Road road) {
  for (auto& tile : road.getTiles()) {
    const unsigned int x = tile.getLocal().x;
    const unsigned int y = tile.getLocal().y;
    this->layerData[y * sideLength + x] = 1;
  }
  this->update();
}

void RoadGraph::update() {
  for (unsigned int i = 0; i < this->sideLength * this->sideLength; i++) {
    this->updateIndex(i);
  }
}

void RoadGraph::updateIndex(unsigned int i) {
  if (this->layerData[i] == NO_ROAD) {
    return;
  }
  char newValue = 1 | 2 | 4 | 8;
  if (i < sideLength || this->layerData[i - sideLength] == NO_ROAD) {
    newValue ^= 1;
  }
  if (i + sideLength >= sideLength * sideLength || this->layerData[i + sideLength] == NO_ROAD) {
    newValue ^= 2;
  }
  if (i % sideLength == 0 || this->layerData[i - 1] == NO_ROAD) {
    newValue ^= 4;
  }
  if (i % sideLength == sideLength - 1 || this->layerData[i + 1] == NO_ROAD) {
    newValue ^= 8;
  }
  if (newValue == 0) {
    newValue = ROAD_NSWE;
  }
  this->layerData[i] = newValue;
}
}
