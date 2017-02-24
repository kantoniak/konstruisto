#include "Renderer.hpp"

#include <GL/glew.h>

#include "WindowHandler.hpp"

namespace rendering {
Renderer::Renderer(engine::Engine& engine) : engine(engine){};

bool Renderer::init() {
  return true;
}

void Renderer::cleanup() {
}

void Renderer::renderWorld() {
  glClearColor(0.349f, 0.624f, 0.820f, 1.f);
  glClear(GL_COLOR_BUFFER_BIT);

  glfwSwapBuffers(&engine.getWindowHandler().getWindow());
}
}