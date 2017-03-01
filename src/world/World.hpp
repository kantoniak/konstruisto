#ifndef WORLD_WORLD_HPP
#define WORLD_WORLD_HPP

#include "Camera.hpp"
#include "Map.hpp"

namespace world {

class World {

public:
  void init();
  void cleanup();

  Camera& getCamera();
  Map& getMap();

protected:
  Camera camera;
  Map map;
};
}

#endif
