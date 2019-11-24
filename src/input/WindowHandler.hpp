#ifndef INPUT_WINDOWHANDLER_HPP
#define INPUT_WINDOWHANDLER_HPP

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <stb/stb_image.h>

#include "../engine/Engine.hpp"
#include "../settings.hpp"

namespace input {

class WindowHandler {

public:
  explicit WindowHandler(engine::Engine& engine);

  void update();
  void cleanup();

  bool createMainWindow();
  GLFWwindow& getWindow();
  float getViewportRatio();
  glm::vec2 getViewportSize();

  glm::vec2 getMousePosition();
  glm::vec2 getMousePositionNormalized();

  // Event callbacks
  void onKey(int key, int scancode, int action, int mods);
  void onMouseButton(int button, int action, int mods);
  void onMouseMove(double x, double y);
  void onScroll(double xoffset, double yoffset);
  void onWindowFocusChange(int focused);
  void onWindowResize(int width, int height);

protected:
  engine::Engine& engine;

  GLFWwindow* window = nullptr;

  glm::vec2 mousePosition;
  glm::vec2 viewportSize;
};
}

#endif
