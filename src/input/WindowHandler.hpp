#ifndef INPUT_WINDOWHANDLER_HPP
#define INPUT_WINDOWHANDLER_HPP

// Enforce correct load order
// clang-format off
#include <glad/gl.h>
#include <GLFW/glfw3.h>
// clang-format on

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

  // OpenGL callback
  void onOpenGLDebugOutput(uint32_t source, uint32_t type, uint32_t id, uint32_t severity, const char* message);
  // GLFW callback
  void onGLFWError(int code, const char* description);

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
