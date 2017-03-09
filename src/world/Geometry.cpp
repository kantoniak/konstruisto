#include "Geometry.hpp"

#include <iostream>
#include "glm/gtx/string_cast.hpp"

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
  glm::vec2 chunk = glm::vec2(field) / (float)data::Chunk::SIDE_LENGTH;
  return glm::ivec2(floor(chunk.x), floor(chunk.y));
}

template <typename T>
bool Geometry::checkRectIntersection(glm::tvec2<T> a1, glm::tvec2<T> a2, glm::tvec2<T> b1, glm::tvec2<T> b2) const {
  return !(a1.y < b2.y || a2.y > b1.y || a1.x < b2.x || a2.x > b1.x);
}

bool Geometry::checkCollisions(data::buildings::Building& building) {
  const glm::ivec2 a2 = glm::vec2(building.x, building.y);
  const glm::ivec2 a1 = glm::vec2(building.x + building.width - 1, building.y + building.length - 1);

  if (!getWorld().getMap().chunkExists(fieldToChunk(a1)) || !getWorld().getMap().chunkExists(fieldToChunk(a2))) {
    return true;
  }

  for (data::Chunk* chunk : getWorld().getMap().getChunks()) {
    for (data::buildings::Building other : chunk->getResidentials()) {
      const glm::ivec2 b2 = glm::vec2(other.x, other.y);
      const glm::ivec2 b1 = glm::vec2(other.x + other.width - 1, other.y + other.length - 1);
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
}