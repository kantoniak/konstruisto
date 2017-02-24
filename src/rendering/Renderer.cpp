#include "Renderer.hpp"

namespace rendering {
Renderer::Renderer(engine::Engine& engine, Camera& camera) : engine(engine), camera(camera) {
}

bool Renderer::init() {

  this->camera = camera;

  clearColor = glm::vec3(89, 159, 209) / 255.f;

  // TODO(kantoniak): Handle compiler/linker failure
  GLuint vertexShader = ShaderManager::compileShader(GL_VERTEX_SHADER, "shaders/terrain.vs", engine.getLogger());
  GLuint fragmentShader = ShaderManager::compileShader(GL_FRAGMENT_SHADER, "shaders/terrain.fs", engine.getLogger());
  this->shaderProgram = ShaderManager::linkProgram(vertexShader, 0, fragmentShader, engine.getLogger());
  transformLoc = glGetUniformLocation(shaderProgram, "transform");
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  GLfloat vertices[] = {100, 0, -100, -100, 0, -100, 100, 0, 100, -100, 0, 100};

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  engine.getLogger().debug(glm::to_string(camera.getViewProjectionMatrix()));

  return true;
}

void Renderer::cleanup() {
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glUseProgram(0);
  glDeleteProgram(this->shaderProgram);
}

void Renderer::renderWorld() {
  glClearColor(clearColor.x, clearColor.y, clearColor.z, 1.f);
  glClear(GL_COLOR_BUFFER_BIT);

  glUseProgram(shaderProgram);

  glm::mat4 vp = camera.getViewProjectionMatrix();
  glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(vp));

  glBindVertexArray(VAO);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  glBindVertexArray(0);

  glfwSwapBuffers(&engine.getWindowHandler().getWindow());
}
}