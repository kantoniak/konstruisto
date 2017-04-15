#include "Geometry.hpp"

#include "glm/gtx/string_cast.hpp"
#include <iostream>

namespace world {
void Geometry::init(engine::Engine& engine, World& world) {
  this->world = &world;
  this->engine = &engine;
}

/**
 * @param entryPoint ray position in near field, [-1, 1]x[-1, 1]
 */
bool Geometry::hitGround(glm::vec2 entryPoint, glm::vec3& hit) {
  Camera camera = getWorld().getCamera();
  const glm::vec3 cameraPos = camera.getPosition();
  const glm::vec3 ray = camera.getRay(entryPoint);
  hit = cameraPos - (cameraPos.y / ray.y) * ray;

  return true;
}

bool Geometry::hitField(glm::vec2 entryPoint, glm::ivec2& hit) {
  glm::vec3 ground;
  if (!hitGround(entryPoint, ground)) {
    return false;
  }
  hit = pointToField(ground);
  return true;
}

glm::ivec2 Geometry::pointToField(glm::vec3 point) const {
  return glm::ivec2(floor(point.x), floor(point.z));
}

glm::ivec2 Geometry::fieldToChunk(glm::ivec2 field) const {
  glm::vec2 chunk = glm::vec2(field) / static_cast<float>(data::Chunk::SIDE_LENGTH);
  return glm::ivec2(floor(chunk.x), floor(chunk.y));
}

template <typename T>
bool Geometry::checkRectIntersection(glm::tvec2<T> a1, glm::tvec2<T> a2, glm::tvec2<T> b1, glm::tvec2<T> b2) const {
  return !(a1.y < b2.y || a2.y > b1.y || a1.x < b2.x || a2.x > b1.x);
}

bool Geometry::checkCollisions(const data::buildings::Building& building) const {
  const glm::ivec2 a2 = glm::vec2(building.x, building.y);
  const glm::ivec2 a1 = getEnd(building);

  if (!getWorld().getMap().chunkExists(fieldToChunk(a1)) || !getWorld().getMap().chunkExists(fieldToChunk(a2))) {
    return true;
  }

  for (data::Chunk* chunk : getWorld().getMap().getChunks()) {
    // With roads
    for (data::Road road : chunk->getRoads()) {
      const glm::ivec2 b2 = road.position.getGlobal();
      const glm::ivec2 b1 = getEnd(road);
      if (checkRectIntersection(a1, a2, b1, b2)) {
        return true;
      }
    }

    // With buildings
    for (data::buildings::Building other : chunk->getResidentials()) {
      const glm::ivec2 b2 = glm::vec2(other.x, other.y);
      const glm::ivec2 b1 = getEnd(other);
      if (checkRectIntersection(a1, a2, b1, b2)) {
        return true;
      }
    }
  }
  return false;
}

// Works only when roads have same width
bool Geometry::checkCollisions(const data::Road& road) const {
  if (!getWorld().getMap().chunkExists(road.position.getChunk())) {
    return true;
  }

  const data::Chunk& chunk = getWorld().getMap().getChunk(road.position.getChunk());

  // With intersections
  // It just works, no idea why...

  // With roads
  for (const data::Road& other : chunk.getRoads()) {
    if (checkCollisions(road, other)) {
      return true;
    }
  }

  // Handle roads from neighbouring chunks
  if (data::Direction::W == road.direction && 0 == road.position.getLocal().x &&
      getWorld().getMap().chunkExists(road.position.getChunk() - glm::ivec2(1, 0))) {
    const data::Chunk& otherChunk = getWorld().getMap().getChunk(road.position.getChunk() - glm::ivec2(1, 0));
    for (const data::Road& other : otherChunk.getRoads()) {
      if (checkCollisions(road, other)) {
        return true;
      }
    }
  }
  if (data::Direction::N == road.direction && 0 == road.position.getLocal().y &&
      getWorld().getMap().chunkExists(road.position.getChunk() - glm::ivec2(0, 1))) {
    const data::Chunk& otherChunk = getWorld().getMap().getChunk(road.position.getChunk() - glm::ivec2(0, 1));
    for (const data::Road& other : otherChunk.getRoads()) {
      if (checkCollisions(road, other)) {
        return true;
      }
    }
  }

  // With buildings
  const glm::ivec2 a2 = road.position.getGlobal();
  const glm::ivec2 a1 = getEnd(road);
  for (const data::buildings::Building& other : chunk.getResidentials()) {
    const glm::ivec2 b2 = glm::vec2(other.x, other.y);
    const glm::ivec2 b1 = getEnd(other);
    if (checkRectIntersection(a1, a2, b1, b2)) {
      return true;
    }
  }

  return false;
}

std::vector<data::buildings::Building> Geometry::getBuildings(const glm::ivec2 from, const glm::ivec2 to) const {
  std::vector<data::buildings::Building> result;

  for (data::Chunk* chunk : getWorld().getMap().getChunks()) {
    for (data::buildings::Building building : chunk->getResidentials()) {
      const glm::ivec2 b2 = glm::vec2(building.x, building.y);
      const glm::ivec2 b1 = getEnd(building);
      if (checkRectIntersection(to, from, b1, b2)) {
        result.push_back(building);
      }
    }
  }

  return result;
}

std::vector<data::Road> Geometry::splitRoadByChunks(const data::Road& road) const {
  std::vector<data::Road> result;
  data::Road current = road;

  while (roadEndOutsideChunk(current)) {
    data::Road toInsert = current;
    if (data::Direction::N == toInsert.direction) {
      toInsert.length = data::Chunk::SIDE_LENGTH - toInsert.position.getLocal().y;
      current.position.setGlobal(current.position.getGlobal() + glm::ivec2(0, toInsert.length));
    } else {
      toInsert.length = data::Chunk::SIDE_LENGTH - toInsert.position.getLocal().x;
      current.position.setGlobal(current.position.getGlobal() + glm::ivec2(toInsert.length, 0));
    }
    current.length -= toInsert.length;
    result.push_back(toInsert);
  }
  result.push_back(current);

  return result;
}

World& Geometry::getWorld() const {
  return *world;
}

engine::Engine& Geometry::getEngine() const {
  return *engine;
}

bool Geometry::roadEndOutsideChunk(const data::Road& road) const {
  glm::ivec2 localEnd = getLocalEnd(road);
  return (int)data::Chunk::SIDE_LENGTH <= localEnd.x || (int)data::Chunk::SIDE_LENGTH <= localEnd.y;
}

bool Geometry::checkCollisions(const data::Road& road, const data::Road& other) const {
  if (!checkIntersection(road, other)) {
    return false;
  }

  // Same direction
  if (other.direction == road.direction) {
    if (road.direction == data::Direction::N && road.position.getGlobal().x != other.position.getGlobal().x) {
      return true;
    }
    if (road.direction == data::Direction::W && road.position.getGlobal().y != other.position.getGlobal().y) {
      return true;
    }
  }

  // Different direction
  if (other.direction != road.direction) {
    if (road.direction == data::Direction::N) {
      if (road.position.getGlobal().x < other.position.getGlobal().x || getEnd(other).x < getEnd(road).x) {
        return true;
      }
      if (other.position.getGlobal().y < road.position.getGlobal().y &&
          road.position.getGlobal().y <= getEnd(other).y) {
        return true;
      }
      if (other.position.getGlobal().y <= getEnd(road).y && getEnd(road).y < getEnd(other).y) {
        return true;
      }
    }
    if (road.direction == data::Direction::W) {
      if (road.position.getGlobal().y < other.position.getGlobal().y || getEnd(other).y < getEnd(road).y) {
        return true;
      }
      if (other.position.getGlobal().x < road.position.getGlobal().x &&
          road.position.getGlobal().x <= getEnd(other).x) {
        return true;
      }
      if (other.position.getGlobal().x <= getEnd(road).x && getEnd(road).x < getEnd(other).x) {
        return true;
      }
    }
  }

  return false;
}

bool Geometry::checkIntersection(const data::Road& a, const data::Road& b) const {
  const glm::ivec2 a2 = a.position.getGlobal();
  const glm::ivec2 a1 = getEnd(a);
  const glm::ivec2 b2 = b.position.getGlobal();
  const glm::ivec2 b1 = getEnd(b);
  return checkRectIntersection(a1, a2, b1, b2);
}

const glm::ivec2 Geometry::getEnd(const data::buildings::Building& building) const {
  return glm::ivec2(building.x + building.width - 1, building.y + building.length - 1);
}

const glm::ivec2 Geometry::getEnd(const data::Road& road) const {
  return road.position.getGlobal() +
         glm::ivec2((road.direction == data::Direction::W ? road.length : road.getType().width) - 1,
                    (road.direction == data::Direction::N ? road.length : road.getType().width) - 1);
}

const glm::ivec2 Geometry::getLocalEnd(const data::Road& road) const {
  return road.position.getLocal() +
         glm::ivec2((road.direction == data::Direction::W ? road.length : road.getType().width) - 1,
                    (road.direction == data::Direction::N ? road.length : road.getType().width) - 1);
}
}