#include "WindowHandler.hpp"

#include <cassert>

#include "../input/callbacks.hpp"

namespace rendering {
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
  const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
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

  window = glfwCreateWindow(800, 600, windowTitle.c_str(), nullptr, nullptr);
  glViewport(0, 0, 800, 600);

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
  engine.getLogger().debug("GLEW initiated.");

  // Callbacks
  glfwSetWindowSizeCallback(window, callbacks::onWindowResize);

  return true;
}

GLFWwindow& WindowHandler::getWindow() {
  assert(window != nullptr);
  return *window;
}

void WindowHandler::onWindowResize(int width, int height) {
  engine.getLogger().debug("Window resized to %dx%d", width, height);
  glViewport(0, 0, width, height);
}
}