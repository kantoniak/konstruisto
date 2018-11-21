#include "RoadGraph.hpp"

namespace data {

RoadGraph::RoadGraph(unsigned int sideLength)
    : Layer(sideLength), neighborN(nullptr), neighborS(nullptr), neighborW(nullptr), neighborE(nullptr) {
}

RoadGraph::RoadGraph(const RoadGraph & roadGraph) : Layer(roadGraph), neighborN(roadGraph.neighborN), neighborS(roadGraph.neighborS), neighborW(roadGraph.neighborW), neighborE(roadGraph.neighborE) {
}

void RoadGraph::setNeighborN(RoadGraph* neighborN) {
  this->neighborN = neighborN;
}

void RoadGraph::setNeighborS(RoadGraph* neighborS) {
  this->neighborS = neighborS;
}

void RoadGraph::setNeighborW(RoadGraph* neighborW) {
  this->neighborW = neighborW;
}

void RoadGraph::setNeighborE(RoadGraph* neighborE) {
  this->neighborE = neighborE;
}

void RoadGraph::addRoad(const data::Road road) {
  for (auto& tile : road.getTiles()) {
    const unsigned int x = tile.getLocal().x;
    const unsigned int y = tile.getLocal().y;
    this->layerData[y * sideLength + x] = 1;
  }
}

void RoadGraph::update(const std::vector<data::Position>& tiles) {
  for (auto& tile : tiles) {
    this->updateIndex(tile.getLocalIndex());
  }
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
  if (noRoadToN(i)) {
    newValue ^= 1;
  }
  if (noRoadToS(i)) {
    newValue ^= 2;
  }
  if (noRoadToW(i)) {
    newValue ^= 4;
  }
  if (noRoadToE(i)) {
    newValue ^= 8;
  }
  if (newValue == 0) {
    newValue = ROAD_NSWE;
  }
  this->layerData[i] = newValue;
}

bool RoadGraph::noRoadToN(unsigned int i) const {
  if (i < sideLength) {
    if (neighborN == nullptr) {
      return true;
    }
    return neighborN->layerData[sideLength * (sideLength - 1) + i] == NO_ROAD;
  } else {
    return this->layerData[i - sideLength] == NO_ROAD;
  }
}

bool RoadGraph::noRoadToS(unsigned int i) const {
  if (i + sideLength >= sideLength * sideLength) {
    if (neighborS == nullptr) {
      return true;
    }
    return neighborS->layerData[i % sideLength] == NO_ROAD;
  } else {
    return this->layerData[i + sideLength] == NO_ROAD;
  }
}

bool RoadGraph::noRoadToW(unsigned int i) const {
  if (i % sideLength == 0) {
    if (neighborW == nullptr) {
      return true;
    }
    return neighborW->layerData[i + sideLength - 1] == NO_ROAD;
  } else {
    return this->layerData[i - 1] == NO_ROAD;
  }
}

bool RoadGraph::noRoadToE(unsigned int i) const {
  if (i % sideLength == sideLength - 1) {
    if (neighborE == nullptr) {
      return true;
    }
    return neighborE->layerData[i - sideLength + 1] == NO_ROAD;
  } else {
    return this->layerData[i + 1] == NO_ROAD;
  }
}
}
