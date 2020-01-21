#include "Geometry.hpp"

#include "glm/gtx/string_cast.hpp"
#include <iostream>

namespace geometry {
Geometry::Geometry()
    : generator(std::chrono::high_resolution_clock::now().time_since_epoch().count()), uniform_distribution(0.f, 1.f),
      normal_distribution(0.f, 1.f) {
}

void Geometry::init(engine::Engine& engine, world::World& world) {
  this->world = &world;
  this->engine = &engine;
}

/**
 * @param entryPoint ray position in near field, [-1, 1]x[-1, 1]
 */
bool Geometry::hitGround(glm::vec2 entryPoint, glm::vec3& hit) {
  world::Camera camera = getWorld().getCamera();
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

template <typename T>
bool Geometry::checkRectIntersection(glm::tvec2<T> a1, glm::tvec2<T> a2, glm::tvec2<T> b1, glm::tvec2<T> b2) const {
  return !(a1.y < b2.y || a2.y > b1.y || a1.x < b2.x || a2.x > b1.x);
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

std::vector<glm::vec2> Geometry::distribute_in_circle(size_t point_count, float radius, float normal_cutoff) noexcept {
  std::vector<glm::vec2> result;
  result.reserve(point_count);

  for (size_t i = 0; i < point_count; i++) {
    const float a = uniform_distribution(generator) * 2.f * M_PI;
    const float radius_base = abs(normal_distribution(generator)) / normal_cutoff;

    if (radius_base > 1.f) {
      continue;
    }

    float r = radius * sqrt(abs(radius_base));
    float x = r * cos(a);
    float y = r * sin(a);
    result.emplace_back(glm::vec2(x, y));
  }

  return result;
}

world::World& Geometry::getWorld() const {
  return *world;
}

engine::Engine& Geometry::getEngine() const {
  return *engine;
}

const glm::ivec2 Geometry::getEnd(const data::buildings::Building& building) const {
  return glm::ivec2(building.x + building.width - 1, building.y + building.length - 1);
}
}