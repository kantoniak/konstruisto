#ifndef ENGINE_ENGINE_HPP
#define ENGINE_ENGINE_HPP

#include <queue>
#include <stack>
#include <vector>

#include "GameState.hpp"

namespace engine {

class Engine {

public:
  virtual ~Engine();

  virtual bool init();
  virtual void cleanup();

  void changeState(GameState& state);
  void pushState(GameState& state);
  void popState();

  GameState& getPreviousState();

  void tick(std::chrono::time_point<std::chrono::high_resolution_clock> now);
  std::chrono::milliseconds getDeltaSinceStart();

  bool running() const {
    return true;
  };

protected:
  std::vector<GameState*> states;
  std::chrono::time_point<std::chrono::high_resolution_clock> start;
  std::chrono::time_point<std::chrono::high_resolution_clock> current;

  virtual void update(std::chrono::milliseconds delta);
  virtual void render();
};
}

#endif
