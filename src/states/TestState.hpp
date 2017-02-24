#ifndef STATES_TESTSTATE_HPP
#define STATES_TESTSTATE_HPP

#include <chrono>
#include <iostream>

#include "../engine/GameState.hpp"
#include "../engine/Logger.hpp"

namespace states {

class TestState : public engine::GameState {

public:
  TestState(engine::Logger& logger) : logger(logger) {
    this->suspended = false;
  };
  virtual ~TestState(){};

  virtual void update(std::chrono::milliseconds delta) {
    logger.debug("UPDATE %d ms", delta.count());
  };

  virtual void render(){};

private:
  engine::Logger& logger;
};
}

#endif
