#ifndef CALLBACKS_HPP
#define CALLBACKS_HPP

#include "../rendering/WindowHandler.hpp"

namespace callbacks {

void onMouseButton(GLFWwindow* window, int button, int action, int mods) {
  rendering::WindowHandler* windowHandler = (rendering::WindowHandler*)glfwGetWindowUserPointer(window);
  windowHandler->onMouseButton(button, action, mods);
}

void onMouseMove(GLFWwindow* window, double xpos, double ypos) {
  rendering::WindowHandler* windowHandler = (rendering::WindowHandler*)glfwGetWindowUserPointer(window);
  windowHandler->onMouseMove(xpos, ypos);
}

void onWindowResize(GLFWwindow* window, int width, int height) {
  rendering::WindowHandler* windowHandler = (rendering::WindowHandler*)glfwGetWindowUserPointer(window);
  windowHandler->onWindowResize(width, height);
}
}

#endif
