#ifndef STATES_TESTSTATE_HPP
#define STATES_TESTSTATE_HPP

#include <chrono>
#include <cmath>
#include <iostream>

#include "../data/CameraState.hpp"
#include "../engine/Engine.hpp"
#include "../engine/GameState.hpp"
#include "../rendering/Camera.hpp"
#include "../rendering/Renderer.hpp"
#include "../rendering/WindowHandler.hpp"

namespace states {

class TestState : public engine::GameState {

public:
  TestState(engine::Engine& engine);

  void init();
  void cleanup();

  void update(std::chrono::milliseconds delta);
  void render();

private:
  rendering::Renderer renderer;
  rendering::Camera camera;
};
}

#endif
