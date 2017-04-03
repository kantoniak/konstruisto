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

glm::ivec2 Geometry::pointToField(glm::vec3 point) {
  return glm::ivec2(floor(point.x), floor(point.z));
}

glm::ivec2 Geometry::fieldToChunk(glm::ivec2 field) {
  glm::vec2 chunk = glm::vec2(field) / static_cast<float>(data::Chunk::SIDE_LENGTH);
  return glm::ivec2(floor(chunk.x), floor(chunk.y));
}

template <typename T>
bool Geometry::checkRectIntersection(glm::tvec2<T> a1, glm::tvec2<T> a2, glm::tvec2<T> b1, glm::tvec2<T> b2) const {
  return !(a1.y < b2.y || a2.y > b1.y || a1.x < b2.x || a2.x > b1.x);
}

bool Geometry::checkCollisions(data::buildings::Building& building) {
  const glm::ivec2 a2 = glm::vec2(building.x, building.y);
  const glm::ivec2 a1 = getEnd(building);

  if (!getWorld().getMap().chunkExists(fieldToChunk(a1)) || !getWorld().getMap().chunkExists(fieldToChunk(a2))) {
    return true;
  }

  for (data::Chunk* chunk : getWorld().getMap().getChunks()) {
    // With roads
    for (data::roads::Road road : chunk->getRoads()) {
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

bool Geometry::checkCollisions(data::roads::Road& road) {
  const glm::ivec2 a2 = road.position.getGlobal();
  const glm::ivec2 a1 = getEnd(road);

  if (!getWorld().getMap().chunkExists(fieldToChunk(a1)) || !getWorld().getMap().chunkExists(fieldToChunk(a2))) {
    return true;
  }

  for (data::Chunk* chunk : getWorld().getMap().getChunks()) {
    // With roads
    // TODO(kantoniak): Handle invalid intersections
    for (data::roads::Road other : chunk->getRoads()) {
      if (other.direction != road.direction) {
        continue;
      }
      const glm::ivec2 b2 = other.position.getGlobal();
      const glm::ivec2 b1 = getEnd(other);
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

World& Geometry::getWorld() {
  return *world;
}

engine::Engine& Geometry::getEngine() {
  return *engine;
}

const glm::ivec2 Geometry::getEnd(data::buildings::Building& building) const {
  return glm::ivec2(building.x + building.width - 1, building.y + building.length - 1);
}

const glm::ivec2 Geometry::getEnd(data::roads::Road& road) const {
  return road.position.getGlobal() +
         glm::ivec2(
             (road.direction == data::roads::Direction::W ? road.length : data::roads::getDefinition(road).width) - 1,
             (road.direction == data::roads::Direction::N ? road.length : data::roads::getDefinition(road).width) - 1);
}
}