#ifndef ENGINE_ENGINE_HPP
#define ENGINE_ENGINE_HPP

#include <cstdlib>
#include <ctime>
#include <queue>
#include <stack>
#include <vector>

#include "GameState.hpp"
#include "Logger.hpp"

namespace rendering {
class WindowHandler;
}

namespace engine {

class Engine {

public:
  Engine(Logger& logger);
  virtual ~Engine();

  bool running();
  void stop();

  virtual bool init(rendering::WindowHandler& windowHandler);
  virtual void cleanup();

  void changeState(GameState& state);
  void pushState(GameState& state);
  void popState();

  GameState* getCurrentState();
  GameState* getPreviousState();

  void tick(std::chrono::time_point<std::chrono::high_resolution_clock> now);
  std::chrono::milliseconds getDeltaSinceStart();

  Logger& getLogger() const;
  rendering::WindowHandler& getWindowHandler() const;

protected:
  bool isRunning;

  Logger& logger;
  rendering::WindowHandler* windowHandler;

  std::vector<GameState*> states;
  std::chrono::time_point<std::chrono::high_resolution_clock> start;
  std::chrono::time_point<std::chrono::high_resolution_clock> current;

  virtual void update(std::chrono::milliseconds delta);
  virtual void render();
};
}

#endif
