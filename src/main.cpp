#include <iostream>

#include "engine/Engine.hpp"
#include "engine/Logger.hpp"
#include "input/WindowHandler.hpp"
#include "rendering/UI.hpp"
#include "settings.hpp"
#include "states/MainMenuState.hpp"
#include "states/MapState.hpp"

// Returns value after prefix if prefix exists, else null
char* stripPrefix(const char* prefix, char* str) {
  int len = strlen(prefix);
  if (strncmp(prefix, str, len) != 0) {
    return nullptr;
  }
  return str + len;
}

void initSettings(engine::Logger& logger, settings& gameSettings, int argc, char** argv) {

  for (int i = 1; i < argc; i++) {
    // Width
    char* width = stripPrefix("--width=", argv[i]);
    if (nullptr != width) {
      gameSettings.input.windowWidth = atoi(width);
      continue;
    }

    // Height
    char* height = stripPrefix("--height=", argv[i]);
    if (nullptr != height) {
      gameSettings.input.windowHeight = atoi(height);
      continue;
    }

    // Fullscreen
    char* fullscreen = stripPrefix("--fullscreen", argv[i]);
    if (nullptr != fullscreen) {
      gameSettings.input.fullscreen = true;
      continue;
    }

    // Logging
    char* loggingLevel = stripPrefix("--loggingLevel=", argv[i]);
    if (nullptr != loggingLevel) {
      logger.setLoggingLevel(logger.getLevelByName(loggingLevel));
      continue;
    }

    logger.warn("Unsupported flag: %s", argv[i]);
  }
}

int main(int argc, char** argv) {
  std::cout.sync_with_stdio(false);
  engine::Logger logger(std::chrono::high_resolution_clock::now(), std::cout);

  logger.info("%s %s", PROJECT_NAME, BUILD_DESC);
  logger.setLoggingLevel(engine::LoggingLevel::DEBUG);

  settings gameSettings;
  initSettings(logger, gameSettings, argc, argv);

  engine::Engine engine(gameSettings, logger);
  input::WindowHandler windowHandler(engine);
  rendering::Renderer renderer(engine);
  rendering::UI ui(engine);

  if (!windowHandler.createMainWindow()) {
    engine.stop();
  } else if (!renderer.init()) {
    engine.stop();
  } else if (!ui.init()) {
    engine.stop();
  }

  engine.init(windowHandler, renderer, ui);

  states::MapState mapState(engine);
  states::MainMenuState menuState(engine, mapState);
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