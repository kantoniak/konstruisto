#ifndef WORLD_WORLD_HPP
#define WORLD_WORLD_HPP

#include "Camera.hpp"

namespace world {

class World {

public:
  void init();

  Camera& getCamera();

protected:
  Camera camera;
};
}

#endif
