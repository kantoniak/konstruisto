#ifndef STATES_TESTSTATE_HPP
#define STATES_TESTSTATE_HPP

#include <chrono>
#include <iostream>

#include "../engine/Engine.hpp"
#include "../engine/GameState.hpp"
#include "../rendering/Renderer.hpp"
#include "../rendering/WindowHandler.hpp"

namespace states {

class TestState : public engine::GameState {

public:
  TestState(engine::Engine& engine) : GameState(engine), renderer(engine) {
    this->suspended = false;
  };
  virtual ~TestState(){};

  virtual void init() {
    if (!engine.getWindowHandler().createMainWindow() || !renderer.init()) {
      engine.stop();
      return;
    }
  };

  virtual void cleanup() {
    renderer.cleanup();
  }

  virtual void update(std::chrono::milliseconds delta) {
    engine.getLogger().debug("UPDATE %d ms", delta.count());
  };

  virtual void render() {
    renderer.renderWorld();
  };

private:
  rendering::Renderer renderer;
};
}

#endif
