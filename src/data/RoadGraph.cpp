#include "RoadGraph.hpp"

namespace data {

void RoadGraph::addRoad(Road road) {

  roads.push_back(road);
  Road* pointer = &(roads[roads.size() - 1]);

  Node start, end;
  start.position.setGlobal(road.position.getGlobal());

  switch (road.direction) {
  case Direction::N:
    start.size = glm::ivec2(road.getType().width, 1);
    start.hasN = true;
    start.N = pointer;
    end.position.setGlobal(road.position.getGlobal() + glm::ivec2(0, road.length - 1));
    end.size = glm::ivec2(road.getType().width, 1);
    break;
  case Direction::W:
    start.size = glm::ivec2(1, road.getType().width);
    start.hasW = true;
    start.W = pointer;
    end.position.setGlobal(road.position.getGlobal() + glm::ivec2(road.length - 1, 0));
    end.size = glm::ivec2(1, road.getType().width);
    break;
  default:
    break;
  }

  nodes.push_back(start);
  nodes.push_back(end);
}

const std::vector<Road>& RoadGraph::getRoads() const {
  return roads;
}

const std::vector<RoadGraph::Node>& RoadGraph::getNodes() const {
  return nodes;
}
}
