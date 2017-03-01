#ifndef STATES_TESTSTATE_HPP
#define STATES_TESTSTATE_HPP

#include <chrono>
#include <cmath>
#include <iostream>

#include "../data/CameraState.hpp"
#include "../engine/Engine.hpp"
#include "../engine/GameState.hpp"
#include "../input/Selection.hpp"
#include "../input/WindowHandler.hpp"
#include "../rendering/Renderer.hpp"
#include "../world/Camera.hpp"

namespace states {

class TestState : public engine::GameState {

public:
  TestState(engine::Engine& engine);

  void init();
  void cleanup();

  void update(std::chrono::milliseconds delta);
  void render();

  void onMouseButton(int button, int action, int mods);

private:
  rendering::Renderer renderer;
  world::Camera camera;
  input::Selection selection;
};
}

#endif
