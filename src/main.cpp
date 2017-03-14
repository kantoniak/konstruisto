#include <iostream>

#include "engine/Engine.hpp"
#include "engine/Logger.hpp"
#include "input/WindowHandler.hpp"
#include "rendering/UI.hpp"
#include "settings.hpp"
#include "states/MainMenuState.hpp"
#include "states/MapState.hpp"

int main() {
  std::cout.sync_with_stdio(false);
  engine::Logger logger(std::chrono::high_resolution_clock::now(), std::cout);

  logger.info("%s %s", PROJECT_NAME, BUILD_DESC);
  logger.setLoggingLevel(engine::LoggingLevel::DEBUG);

  settings gameSettings;
  engine::Engine engine(gameSettings, logger);
  input::WindowHandler windowHandler(engine);
  rendering::UI ui(engine);

  if (!windowHandler.createMainWindow()) {
    engine.stop();
  } else if (!ui.init()) {
    engine.stop();
  }

  engine.init(windowHandler, ui);

  states::MapState mapState(engine);
  states::MainMenuState menuState(engine, mapState.getRenderer(), mapState);
  engine.changeState(menuState);

  while (engine.running() && !glfwWindowShouldClose(&windowHandler.getWindow())) {
    engine.getDebugInfo().onFrameStart();
    windowHandler.update();
    engine.tick(std::chrono::high_resolution_clock::now());
    engine.getDebugInfo().onFrameEnd();
  }

  engine.cleanup();
  windowHandler.cleanup();
  ui.cleanup();
  return 0;
}