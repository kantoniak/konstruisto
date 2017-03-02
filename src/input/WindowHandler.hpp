#ifndef INPUT_WINDOWHANDLER_HPP
#define INPUT_WINDOWHANDLER_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "../engine/Engine.hpp"

namespace input {

class WindowHandler {

public:
  WindowHandler(engine::Engine& engine);

  void update();
  void cleanup();

  bool createMainWindow();
  GLFWwindow& getWindow();
  float getViewportRatio();
  glm::vec2 getViewportSize();

  glm::vec2 getMousePosition();

  // Event callbacks
  void onMouseButton(int button, int action, int mods);
  void onMouseMove(double x, double y);
  void onWindowResize(int width, int height);

protected:
  engine::Engine& engine;

  GLFWwindow* window = nullptr;

  glm::vec2 mousePosition;
  glm::vec2 viewportSize;
};
}

#endif
