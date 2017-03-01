#ifndef CALLBACKS_HPP
#define CALLBACKS_HPP

#include "WindowHandler.hpp"

namespace callbacks {

void onMouseButton(GLFWwindow* window, int button, int action, int mods) {
  input::WindowHandler* windowHandler = (input::WindowHandler*)glfwGetWindowUserPointer(window);
  windowHandler->onMouseButton(button, action, mods);
}

void onMouseMove(GLFWwindow* window, double xpos, double ypos) {
  input::WindowHandler* windowHandler = (input::WindowHandler*)glfwGetWindowUserPointer(window);
  windowHandler->onMouseMove(xpos, ypos);
}

void onWindowResize(GLFWwindow* window, int width, int height) {
  input::WindowHandler* windowHandler = (input::WindowHandler*)glfwGetWindowUserPointer(window);
  windowHandler->onWindowResize(width, height);
}
}

#endif
