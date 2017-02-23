#ifndef STATES_TESTSTATE_HPP
#define STATES_TESTSTATE_HPP

#include <chrono>
#include <iostream>

#include "../engine/GameState.hpp"

namespace states {

class TestState : public engine::GameState {

public:
  TestState(){};
  virtual ~TestState(){};

  virtual void update(std::chrono::milliseconds delta) {
    std::cout << "UPDATE " << delta.count() << " ms" << std::endl;
  };

  virtual void render(){};
};
}

#endif
