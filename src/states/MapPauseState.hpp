#ifndef STATES_MAPPAUSESTATE_HPP
#define STATES_MAPPAUSESTATE_HPP

#include <chrono>

#include "../data/City.hpp"
#include "../engine/Engine.hpp"
#include "../engine/GameState.hpp"
#include "../input/WindowHandler.hpp"
#include "../rendering/WorldRenderer.hpp"
#include "../world/World.hpp"

namespace states {

class MapPauseState : public engine::GameState {

public:
  MapPauseState(engine::Engine& engine, world::World& world, rendering::WorldRenderer& worldRenderer);

  void init() override;

  void update(std::chrono::milliseconds delta) override;
  void render() override;

  void onKey(int key, int scancode, int action, int mods) override;
  void onMouseButton(int button, int action, int mods) override;
  void onScroll(double xoffset, double yoffset) override;
  void onWindowFocusChange(int focused) override;
  void onWindowResize(int width, int height) override;

private:
  world::World& world;
  rendering::WorldRenderer& worldRenderer;
};
}

#endif
