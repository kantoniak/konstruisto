#ifndef WORLD_GEOMETRY_HPP
#define WORLD_GEOMETRY_HPP

#include <algorithm>

#include "../data/Chunk.hpp"
#include "../engine/Engine.hpp"
#include "Camera.hpp"
#include "World.hpp"

namespace world {

class Geometry {

public:
  void init(engine::Engine& engine, World& world);

  /**
   * @param entryPoint ray position in near field, [-1, 1]x[-1, 1]
   */
  bool hitGround(glm::vec2 entryPoint, glm::vec3& hit);
  bool hitField(glm::vec2 entryPoint, glm::ivec2& hit);

  glm::ivec2 pointToField(glm::vec3 point) const;
  glm::ivec2 fieldToChunk(glm::ivec2 field) const;

  template <typename T>
  bool checkRectIntersection(glm::tvec2<T> a1, glm::tvec2<T> a2, glm::tvec2<T> b1, glm::tvec2<T> b2) const;

  // TODO(kantoniak): Return structure with object IDs by type
  bool checkCollisions(const data::buildings::Building& building) const;
  bool checkCollisions(const data::Road& road) const;

  std::vector<data::buildings::Building> getBuildings(const glm::ivec2 from, const glm::ivec2 to) const;

  std::vector<data::Road> splitRoadByChunks(const data::Road& road) const;

protected:
  World* world;
  engine::Engine* engine;

  World& getWorld() const;
  engine::Engine& getEngine() const;

  bool checkCollisions(const data::Road& road, const data::Road& other) const;
  bool checkIntersection(const data::Road& a, const data::Road& b) const;

  const glm::ivec2 getEnd(const data::buildings::Building& building) const;
  const glm::ivec2 getEnd(const data::Road& road) const;
  const glm::ivec2 getLocalEnd(const data::Road& road) const;
};
}

#endif
