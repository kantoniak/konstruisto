#include "RoadGraph.hpp"

namespace data {

template <unsigned int L>
RoadGraph<L>::RoadGraph()
    : Layer<char, L>(), neighborN(nullptr), neighborS(nullptr), neighborW(nullptr), neighborE(nullptr) {
}

template <unsigned int L>
RoadGraph<L>::RoadGraph(const RoadGraph& roadGraph)
    : Layer<char, L>(roadGraph), neighborN(roadGraph.neighborN), neighborS(roadGraph.neighborS),
      neighborW(roadGraph.neighborW), neighborE(roadGraph.neighborE) {
}

template <unsigned int L>
void RoadGraph<L>::setNeighborN(RoadGraph* neighborN) {
  this->neighborN = neighborN;
}

template <unsigned int L>
void RoadGraph<L>::setNeighborS(RoadGraph* neighborS) {
  this->neighborS = neighborS;
}

template <unsigned int L>
void RoadGraph<L>::setNeighborW(RoadGraph* neighborW) {
  this->neighborW = neighborW;
}

template <unsigned int L>
void RoadGraph<L>::setNeighborE(RoadGraph* neighborE) {
  this->neighborE = neighborE;
}

template <unsigned int L>
void RoadGraph<L>::addRoad(const data::Road road) {
  for (auto& tile : road.getTiles()) {
    const unsigned int x = tile.getLocal().x;
    const unsigned int y = tile.getLocal().y;
    this->layerData[y * L + x] = 1;
  }
}

template <unsigned int L>
void RoadGraph<L>::update(const std::vector<data::Position>& tiles) {
  for (auto& tile : tiles) {
    this->updateIndex(tile.getLocalIndex());
  }
}

template <unsigned int L>
void RoadGraph<L>::update() {
  for (unsigned int i = 0; i < L * L; i++) {
    this->updateIndex(i);
  }
}

template <unsigned int L>
void RoadGraph<L>::updateIndex(unsigned int i) {
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

template <unsigned int L>
bool RoadGraph<L>::noRoadToN(unsigned int i) const {
  if (i < L) {
    if (neighborN == nullptr) {
      return true;
    }
    return neighborN->layerData[L * (L - 1) + i] == NO_ROAD;
  } else {
    return this->layerData[i - L] == NO_ROAD;
  }
}

template <unsigned int L>
bool RoadGraph<L>::noRoadToS(unsigned int i) const {
  if (i + L >= L * L) {
    if (neighborS == nullptr) {
      return true;
    }
    return neighborS->layerData[i % L] == NO_ROAD;
  } else {
    return this->layerData[i + L] == NO_ROAD;
  }
}

template <unsigned int L>
bool RoadGraph<L>::noRoadToW(unsigned int i) const {
  if (i % L == 0) {
    if (neighborW == nullptr) {
      return true;
    }
    return neighborW->layerData[i + L - 1] == NO_ROAD;
  } else {
    return this->layerData[i - 1] == NO_ROAD;
  }
}

template <unsigned int L>
bool RoadGraph<L>::noRoadToE(unsigned int i) const {
  if (i % L == L - 1) {
    if (neighborE == nullptr) {
      return true;
    }
    return neighborE->layerData[i - L + 1] == NO_ROAD;
  } else {
    return this->layerData[i + 1] == NO_ROAD;
  }
}

template class RoadGraph<64u>;
}
