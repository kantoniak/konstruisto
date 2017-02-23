#include <iostream>

#include "engine/Engine.hpp"
#include "states/TestState.hpp"

int main() {

  // Test defines
  std::cout << "Hello, " << PROJECT_NAME << " " << BUILD_DESC << "!" << std::endl;
#ifdef DEBUG
  std::cout << "It's debug." << std::endl;
#endif

  engine::Engine engine;
  engine.init();

  states::TestState testState;
  testState.init();

  engine.changeState(testState);

  while (engine.running() && engine.getDeltaSinceStart().count() < 200) {
    engine.tick(std::chrono::high_resolution_clock::now());
  }

  testState.cleanup();
  engine.cleanup();

  return 0;
}