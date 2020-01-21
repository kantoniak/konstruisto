#ifndef WORLD_WORLD_HPP
#define WORLD_WORLD_HPP

#include "../geometry/CollisionSpace.hpp"
#include "Camera.hpp"
#include "Map.hpp"
#include "Timer.hpp"

namespace world {

class World {

public:
  void init();
  void cleanup();

  void update(std::chrono::milliseconds delta);

  Camera& getCamera();
  Map& getMap();
  Timer& getTimer();
  geometry::CollisionSpace& get_collision_space() noexcept;

protected:
  Camera camera;
  Map map;
  geometry::CollisionSpace collision_space;
  Timer timer;
};
}

#endif
