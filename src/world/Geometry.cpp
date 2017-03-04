#include "Geometry.hpp"

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
  return field / (int)data::Chunk::SIDE_LENGTH;
}

// TODO(kantoniak): Geometry::checkRectIntersection as template for GLM vectors
bool Geometry::checkRectIntersection(glm::vec2 a1, glm::vec2 a2, glm::vec2 b1, glm::vec2 b2) const {
  return !(a1.y < b2.y || a2.y > b1.y || a1.x < b2.x || a2.x > b1.x);
}

bool Geometry::checkCollisions(data::buildings::Building& building) {
  const glm::vec2 a2 = glm::vec2(building.x, building.y);
  const glm::vec2 a1 = glm::vec2(building.x + building.width - 1, building.y + building.length - 1);
  for (data::Chunk* chunk : getWorld().getMap().getChunks()) {
    for (data::buildings::Building other : chunk->getResidentials()) {
      const glm::vec2 b2 = glm::vec2(other.x, other.y);
      const glm::vec2 b1 = glm::vec2(other.x + other.width - 1, other.y + other.length - 1);
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