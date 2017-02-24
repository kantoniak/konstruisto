#ifndef CALLBACKS_HPP
#define CALLBACKS_HPP

#include "../rendering/WindowHandler.hpp"

namespace callbacks {

void onWindowResize(GLFWwindow* window, int width, int height) {
  rendering::WindowHandler* windowHandler = (rendering::WindowHandler*)glfwGetWindowUserPointer(window);
  windowHandler->onWindowResize(width, height);
}
}

#endif
