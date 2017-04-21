#ifndef DATA_ROADGRAPH_HPP
#define DATA_ROADGRAPH_HPP

#include <cassert>
#include <stdexcept>
#include <utility>
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
    Road *N = nullptr, *S = nullptr, *W = nullptr, *E = nullptr;
    bool hasN = false, hasS = false, hasW = false, hasE = false;
    Position position;
    glm::ivec2 size;

    bool isIntersection() const {
      return size.x == size.y;
    }

    bool isStartNode() const {
      return !isIntersection() && (hasN || hasW);
    }

    bool isEndNode() const {
      return !isIntersection() && (hasS || hasE);
    }
  };

  void addRoad(const Road& road);
  const std::vector<Road>& getRoads() const;

  const std::vector<Node>& getNodes() const;

private:
  std::vector<Road> roads;
  std::vector<Node> nodes;

  bool hasRoadAt(const std::vector<Road>& roads, const glm::ivec2 global) const;
  Road& getRoadAt(std::vector<Road>& roads, const glm::ivec2 global) const;

  // TODO(kantoniak): I need <optional> so bad...
  bool hasNodeAt(const std::vector<Node>& nodes, const glm::ivec2 global) const;
  Node& getNodeAt(std::vector<Node>& nodes, const glm::ivec2 global) const;
  std::vector<Node> getNodesCopy(std::vector<Road>& roadsCopy) const;

  Node& divideRoadAt(std::vector<Road>& roads, std::vector<Node>& nodes, const glm::ivec2 global);

  void deleteNode(std::vector<Node>& nodes, Node& a);

  // FIXME(kantoniak): Copied from Geometry, fix this
  template <typename T>
  bool checkRectIntersection(glm::tvec2<T> a1, glm::tvec2<T> a2, glm::tvec2<T> b1, glm::tvec2<T> b2) const;
  bool checkIntersection(const data::Road& a, const data::Road& b) const;
  const glm::ivec2 getEnd(const data::Road& road) const;
};
}

#endif