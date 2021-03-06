#ifndef CALLBACKS_HPP
#define CALLBACKS_HPP

#include <iostream>

#include "WindowHandler.hpp"

namespace callbacks {

input::WindowHandler* main_window_handler = nullptr;

// OpenGL debugging
void APIENTRY onOpenGLDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity,
                                  [[maybe_unused]] GLsizei length, const GLchar* message, const void* user_param) {
  auto* window_handler = (input::WindowHandler*)user_param;
  window_handler->onOpenGLDebugOutput(source, type, id, severity, message);
}

// GLFW error handling
void onGLFWError(int code, const char* description) {
  if (main_window_handler != nullptr) {
    main_window_handler->onGLFWError(code, description);
  } else {
    std::cout << "GLFW error #" << code << ": " << description << std::endl;
  }
}

void onKey(GLFWwindow* window, int key, int scancode, int action, int mods) {
  auto* windowHandler = (input::WindowHandler*)glfwGetWindowUserPointer(window);
  windowHandler->onKey(key, scancode, action, mods);
}

void onMouseButton(GLFWwindow* window, int button, int action, int mods) {
  auto* windowHandler = (input::WindowHandler*)glfwGetWindowUserPointer(window);
  windowHandler->onMouseButton(button, action, mods);
}

void onMouseMove(GLFWwindow* window, double xpos, double ypos) {
  auto* windowHandler = (input::WindowHandler*)glfwGetWindowUserPointer(window);
  windowHandler->onMouseMove(xpos, ypos);
}

void onScroll(GLFWwindow* window, double xoffset, double yoffset) {
  auto* windowHandler = (input::WindowHandler*)glfwGetWindowUserPointer(window);
  windowHandler->onScroll(xoffset, yoffset);
}

void onWindowResize(GLFWwindow* window, int width, int height) {
  auto* windowHandler = (input::WindowHandler*)glfwGetWindowUserPointer(window);
  windowHandler->onWindowResize(width, height);
}

void onWindowFocusChange(GLFWwindow* window, int focused) {
  auto* windowHandler = (input::WindowHandler*)glfwGetWindowUserPointer(window);
  windowHandler->onWindowFocusChange(focused);
}
}

#endif
