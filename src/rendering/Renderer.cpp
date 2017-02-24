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

  GLfloat vertices[] = {50, 0, -50, -50, 0, -50, 50, 0, 50, -50, 0, 50};

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  // TODO(kantoniak): Load from file
  // TODO(kantoniak): Add option to turn off the infinite grid
  unsigned short size = 64;
  unsigned char* pixels = new unsigned char[size * size * 4];

  for (unsigned short x = 0; x < size; x++) {
    for (unsigned short y = 0; y < size; y++) {
      pixels[4 * (size * y + x) + 0] = 159;
      pixels[4 * (size * y + x) + 1] = 164;
      pixels[4 * (size * y + x) + 2] = 81;
      pixels[4 * (size * y + x) + 3] = 255;

      if (x == size - 1 || y == size - 1 || x == 0 || y == 0) {
        pixels[4 * (size * y + x) + 0] = 159 * 0.8f;
        pixels[4 * (size * y + x) + 1] = 164 * 0.8f;
        pixels[4 * (size * y + x) + 2] = 81 * 0.8f;
        pixels[4 * (size * y + x) + 3] = 255;
      }
    }
  }

  // Texture of the ground
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size, size, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
  glGenerateMipmap(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, 0);

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

  glBindTexture(GL_TEXTURE_2D, texture);
  glUseProgram(shaderProgram);

  glm::mat4 vp = camera.getViewProjectionMatrix();
  glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(vp));

  glBindVertexArray(VAO);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  glBindVertexArray(0);

  glBindTexture(GL_TEXTURE_2D, 0);

  glfwSwapBuffers(&engine.getWindowHandler().getWindow());
}
}