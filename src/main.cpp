#include <iostream>

#include "engine/Engine.hpp"
#include "engine/Logger.hpp"
#include "states/TestState.hpp"

int main() {
  std::cout.sync_with_stdio(false);
  engine::Logger logger(std::chrono::high_resolution_clock::now(), std::cout);

  logger.info("%s %s", PROJECT_NAME, BUILD_DESC);
#ifdef DEBUG_CONFIG
  logger.debug("It's debug.");
#endif

  engine::Engine engine;
  engine.init();

  states::TestState testState(logger);
  testState.init();
  engine.changeState(testState);

  while (engine.running() && engine.getDeltaSinceStart().count() < 2000) {
    engine.tick(std::chrono::high_resolution_clock::now());
  }

  testState.cleanup();
  engine.cleanup();

  return 0;
}