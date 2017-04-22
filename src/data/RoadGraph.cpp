#include "RoadGraph.hpp"
#include <iostream>
namespace data {

void RoadGraph::test() {

  roads.reserve(100);

  data::Road road;
  road.setType(data::RoadTypes.Standard);
  road.position.setGlobal(glm::ivec2(2, 2));
  road.direction = data::Direction::N;
  road.length = 6;
  addRoad(road);
  describe();

  divideRoadAt(glm::ivec2(2, 4), Road());
  describe();

}

void RoadGraph::addRoad(const Road& road) {
  roads.push_back(road);
  Road& current = roads.back();

  /// Start
  if (hasNodeAt(current.position.getGlobal())) {
    Node& node = getNodeAt(current.position.getGlobal());

    if (node.isIntersection()) {

      if ((Direction::N == current.direction && node.hasN) ||
          (Direction::W == current.direction && node.hasW)) {
        Road& other = *(Direction::N == current.direction ? node.N : node.W);

        if (other.length >= current.length) {

          // Do nothing
          roads.pop_back();

        } else {

          current.position.setGlobal(current.position.getGlobal() + toVector(current.direction) * (int)(other.length - 1));
          current.length -= other.length - 1;

          Road copy = current;
          roads.pop_back();
          addRoad(copy);

        }

      } else {
        pinRoadToNodeAndIterate(current, node);
      }

    } else if (node.isStartNode()) {

      Road& other = getRoadAt(current.position.getGlobal());
      if (other.direction == current.direction) {

        if (other.length >= current.length) {

          // Do nothing
          roads.pop_back();

        } else {

          current.position.setGlobal(current.position.getGlobal() + toVector(current.direction) * (int)(other.length - 1));
          current.length -= other.length - 1;

          Road copy = current;
          roads.pop_back();
          addRoad(copy);

        }
        
      } else {
        node.size = glm::ivec2(1, 1) * current.getType().width;
        pinRoadToNodeAndIterate(current, node);
      }

    } else {
      assert(node.isEndNode());
      assert(Direction::N == current.direction || !node.hasS);
      assert(Direction::W == current.direction || !node.hasE);

      Road& other = getRoadAt(current.position.getGlobal());
      other.length += current.length - 1;

      deleteNode(nodes, node);
      roads.pop_back();

      iterateNewRoad(other, current.position.getGlobal() + toVector(current.direction));
    }

  } else if (hasRoadAt(current.position.getGlobal())) {
    // Hit road
    Road& other = getRoadAt(current.position.getGlobal());

    if (other.direction == current.direction) {

      glm::ivec2 exceeding = current.getEnd() - other.getEnd();
      bool exceeds = exceeding.x > 0 || exceeding.y > 0;

      if (exceeds) {

        if (Direction::N == current.direction) {
          current.length -= (other.getEnd() - current.position.getGlobal()).y;
          current.position.setGlobal(other.getEnd() - glm::ivec2(1, 0));
        } else {
          current.length -= (other.getEnd() - current.position.getGlobal()).x;
          current.position.setGlobal(other.getEnd() - glm::ivec2(0, 1));
        }

        Road copy = current;
        roads.pop_back();
        addRoad(copy);

      } else {

        // Do nothing
        roads.pop_back();

      }

    } else {
      Node& newNode = divideRoadAt(current.position.getGlobal(), current);
      pinRoadToNodeAndIterate(current, newNode);
    }

  } else {
    addStartNode(current);
    iterateNewRoad(current, current.position.getGlobal() + toVector(current.direction));
  }

}

RoadGraph::Node& RoadGraph::addStartNode(Road& road) {
  nodes.push_back(Node());
  Node& node = nodes.back();
  node.position.setGlobal(road.position.getGlobal());
  if (Direction::N == road.direction) {
    node.size = glm::ivec2(road.getType().width, 1);
    node.hasN = true;
    node.N = &road;
  } else {
    node.size = glm::ivec2(1, road.getType().width);
    node.hasW = true;
    node.W = &road;
  }
  return node;
}

RoadGraph::Node& RoadGraph::addEndNode(Road& road) {
  nodes.push_back(Node());
  Node& node = nodes.back();
  node.position.setGlobal(road.position.getGlobal() + toVector(road.direction) * (road.length - 1));
  if (Direction::N == road.direction) {
    node.size = glm::ivec2(road.getType().width, 1);
    node.hasS = true;
    node.S = &road;
  } else {
    node.size = glm::ivec2(1, road.getType().width);
    node.hasE = true;
    node.E = &road;
  }
  std::cout << "END NODE: (" << road.getEnd().x << " " << road.getEnd().y << ") (" << node.position.getGlobal().x << " " << node.position.getGlobal().y << ")" << std::endl;
  return node;
}

void RoadGraph::pinRoadToNodeAndIterate(Road& road, Node& node) {
  if (Direction::W == road.direction) {
    assert(!node.hasW);
    node.hasW = true;
    node.W = &road;
  } else {
    assert(!node.hasN);
    node.hasN = true;
    node.N = &road;
  }

  iterateNewRoad(road, road.position.getGlobal() + toVector(road.direction) * road.getType().width);
}

bool RoadGraph::posIsNotRoadEnd(const glm::ivec2 pos, const Road& road) const {
  if (Direction::N == road.direction) {
    return pos.y < road.getEnd().y;
  } else {
    return pos.x < road.getEnd().x;
  }
}

void RoadGraph::iterateNewRoad(Road& road, glm::ivec2 startPoint) {
  const glm::ivec2 alongDirection = toVector(road.direction);
  for (glm::ivec2 pos = startPoint; posIsNotRoadEnd(pos, road); pos += alongDirection) {
    std::cout << "TEST " << pos.x << " " << pos.y << " " << road.getEnd().x << " " << road.getEnd().y << std::endl;

    if (hasNodeAt(pos)) {

    }

    if (hasRoadAt(pos, road)) {
      Road& other = getRoadAt(pos, road);
      
      if (other.direction == road.direction) {

      } else {
        
        Node& newNode = divideRoadAt(pos, road);

        Road& oldRoad = road;
        road = Road(road);

        if (Direction::N == road.direction) {
          oldRoad.length = pos.y - road.position.getGlobal().y;
          road.position.setGlobal(pos);
          road.length -= oldRoad.length;
          oldRoad.length += oldRoad.getType().width;

          newNode.hasS = true;
          newNode.S = &oldRoad;
        } else {
          oldRoad.length = pos.x - road.position.getGlobal().x;
          road.position.setGlobal(pos);
          road.length -= oldRoad.length;
          oldRoad.length += oldRoad.getType().width;
          
          newNode.hasE = true;
          newNode.E = &oldRoad; 
        }

        addRoad(road);
        return;

      }

    }

  }

  addEndNode(road);

  /*for (int x = middleStart.x; x <= middleEnd.x; x++) {
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
    }*/
}

/*void RoadGraph::addRoad(const Road& road) {

  std::vector<Road> roadsCopy = roads;
  std::vector<Node> nodesCopy = getNodesCopy(roadsCopy);
  roadsCopy.push_back(road);

  Road& current = roadsCopy.back();

  // We will later check intersections in the middle basing on this.
  glm::ivec2 middleStart = current.position.getGlobal();
  glm::ivec2 middleEnd = current.getEnd();

  /// Start
  if (hasNodeAt(current.position.getGlobal())) {
    Node& node = getNodeAt(nodesCopy, current.position.getGlobal());

    if (getNodeAt(current.position.getGlobal()).isIntersection()) {
      // Hit intersection

      if (Direction::W == current.direction) {
        node.hasW = true;
        node.W = &current;
        middleStart.x += current.getType().width;
      } else {
        node.hasN = true;
        node.N = &current;
        middleStart.y += current.getType().width;
      }
    } else {
      // Hit road start/end

      assert(Direction::N == current.direction || (!node.hasN && !node.hasS));
      assert(Direction::W == current.direction || (!node.hasW && !node.hasE));
      assert(node.isStartNode() || node.isEndNode());
      // TODO(kantoniak): Write XOR and check for a single road on this node.

      if (node.isStartNode()) {

      } else {
        std::cout << "END NODE" << std::endl;
        Road& oldRoad = (road.direction == Direction::N ? *(node.N) : *(node.W));
        // oldRoad.length += current.length - 1;
        oldRoad.length = 10;
        current = oldRoad;
        current.length = 100;

        deleteNode(nodesCopy, node);
        roadsCopy.pop_back();
      }
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
  const glm::ivec2 endIntersectionPos = current.getEnd() - glm::ivec2(1, 1) * (current.getType().width - 1);
  if (hasNodeAt(endIntersectionPos) && getNodeAt(endIntersectionPos).isIntersection()) {
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

  } else if (hasRoadAt(roads, current.getEnd())) {
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
}*/

const std::vector<Road>& RoadGraph::getRoads() const {
  return roads;
}

const std::vector<RoadGraph::Node>& RoadGraph::getNodes() const {
  return nodes;
}

void RoadGraph::describe() const {
  std::cout << "ROADS: " << roads.size() << std::endl;
  for (const data::Road& road : roads) {
    std::cout << " " << &road << " " << road.describe() << std::endl;
  }
  std::cout << "NODES: " << nodes.size() << std::endl;
  for (const Node& node : nodes) {
    std::cout << " pos: " << node.position.getGlobal().x << " " << node.position.getGlobal().y << std::endl;
  }
}

bool RoadGraph::hasRoadAt(const glm::ivec2 global) const {
  for (const data::Road& road : roads) {
    const glm::ivec2 b2 = road.position.getGlobal();
    const glm::ivec2 b1 = road.getEnd();
    if (checkRectIntersection(global, global, b1, b2)) {
      if (road.position.getGlobal() == roads.back().position.getGlobal()) {
        continue;
      }
      return true;
    }
  }
  return false;
}

bool RoadGraph::hasRoadAt(const glm::ivec2 global, const Road& toIgnore) const {
  for (const data::Road& road : roads) {
    const glm::ivec2 b2 = road.position.getGlobal();
    const glm::ivec2 b1 = road.getEnd();
    if (checkRectIntersection(global, global, b1, b2)) {
      if (road.position.getGlobal() == toIgnore.position.getGlobal()) {
        continue;
      }
      return true;
    }
  }
  return false;
}

Road& RoadGraph::getRoadAt(const glm::ivec2 global) {
  for (data::Road& road : roads) {
    const glm::ivec2 b2 = road.position.getGlobal();
    const glm::ivec2 b1 = road.getEnd();
    if (checkRectIntersection(global, global, b1, b2)) {
      return road;
    }
  }
  throw std::invalid_argument("Road does not exist");
}

Road& RoadGraph::getRoadAt(const glm::ivec2 global, const Road& toIgnore) {
  for (data::Road& road : roads) {
    const glm::ivec2 b2 = road.position.getGlobal();
    const glm::ivec2 b1 = road.getEnd();
    if (checkRectIntersection(global, global, b1, b2)) {
      if (road.position.getGlobal() == toIgnore.position.getGlobal()) {
        continue;
      }
      return road;
    }
  }
  throw std::invalid_argument("Road does not exist");
}

bool RoadGraph::hasNodeAt(const glm::ivec2 global) const {
  for (const Node& node : nodes) {
    const glm::ivec2 b2 = node.position.getGlobal();
    const glm::ivec2 b1 = node.position.getGlobal() + node.size - glm::ivec2(1, 1);
    if (checkRectIntersection(global, global, b1, b2)) {
      return true;
    }
  }
  return false;
}

RoadGraph::Node& RoadGraph::getNodeAt(const glm::ivec2 global) {
  for (Node& node : nodes) {
    const glm::ivec2 b2 = node.position.getGlobal();
    const glm::ivec2 b1 = node.position.getGlobal() + node.size - glm::ivec2(1, 1);
    if (checkRectIntersection(global, global, b1, b2)) {
      return node;
    }
  }
  throw std::invalid_argument("Node does not exist at (" + std::to_string(global.x) + ", " + std::to_string(global.y) + ")");
}

std::vector<RoadGraph::Node> RoadGraph::getNodesCopy(std::vector<Road>& roadsCopy) const {
  auto result = nodes;
  for (size_t i = 0; i < nodes.size(); i++) {
    result[i].N = nodes[i].N ? roadsCopy.data() + (nodes[i].N - roads.data()) : nullptr;
    result[i].S = nodes[i].S ? roadsCopy.data() + (nodes[i].S - roads.data()) : nullptr;
    result[i].W = nodes[i].W ? roadsCopy.data() + (nodes[i].W - roads.data()) : nullptr;
    result[i].E = nodes[i].E ? roadsCopy.data() + (nodes[i].E - roads.data()) : nullptr;
  }
  return result;
}

RoadGraph::Node& RoadGraph::divideRoadAt(const glm::ivec2 global, const Road& toIgnore) {
  Road& oldRoad = getRoadAt(global, toIgnore);
  std::cout << "divideRoadAt (" << global.x << " " << global.y << ") " << &oldRoad << std::endl;

  // Special: Handle division at start
  {
    const glm::ivec2& desired = oldRoad.position.getGlobal();
    if (global.x == desired.x && global.y == desired.y) {
      std::cout << "divideRoadAt special start" << std::endl;
      Node& node = getNodeAt(global);
      node.size = glm::ivec2(1, 1) * oldRoad.getType().width;
      return node;
    }
  }

  // Special: Handle division at end
  {
    const glm::ivec2& desired = oldRoad.getEnd() - glm::ivec2(1, 1) * (oldRoad.getType().width - 1);
    if (global.x == desired.x && global.y == desired.y) {
      std::cout << "divideRoadAt special end" << std::endl;
      const glm::ivec2 nodePos =
          oldRoad.getEnd() -
          (Direction::N == oldRoad.direction ? glm::ivec2(1, 0) : glm::ivec2(0, 1)) * (oldRoad.getType().width - 1);
      Node& node = getNodeAt(nodePos);
      node.position.setGlobal(global);
      node.size = glm::ivec2(1, 1) * oldRoad.getType().width;
      return node;
    }
  }

  // Create extension road
  roads.push_back(Road());
  Road& newRoad = roads.back();
  newRoad.setType(RoadTypes.Standard);
  newRoad.position.setGlobal(global);
  newRoad.direction = oldRoad.direction;
  if (Direction::W == newRoad.direction) {
    newRoad.length = oldRoad.getEnd().x - global.x + 1;
  } else {
    newRoad.length = oldRoad.getEnd().y - global.y + 1;
  }

  describe();

  // Shorten previous road
  oldRoad.length = oldRoad.length - newRoad.length + oldRoad.getType().width;

  // Pin new road at the end of old road
  Node& endNode = getNodeAt(newRoad.getEnd());
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

void RoadGraph::deleteNode(std::vector<Node>& nodes, Node& node) {
  if (nodes.back().position.getGlobal() == node.position.getGlobal()) {
    nodes.pop_back();
    return;
  }

  node = nodes.back();
  nodes.pop_back();
}

template <typename T>
bool RoadGraph::checkRectIntersection(glm::tvec2<T> a1, glm::tvec2<T> a2, glm::tvec2<T> b1, glm::tvec2<T> b2) const {
  return !(a1.y < b2.y || a2.y > b1.y || a1.x < b2.x || a2.x > b1.x);
}

bool RoadGraph::checkIntersection(const data::Road& a, const data::Road& b) const {
  const glm::ivec2 a2 = a.position.getGlobal();
  const glm::ivec2 a1 = a.getEnd();
  const glm::ivec2 b2 = b.position.getGlobal();
  const glm::ivec2 b1 = b.getEnd();
  return checkRectIntersection(a1, a2, b1, b2);
}
}
