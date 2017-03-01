#ifndef WORLD_GEOMETRY_HPP
#define WORLD_GEOMETRY_HPP

#include "Camera.hpp"
#include "World.hpp"

namespace world {

class Geometry {

public:
  void init(World& world);

  /**
   * @param entryPoint ray position in near field, [-1, 1]x[-1, 1]
   */
  bool hitGround(glm::vec2 entryPoint, glm::vec3& hit);
  bool hitField(glm::vec2 entryPoint, glm::ivec2& hit);

protected:
  World* world;

  World& getWorld();
};
}

#endif
