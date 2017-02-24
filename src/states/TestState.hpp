#ifndef STATES_TESTSTATE_HPP
#define STATES_TESTSTATE_HPP

#include <chrono>
#include <iostream>

#include "../engine/Engine.hpp"
#include "../engine/GameState.hpp"

namespace states {

class TestState : public engine::GameState {

public:
  TestState(engine::Engine& engine) : GameState(engine) {
    this->suspended = false;
  };
  virtual ~TestState(){};

  virtual void update(std::chrono::milliseconds delta) {
    engine.getLogger().debug("UPDATE %d ms", delta.count());
  };

  virtual void render(){};
};
}

#endif