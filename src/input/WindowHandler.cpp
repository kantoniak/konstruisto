#include "WindowHandler.hpp"

#include <cassert>

#include "callbacks.hpp"

namespace input {
WindowHandler::WindowHandler(engine::Engine& engine) : engine(engine){};

void WindowHandler::update() {
  glfwPollEvents();
}

void WindowHandler::cleanup() {
  glfwMakeContextCurrent(nullptr);
  glfwTerminate();
}

bool WindowHandler::createMainWindow() {
  if (!glfwInit()) {
    engine.getLogger().severe("GLFW init failed.");
    return false;
  }
  engine.getLogger().debug("GLFW initiated.");

  // Window
  GLFWmonitor* monitor = glfwGetPrimaryMonitor();
  const GLFWvidmode* mode = glfwGetVideoMode(monitor);
  glfwWindowHint(GLFW_RED_BITS, mode->redBits);
  glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
  glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
  glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

  glfwWindowHint(GLFW_SAMPLES, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  std::string windowTitle(PROJECT_NAME);
#ifdef DEBUG_CONFIG
  windowTitle += std::string(" ") + BUILD_DESC;
#endif

  if (engine.getSettings().input.fullscreen) {
    viewportSize = glm::vec2(mode->width, mode->height);
    window = glfwCreateWindow(viewportSize.x, viewportSize.y, windowTitle.c_str(), monitor, nullptr);
  } else {
    viewportSize = glm::vec2(engine.getSettings().input.windowWidth, engine.getSettings().input.windowHeight);
    window = glfwCreateWindow(viewportSize.x, viewportSize.y, windowTitle.c_str(), nullptr, nullptr);
  }
  glViewport(0, 0, viewportSize.x, viewportSize.y);
  glfwSetWindowUserPointer(window, this);

  if (!window) {
    engine.getLogger().severe("Window creation failed.");
    glfwTerminate();
    return false;
  }
  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);
  engine.getLogger().debug("Window created.");

  // GLEW
  glewExperimental = GL_TRUE;
  GLint glewInitResult = glewInit();
  if (glewInitResult != GLEW_OK) {
    engine.getLogger().severe("GLEW init failed with error #%d: %s", glewInitResult,
                              glewGetErrorString(glewInitResult));
    glfwTerminate();
    return false;
  }
  glGetError();
  engine.getLogger().debug("GLEW initiated.");

  engine.getLogger().info("OpenGL version: %s", glGetString(GL_VERSION));

  // Callbacks
  glfwSetWindowSizeCallback(window, callbacks::onWindowResize);
  glfwSetCursorPosCallback(window, callbacks::onMouseMove);
  glfwSetMouseButtonCallback(window, callbacks::onMouseButton);
  glfwSetKeyCallback(window, callbacks::onKey);
  glfwSetScrollCallback(window, callbacks::onScroll);
  glfwSetWindowFocusCallback(window, callbacks::onWindowFocusChange);

  return true;
}

GLFWwindow& WindowHandler::getWindow() {
  assert(window != nullptr);
  return *window;
}

float WindowHandler::getViewportRatio() {
  return viewportSize.x / viewportSize.y;
}

glm::vec2 WindowHandler::getViewportSize() {
  return viewportSize;
}

glm::vec2 WindowHandler::getMousePosition() {
  return mousePosition;
}

glm::vec2 WindowHandler::getMousePositionNormalized() {
  return (mousePosition / viewportSize * 2.f) - glm::vec2(1, 1);
}

void WindowHandler::onKey(int key, int scancode, int action, int mods) {
  engine.getCurrentState()->onKey(key, scancode, action, mods);
}

void WindowHandler::onMouseButton(int button, int action, int mods) {
  engine.getCurrentState()->onMouseButton(button, action, mods);
}

void WindowHandler::onMouseMove(double x, double y) {
  mousePosition = glm::vec2(x, y);
}

void WindowHandler::onScroll(double xoffset, double yoffset) {
  engine.getCurrentState()->onScroll(xoffset, yoffset);
}

void WindowHandler::onWindowFocusChange(int focused) {
  engine.getCurrentState()->onWindowFocusChange(focused);
}

void WindowHandler::onWindowResize(int width, int height) {
  if (width == 0 && height == 0) {
    return;
  }

  engine.getLogger().debug("Window resized to %dx%d", width, height);
  viewportSize.x = width;
  viewportSize.y = height;
  glViewport(0, 0, width, height);
  engine.getCurrentState()->onWindowResize(width, height);
}
}