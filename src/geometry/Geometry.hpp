#ifndef GEOMETRY_GEOMETRY_HPP
#define GEOMETRY_GEOMETRY_HPP

#include <algorithm>
#include <random>

#include "../engine/Engine.hpp"
#include "../world/Camera.hpp"
#include "../world/World.hpp"

namespace geometry {

class Geometry {

public:
  Geometry();
  void init(engine::Engine& engine, world::World& world);

  /**
   * @param entryPoint ray position in near field, [-1, 1]x[-1, 1]
   */
  bool hitGround(glm::vec2 entryPoint, glm::vec3& hit);
  bool hitField(glm::vec2 entryPoint, glm::ivec2& hit);

  [[nodiscard]] glm::ivec2 pointToField(glm::vec3 point) const;

  std::vector<glm::vec2> distribute_in_circle(size_t point_count, float radius, float normal_cutoff) noexcept;

protected:
  world::World* world;
  engine::Engine* engine;

  std::mt19937 generator;
  std::uniform_real_distribution<float> uniform_distribution;
  std::normal_distribution<float> normal_distribution;

  [[nodiscard]] world::World& getWorld() const;
  [[nodiscard]] engine::Engine& getEngine() const;
};
}

#endif
