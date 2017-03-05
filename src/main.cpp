#include <iostream>

#include "engine/Engine.hpp"
#include "engine/Logger.hpp"
#include "input/WindowHandler.hpp"
#include "states/MapState.hpp"

int main() {
  std::cout.sync_with_stdio(false);
  engine::Logger logger(std::chrono::high_resolution_clock::now(), std::cout);

  logger.info("%s %s", PROJECT_NAME, BUILD_DESC);
  logger.setLoggingLevel(engine::LoggingLevel::DEBUG);

  engine::Engine engine(logger);
  input::WindowHandler windowHandler(engine);
  engine.init(windowHandler);

  states::MapState mapState(engine);
  engine.changeState(mapState);

  while (engine.running() && !glfwWindowShouldClose(&windowHandler.getWindow())) {
    engine.getDebugInfo().onFrameStart();
    windowHandler.update();
    engine.tick(std::chrono::high_resolution_clock::now());
    engine.getDebugInfo().onFrameEnd();
  }

  engine.cleanup();
  windowHandler.cleanup();
  return 0;
}