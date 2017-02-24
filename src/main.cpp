#include <iostream>

#include "engine/Engine.hpp"
#include "engine/Logger.hpp"
#include "rendering/WindowHandler.hpp"
#include "states/TestState.hpp"

int main() {
  std::cout.sync_with_stdio(false);
  engine::Logger logger(std::chrono::high_resolution_clock::now(), std::cout);

  logger.info("%s %s", PROJECT_NAME, BUILD_DESC);
  logger.setLoggingLevel(engine::LoggingLevel::DEBUG);

  engine::Engine engine(logger);
  rendering::WindowHandler windowHandler(engine);
  engine.init(windowHandler);

  states::TestState testState(engine);
  engine.changeState(testState);

  while (engine.running() && engine.getDeltaSinceStart().count() < 500) {
    engine.tick(std::chrono::high_resolution_clock::now());
  }

  engine.cleanup();
  return 0;
}