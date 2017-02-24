#ifndef RENDERING_WINDOWHANDLER_HPP
#define RENDERING_WINDOWHANDLER_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../engine/Engine.hpp"

namespace rendering {

class WindowHandler {

public:
  WindowHandler(engine::Engine& engine);

  void update();
  void cleanup();

  bool createMainWindow();
  GLFWwindow& getWindow();
  float getViewportRatio();

  // Event callbacks
  void onWindowResize(int width, int height);

protected:
  engine::Engine& engine;

  GLFWwindow* window = nullptr;
};
}

#endif
