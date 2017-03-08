#ifndef STATES_MAPPAUSESTATE_HPP
#define STATES_MAPPAUSESTATE_HPP

#include <chrono>

#include "../data/City.hpp"
#include "../engine/Engine.hpp"
#include "../engine/GameState.hpp"
#include "../input/WindowHandler.hpp"
#include "../rendering/Renderer.hpp"
#include "../world/World.hpp"

namespace states {

class MapPauseState : public engine::GameState {

public:
  MapPauseState(engine::Engine& engine, world::World& world, rendering::Renderer& renderer);

  void init();

  void update(std::chrono::milliseconds delta);
  void render();

  void onKey(int key, int scancode, int action, int mods);
  void onMouseButton(int button, int action, int mods);
  void onScroll(double xoffset, double yoffset);
  void onWindowResize(int width, int height);

private:
  world::World& world;
  rendering::Renderer& renderer;
  NVGcontext* nvgContext;
};
}

#endif
