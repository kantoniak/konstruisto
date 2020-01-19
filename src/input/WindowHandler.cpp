#include "WindowHandler.hpp"

#include <cassert>

#include "../opengl/debug.hpp"
#include "callbacks.hpp"

namespace input {
WindowHandler::WindowHandler(engine::Engine& engine) : engine(engine){};

void WindowHandler::update() {
  glfwPollEvents();
}

void WindowHandler::cleanup() {
  glfwMakeContextCurrent(nullptr);
  glfwDestroyWindow(window);
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
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef _DEBUG
  // Add OpenGL debugging
  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#endif

  std::string windowTitle(PROJECT_NAME);
#ifdef _DEBUG
  windowTitle += std::string(" ") + BUILD_DESC;
#endif

  if (engine.getSettings().input.fullscreen) {
    viewportSize = glm::vec2(mode->width, mode->height);
    window = glfwCreateWindow(viewportSize.x, viewportSize.y, windowTitle.c_str(), monitor, nullptr);
  } else {
    viewportSize = glm::vec2(engine.getSettings().input.windowWidth, engine.getSettings().input.windowHeight);
    window = glfwCreateWindow(viewportSize.x, viewportSize.y, windowTitle.c_str(), nullptr, nullptr);
  }

  if (!window) {
    engine.getLogger().severe("Window creation failed.");
    glfwTerminate();
    return false;
  }

  glfwSetWindowUserPointer(window, this);
  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);
  engine.getLogger().debug("Window created.");

  // GLAD
  if (!gladLoadGL(glfwGetProcAddress)) {
    engine.getLogger().severe("GLAD init failed.");
    glfwTerminate();
    return false;
  }

  if (!GLAD_GL_VERSION_4_3) {
    engine.getLogger().severe("OpenGL 4.3 not loaded.");
    glfwTerminate();
    return false;
  }

  engine.getLogger().debug("GLAD initiated.");
  engine.getLogger().debug("OpenGL version: %s", glGetString(GL_VERSION));

  if (GLAD_GL_EXT_texture_filter_anisotropic) {
    engine.getLogger().debug("OpenGL: EXT_texture_filter_anisotropic support enabled.");
  }

  glViewport(0, 0, viewportSize.x, viewportSize.y);

#ifndef _WIN32
  // Windows will use embedded icon, set manually for other systems
  GLFWimage windowIcon;
  windowIcon.pixels =
      stbi_load("assets/konstruisto-icon-16.png", &windowIcon.width, &windowIcon.height, nullptr, STBI_rgb_alpha);
  if (nullptr == windowIcon.pixels) {
    engine.getLogger().warn("Could not load icon for window: %s", "assets/konstruisto-icon-16.png");
  } else {
    glfwSetWindowIcon(window, 1, &windowIcon);
    stbi_image_free(windowIcon.pixels);
  }
#endif

#ifdef _DEBUG

  // OpenGL debugging
  int32_t opengl_context_flags;
  glGetIntegerv(GL_CONTEXT_FLAGS, &opengl_context_flags);
  if (opengl_context_flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(callbacks::onOpenGLDebugOutput, this);

    // Enable high and medium severity messages only
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_HIGH, 0, nullptr, GL_TRUE);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_MEDIUM, 0, nullptr, GL_TRUE);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_LOW, 0, nullptr, GL_FALSE);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);
  } else {
    engine.getLogger().warn("OpenGL: GL_CONTEXT_FLAG_DEBUG_BIT disabled");
  }

#endif

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

void WindowHandler::onOpenGLDebugOutput(uint32_t source, uint32_t type, uint32_t id, uint32_t severity,
                                        const char* message) {
  using namespace opengl;
  std::string to_print = "OpenGL: Debug message #" + std::to_string(id) + " (";
  to_print += "source: " + gl_debug_source_to_string(source) + ", ";
  to_print += "type: " + gl_debug_type_to_string(type) + ", ";
  to_print += "severity: " + gl_debug_severity_to_string(severity) + "): ";
  to_print += message;
  engine.getLogger().debug(to_print);
}

void WindowHandler::onGLFWError(int code, const char* description) {
  std::string to_print = "GLFW error #";
  to_print += code;
  to_print += ": ";
  to_print += description;
  engine.getLogger().error(to_print);
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