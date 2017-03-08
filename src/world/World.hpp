#ifndef WORLD_WORLD_HPP
#define WORLD_WORLD_HPP

#include "Camera.hpp"
#include "Map.hpp"
#include "Timer.hpp"

namespace world {

class World {

public:
  void init();
  void cleanup();

  Camera& getCamera();
  Map& getMap();
  Timer& getTimer();

protected:
  Camera camera;
  Map map;
  Timer timer;
};
}

#endif
