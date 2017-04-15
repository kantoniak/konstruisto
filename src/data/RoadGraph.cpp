#include "RoadGraph.hpp"
#include <iostream>
namespace data {

void RoadGraph::addRoad(const Road& road) {

  std::vector<Road> roadsCopy = roads;
  std::vector<Node> nodesCopy = nodes;
  roadsCopy.push_back(road);

  Road& current = roadsCopy.back();

  // We will later check intersections in the middle basing on this.
  glm::ivec2 middleStart = current.position.getGlobal();
  glm::ivec2 middleEnd = getEnd(current);

  /// Start
  if (hasNodeAt(nodes, current.position.getGlobal()) && getNodeAt(nodes, current.position.getGlobal()).isSquare()) {
    // Hit intersection

    Node& node = getNodeAt(nodesCopy, current.position.getGlobal());
    if (Direction::W == current.direction) {
      node.hasW = true;
      node.W = &current;
      middleStart.x += current.getType().width;
    } else {
      node.hasN = true;
      node.N = &current;
      middleStart.y += current.getType().width;
    }

  } else if (hasRoadAt(roads, current.position.getGlobal())) {
    // Hit road

    Node& newNode = divideRoadAt(roadsCopy, nodesCopy, current.position.getGlobal());
    if (Direction::W == current.direction) {
      newNode.hasW = true;
      newNode.W = &current;
      middleStart.x += current.getType().width;
    } else {
      newNode.hasN = true;
      newNode.N = &current;
      middleStart.y += current.getType().width;
    }

  } else {
    // Standard end

    Node start;
    start.position.setGlobal(current.position.getGlobal());
    if (Direction::N == current.direction) {
      start.size = glm::ivec2(current.getType().width, 1);
      start.hasN = true;
      start.N = &current;
      middleStart.y += 1;
    } else {
      start.size = glm::ivec2(1, current.getType().width);
      start.hasW = true;
      start.W = &current;
      middleStart.x += 1;
    }
    nodesCopy.push_back(start);
  }

  /// End
  const glm::ivec2 endIntersectionPos = getEnd(current) - glm::ivec2(1, 1) * (current.getType().width - 1);
  if (hasNodeAt(nodes, endIntersectionPos) && getNodeAt(nodes, endIntersectionPos).isSquare()) {
    // Hit intersection

    Node& node = getNodeAt(nodesCopy, endIntersectionPos);
    if (Direction::W == current.direction) {
      node.hasE = true;
      node.E = &current;
      middleEnd.x -= current.getType().width;
    } else {
      node.hasS = true;
      node.S = &current;
      middleEnd.y -= current.getType().width;
    }

  } else if (hasRoadAt(roads, getEnd(current))) {
    // Hit road

    Node& newNode = divideRoadAt(roadsCopy, nodesCopy, endIntersectionPos);
    if (Direction::W == current.direction) {
      newNode.hasE = true;
      newNode.E = &current;
      middleEnd.x -= current.getType().width;
    } else {
      newNode.hasS = true;
      newNode.S = &current;
      middleEnd.y -= current.getType().width;
    }

  } else {
    // Standard end

    Node end;
    if (Direction::N == current.direction) {
      end.position.setGlobal(current.position.getGlobal() + glm::ivec2(0, current.length - 1));
      end.size = glm::ivec2(current.getType().width, 1);
      end.hasS = true;
      end.S = &current;
      middleEnd.y -= 1;
    } else {
      end.position.setGlobal(current.position.getGlobal() + glm::ivec2(current.length - 1, 0));
      end.size = glm::ivec2(1, current.getType().width);
      end.hasE = true;
      end.E = &current;
      middleEnd.x -= 1;
    }
    nodesCopy.push_back(end);
  }

  /// Middle
  if (Direction::W == current.direction) {
    for (int x = middleStart.x; x <= middleEnd.x; x++) {
      glm::ivec2 testedPoint = glm::ivec2(x, current.position.getGlobal().y);

      // TODO(kantoniak): if (hasNode) add to node and continue
      if (hasRoadAt(roads, testedPoint)) {
        roadsCopy.push_back(current);
        Road& oldCurrent = roadsCopy.back();
        oldCurrent.length = x - current.position.getGlobal().x;
        current.position.setGlobal(testedPoint);
        current.length -= oldCurrent.length;
        oldCurrent.length += oldCurrent.getType().width;

        Node& newNode = divideRoadAt(roadsCopy, nodesCopy, testedPoint);
        newNode.hasE = true;
        newNode.E = &oldCurrent;
        newNode.hasW = true;
        newNode.W = &current;

        x += oldCurrent.getType().width;
        continue;
      }
    }
  } else {
    for (int y = middleStart.y; y <= middleEnd.y; y++) {
      glm::ivec2 testedPoint = glm::ivec2(current.position.getGlobal().x, y);

      // TODO(kantoniak): if (hasNode) add to node and continue
      if (hasRoadAt(roads, testedPoint)) {
        roadsCopy.push_back(current);
        Road& oldCurrent = roadsCopy.back();
        oldCurrent.length = y - current.position.getGlobal().y;
        current.position.setGlobal(testedPoint);
        current.length -= oldCurrent.length;
        oldCurrent.length += oldCurrent.getType().width;

        Node& newNode = divideRoadAt(roadsCopy, nodesCopy, testedPoint);
        newNode.hasS = true;
        newNode.S = &oldCurrent;
        newNode.hasN = true;
        newNode.N = &current;

        y += oldCurrent.getType().width;
        continue;
      }
    }
  }

  nodes.swap(nodesCopy);
  roads.swap(roadsCopy);
}

const std::vector<Road>& RoadGraph::getRoads() const {
  return roads;
}

const std::vector<RoadGraph::Node>& RoadGraph::getNodes() const {
  return nodes;
}

bool RoadGraph::hasRoadAt(const std::vector<Road>& roads, const glm::ivec2 global) const {
  for (const data::Road& road : roads) {
    const glm::ivec2 b2 = road.position.getGlobal();
    const glm::ivec2 b1 = getEnd(road);
    if (checkRectIntersection(global, global, b1, b2)) {
      return true;
    }
  }
  return false;
}

Road& RoadGraph::getRoadAt(std::vector<Road>& roads, const glm::ivec2 global) const {
  for (data::Road& road : roads) {
    const glm::ivec2 b2 = road.position.getGlobal();
    const glm::ivec2 b1 = getEnd(road);
    if (checkRectIntersection(global, global, b1, b2)) {
      return road;
    }
  }
  throw std::invalid_argument("Road does not exist");
}

bool RoadGraph::hasNodeAt(const std::vector<Node>& nodes, const glm::ivec2 global) const {
  for (const Node& node : nodes) {
    const glm::ivec2 b2 = node.position.getGlobal();
    const glm::ivec2 b1 = node.position.getGlobal() + node.size - glm::ivec2(1, 1);
    if (checkRectIntersection(global, global, b1, b2)) {
      return true;
    }
  }
  return false;
}

RoadGraph::Node& RoadGraph::getNodeAt(std::vector<Node>& nodes, const glm::ivec2 global) const {
  for (Node& node : nodes) {
    const glm::ivec2 b2 = node.position.getGlobal();
    const glm::ivec2 b1 = node.position.getGlobal() + node.size - glm::ivec2(1, 1);
    if (checkRectIntersection(global, global, b1, b2)) {
      return node;
    }
  }
  throw std::invalid_argument("Node does not exist");
}

RoadGraph::Node& RoadGraph::divideRoadAt(std::vector<Road>& roads, std::vector<Node>& nodes, const glm::ivec2 global) {
  Road& oldRoad = getRoadAt(roads, global);

  // Special: Handle division at start
  {
    const glm::ivec2& desired = oldRoad.position.getGlobal();
    if (global.x == desired.x && global.y == desired.y) {
      Node& node = getNodeAt(nodes, global);
      node.size = glm::ivec2(1, 1) * oldRoad.getType().width;
      return node;
    }
  }

  // Special: Handle division at end
  {
    const glm::ivec2& desired = getEnd(oldRoad) - glm::ivec2(1, 1) * (oldRoad.getType().width - 1);
    if (global.x == desired.x && global.y == desired.y) {
      const glm::ivec2 nodePos =
          getEnd(oldRoad) -
          (Direction::N == oldRoad.direction ? glm::ivec2(1, 0) : glm::ivec2(0, 1)) * (oldRoad.getType().width - 1);
      Node& node = getNodeAt(nodes, nodePos);
      node.position.setGlobal(global);
      node.size = glm::ivec2(1, 1) * oldRoad.getType().width;
      return node;
    }
  }

  // Create extension road
  roads.push_back(Road());
  Road& newRoad = roads.back();
  newRoad.setType(oldRoad.getType());
  newRoad.position.setGlobal(global);
  newRoad.direction = oldRoad.direction;
  if (Direction::W == newRoad.direction) {
    newRoad.length = getEnd(oldRoad).x - global.x + 1;
  } else {
    newRoad.length = getEnd(oldRoad).y - global.y + 1;
  }

  // Shorten previous road
  if (Direction::W == oldRoad.direction) {
    oldRoad.length = global.x - oldRoad.position.getGlobal().x + oldRoad.getType().width;
  } else {
    oldRoad.length = global.y - oldRoad.position.getGlobal().y + oldRoad.getType().width;
  }

  // Pin new road at the end of old road
  Node& endNode = getNodeAt(nodes, getEnd(newRoad));
  if (Direction::W == newRoad.direction) {
    endNode.E = &newRoad;
  } else {
    endNode.S = &newRoad;
  }

  // Create intersection in the middle
  nodes.push_back(Node());
  Node& newNode = nodes.back();
  newNode.position.setGlobal(global);
  newNode.size = glm::ivec2(1, 1) * oldRoad.getType().width;
  if (Direction::W == newRoad.direction) {
    newNode.hasW = true;
    newNode.W = &newRoad;
    newNode.hasE = true;
    newNode.E = &oldRoad;
  } else {
    newNode.hasN = true;
    newNode.N = &newRoad;
    newNode.hasS = true;
    newNode.S = &oldRoad;
  }

  return newNode;
}

template <typename T>
bool RoadGraph::checkRectIntersection(glm::tvec2<T> a1, glm::tvec2<T> a2, glm::tvec2<T> b1, glm::tvec2<T> b2) const {
  return !(a1.y < b2.y || a2.y > b1.y || a1.x < b2.x || a2.x > b1.x);
}

bool RoadGraph::checkIntersection(const data::Road& a, const data::Road& b) const {
  const glm::ivec2 a2 = a.position.getGlobal();
  const glm::ivec2 a1 = getEnd(a);
  const glm::ivec2 b2 = b.position.getGlobal();
  const glm::ivec2 b1 = getEnd(b);
  return checkRectIntersection(a1, a2, b1, b2);
}

const glm::ivec2 RoadGraph::getEnd(const data::Road& road) const {
  return road.position.getGlobal() +
         glm::ivec2((road.direction == data::Direction::W ? road.length : road.getType().width) - 1,
                    (road.direction == data::Direction::N ? road.length : road.getType().width) - 1);
}
}
