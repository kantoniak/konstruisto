#ifndef ENGINE_GAMESTATE_HPP
#define ENGINE_GAMESTATE_HPP

#include <chrono>

namespace engine {

class Engine;

class GameState {

public:
  GameState(Engine& engine) : engine(engine){};
  virtual ~GameState(){};
  virtual void init(){};
  virtual void cleanup(){};

  void suspend() {
    suspended = true;
  };
  void unsuspend() {
    suspended = false;
  };
  bool isSuspended() {
    return suspended;
  };

  virtual void update(std::chrono::milliseconds delta) = 0;
  virtual void render() = 0;

protected:
  bool suspended;

  Engine& engine;

  GameState(const GameState&) = delete;
  GameState& operator=(const GameState&) = delete;
};
}

#endif
