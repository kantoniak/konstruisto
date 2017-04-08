#ifndef DATA_ROADGRAPH_HPP
#define DATA_ROADGRAPH_HPP

#include <vector>

#include "Direction.hpp"
#include "Position.hpp"
#include "Road.hpp"

namespace data {

class RoadGraph {

public:
  class Node {
  public:
    // TODO(kantoniak): Switch to C++17 and use <optional>
    Road *N, *S, *W, *E;
    bool hasN, hasS, hasW, hasE;
    Position position;
    glm::ivec2 size;
  };

  void addRoad(Road road);
  const std::vector<Road>& getRoads() const;

  const std::vector<Node>& getNodes() const;

private:
  std::vector<Road> roads;
  std::vector<Node> nodes;
};
}

#endif