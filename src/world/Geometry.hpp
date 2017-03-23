#ifndef WORLD_GEOMETRY_HPP
#define WORLD_GEOMETRY_HPP

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

  glm::ivec2 pointToField(glm::vec3 point);
  glm::ivec2 fieldToChunk(glm::ivec2 field);

  template <typename T>
  bool checkRectIntersection(glm::tvec2<T> a1, glm::tvec2<T> a2, glm::tvec2<T> b1, glm::tvec2<T> b2) const;

  // TODO(kantoniak): Return structure with object IDs by type
  bool checkCollisions(data::buildings::Building& building);
  bool checkCollisions(data::roads::Road& road);

protected:
  World* world;
  engine::Engine* engine;

  World& getWorld();
  engine::Engine& getEngine();

  const glm::ivec2 getEnd(data::buildings::Building& building) const;
  const glm::ivec2 getEnd(data::roads::Road& road) const;
};
}

#endif
