#ifndef CALLBACKS_HPP
#define CALLBACKS_HPP

#include "../rendering/WindowHandler.hpp"

namespace callbacks {

void onWindowClose(GLFWwindow* window) {
  rendering::WindowHandler* windowHandler = (rendering::WindowHandler*)glfwGetWindowUserPointer(window);
  windowHandler->onWindowClose();
}
}

#endif
