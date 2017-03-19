#ifndef ENGINE_ENGINE_HPP
#define ENGINE_ENGINE_HPP

#include <cstdlib>
#include <ctime>
#include <queue>
#include <stack>
#include <vector>

#include "DebugInfo.hpp"
#include "GameState.hpp"
#include "Logger.hpp"

struct settings;

namespace input {
class WindowHandler;
}

namespace rendering {
class Renderer;
class UI;
}

namespace engine {

class Engine {

public:
  Engine(settings& gameSettings, Logger& logger);
  virtual ~Engine();

  bool running();
  void stop();

  virtual bool init(input::WindowHandler& windowHandler, rendering::Renderer& renderer, rendering::UI& ui);
  virtual void cleanup();

  void changeState(GameState& state);
  void pushState(GameState& state);
  void popState();

  GameState* getCurrentState();
  GameState* getPreviousState();

  void tick(std::chrono::time_point<std::chrono::high_resolution_clock> now);
  std::chrono::milliseconds getDeltaSinceStart();

  settings& getSettings() const;
  Logger& getLogger() const;
  DebugInfo& getDebugInfo();
  input::WindowHandler& getWindowHandler() const;
  rendering::Renderer& getRenderer() const;
  rendering::UI& getUI() const;

protected:
  settings& gameSettings;
  bool isRunning;

  Logger& logger;
  DebugInfo debugInfo;
  input::WindowHandler* windowHandler;
  rendering::Renderer* renderer;
  rendering::UI* ui;

  std::vector<GameState*> states;
  std::chrono::time_point<std::chrono::high_resolution_clock> start;
  std::chrono::time_point<std::chrono::high_resolution_clock> current;

  virtual void update(std::chrono::milliseconds delta);
  virtual void render();
};
}

#endif
