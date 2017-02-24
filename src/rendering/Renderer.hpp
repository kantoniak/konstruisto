#ifndef RENDERING_RENDERER_HPP
#define RENDERING_RENDERER_HPP

#include "../engine/Engine.hpp"

namespace rendering {

class Renderer {

public:
  Renderer(engine::Engine& engine);

  bool init();
  void cleanup();

  void renderWorld();

protected:
  engine::Engine& engine;
};
}

#endif
