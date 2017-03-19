#include "Renderer.hpp"

#include "../data/Chunk.hpp"

namespace rendering {
Renderer::Renderer(engine::Engine& engine) : engine(engine) {
  clearColor = glm::vec3(89, 159, 209) / 255.f;
}

bool Renderer::init() {
  glClearColor(clearColor.x, clearColor.y, clearColor.z, 1.f);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);

  return true;
}

void Renderer::cleanup() {
}

void Renderer::prepareFrame() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void Renderer::sendFrame() {
  glfwSwapBuffers(&engine.getWindowHandler().getWindow());
}

GLuint Renderer::compileShader(GLenum shaderType, std::string filename) {
  return ShaderManager::compileShader(shaderType, filename, engine.getLogger());
}
}