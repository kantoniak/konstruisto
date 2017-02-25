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
  selectionLoc = glGetUniformLocation(shaderProgram, "selection");
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

  // Buildings
  GLfloat building[] = {// 8x2
                        0.1f, 0.0f, 0.1f, 0.1f, 1.0f, 0.1f, 0.9f, 0.0f, 0.1f, 0.9f, 1.0f, 0.1f,
                        0.9f, 0.0f, 0.9f, 0.9f, 1.0f, 0.9f, 0.1f, 0.0f, 0.9f, 0.1f, 1.0f, 0.9f,
                        0.1f, 0.0f, 0.1f, 0.1f, 1.0f, 0.1f, 0.1f, 0.0f, 0.1f, 0.1f, 1.0f, 0.1f, // Reset
                        0.9f, 1.0f, 0.1f, 0.1f, 1.0f, 0.1f, 0.9f, 1.0f, 0.9f, 0.1f, 1.0f, 0.9f};

  glm::vec3 positions[buildingsCount];
  for (unsigned int i = 0; i < buildingsCount; i++) {
    glm::vec3 position;
    position.x = rand() % (sideSize + 1);
    position.z = rand() % (sideSize + 1);
    position.y = rand() % (maxHeight) + 1;
    position -= glm::vec3(sideSize, 0, sideSize) / 2;
    positions[i] = position;
  }

  glGenVertexArrays(1, &buildingsVAO);
  glBindVertexArray(buildingsVAO);

  glGenBuffers(1, &buildingsVBO);
  glBindBuffer(GL_ARRAY_BUFFER, buildingsVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(building), building, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

  glGenBuffers(1, &buildingsInstanceVBO);
  glBindBuffer(GL_ARRAY_BUFFER, buildingsInstanceVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * buildingsCount, &positions[0], GL_STATIC_DRAW);

  glEnableVertexAttribArray(1);
  glBindBuffer(GL_ARRAY_BUFFER, buildingsInstanceVBO);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
  glVertexAttribDivisor(1, 1);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  GLuint buildingsVertexShader =
      ShaderManager::compileShader(GL_VERTEX_SHADER, "shaders/buildings.vs", engine.getLogger());
  GLuint buildingsGeomShader =
      ShaderManager::compileShader(GL_GEOMETRY_SHADER, "shaders/buildings.gs", engine.getLogger());
  GLuint buildingsFragmentShader =
      ShaderManager::compileShader(GL_FRAGMENT_SHADER, "shaders/buildings.fs", engine.getLogger());
  this->buildingsShaderProgram = ShaderManager::linkProgram(buildingsVertexShader, buildingsGeomShader,
                                                            buildingsFragmentShader, engine.getLogger());
  buildingsTransformLoc = glGetUniformLocation(buildingsShaderProgram, "transform");
  glDeleteShader(buildingsVertexShader);
  glDeleteShader(buildingsGeomShader);
  glDeleteShader(buildingsFragmentShader);

  // Others
  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);

  return true;
}

void Renderer::cleanup() {
  glUseProgram(0);

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteProgram(this->shaderProgram);

  glDeleteVertexArrays(1, &buildingsVAO);
  glDeleteBuffers(1, &buildingsVBO);
  glDeleteBuffers(1, &buildingsInstanceVBO);
  glDeleteProgram(this->buildingsShaderProgram);
}

void Renderer::renderWorld() {
  glClearColor(clearColor.x, clearColor.y, clearColor.z, 1.f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glm::mat4 vp = camera.getViewProjectionMatrix();

  // TODO(kantoniak): Move out
  // Selection
  glm::vec3 cameraPos = camera.getPosition();
  glm::vec3 ray = camera.getRay(engine.getWindowHandler().getMousePosition());
  glm::vec3 onPlane = cameraPos - (cameraPos.y / ray.y) * ray;
  if (onPlane.x < 0) {
    onPlane.x--;
  }
  if (onPlane.z < 0) {
    onPlane.z--;
  }

  // Terrain
  glUseProgram(shaderProgram);
  glBindVertexArray(VAO);
  glBindTexture(GL_TEXTURE_2D, texture);

  glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(vp));
  glUniform4i(selectionLoc, onPlane.x, onPlane.z, onPlane.x + 1, onPlane.z + 1);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

  glBindTexture(GL_TEXTURE_2D, 0);

  // Buildings
  glUseProgram(buildingsShaderProgram);
  glBindVertexArray(buildingsVAO);

  glUniformMatrix4fv(buildingsTransformLoc, 1, GL_FALSE, glm::value_ptr(vp));
  glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 16, buildingsCount);

  glBindVertexArray(0);

  glfwSwapBuffers(&engine.getWindowHandler().getWindow());
}
}