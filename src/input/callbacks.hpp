#ifndef CALLBACKS_HPP
#define CALLBACKS_HPP

#include "WindowHandler.hpp"

namespace callbacks {

void onKey(GLFWwindow* window, int key, int scancode, int action, int mods) {
  input::WindowHandler* windowHandler = (input::WindowHandler*)glfwGetWindowUserPointer(window);
  windowHandler->onKey(key, scancode, action, mods);
}

void onMouseButton(GLFWwindow* window, int button, int action, int mods) {
  input::WindowHandler* windowHandler = (input::WindowHandler*)glfwGetWindowUserPointer(window);
  windowHandler->onMouseButton(button, action, mods);
}

void onMouseMove(GLFWwindow* window, double xpos, double ypos) {
  input::WindowHandler* windowHandler = (input::WindowHandler*)glfwGetWindowUserPointer(window);
  windowHandler->onMouseMove(xpos, ypos);
}

void onScroll(GLFWwindow* window, double xoffset, double yoffset) {
  input::WindowHandler* windowHandler = (input::WindowHandler*)glfwGetWindowUserPointer(window);
  windowHandler->onScroll(xoffset, yoffset);
}

void onWindowResize(GLFWwindow* window, int width, int height) {
  input::WindowHandler* windowHandler = (input::WindowHandler*)glfwGetWindowUserPointer(window);
  windowHandler->onWindowResize(width, height);
}
}

#endif
