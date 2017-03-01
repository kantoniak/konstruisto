#include "Renderer.hpp"

#include "../data/Chunk.hpp"

namespace rendering {
Renderer::Renderer(engine::Engine& engine, world::World& world, input::Selection& selection)
    : engine(engine), world(world), selection(selection) {
}

bool Renderer::init() {
  // TODO(kantoniak): Renderer::init(): clean it up

  clearColor = glm::vec3(89, 159, 209) / 255.f;

  // TODO(kantoniak): Handle loader/compiler/linker failure when initializing shaders
  GLuint vertexShader = ShaderManager::compileShader(GL_VERTEX_SHADER, "shaders/terrain.vs", engine.getLogger());
  GLuint fragmentShader = ShaderManager::compileShader(GL_FRAGMENT_SHADER, "shaders/terrain.fs", engine.getLogger());
  this->shaderProgram = ShaderManager::linkProgram(vertexShader, 0, fragmentShader, engine.getLogger());
  transformLoc = glGetUniformLocation(shaderProgram, "transform");
  selectionLoc = glGetUniformLocation(shaderProgram, "selection");
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  GLfloat vertices[] = {
      data::Chunk::SIDE_LENGTH, 0, 0, 0, 0, 0, data::Chunk::SIDE_LENGTH, 0, data::Chunk::SIDE_LENGTH, 0, 0,
      data::Chunk::SIDE_LENGTH};

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
  glEnableVertexAttribArray(0);

  std::vector<glm::vec2> chunkPositions;
  chunkPositions.reserve(world.getMap().getChunksCount());

  auto chunkPositionsIt = chunkPositions.begin();
  for (data::Chunk* chunk : world.getMap().getChunks()) {
    *chunkPositionsIt = glm::vec2(chunk->getPosition()) * (float)data::Chunk::SIDE_LENGTH;
    chunkPositionsIt++;
  }

  glGenBuffers(1, &terrainPositionVBO);
  glBindBuffer(GL_ARRAY_BUFFER, terrainPositionVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * world.getMap().getChunksCount(), &chunkPositions[0].x,
               GL_STATIC_DRAW);

  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
  glVertexAttribDivisor(1, 1);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  // TODO(kantoniak): Load ground texture from file
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
  delete[] pixels;

  // Buildings
  GLfloat building[] = {// 8x2
                        0.2f, 0.0f, 0.2f, 0.2f, 1.0f, 0.2f, 1.8f, 0.0f, 0.2f, 1.8f, 1.0f, 0.2f,
                        1.8f, 0.0f, 1.8f, 1.8f, 1.0f, 1.8f, 0.2f, 0.0f, 1.8f, 0.2f, 1.0f, 1.8f,
                        0.2f, 0.0f, 0.2f, 0.2f, 1.0f, 0.2f, 0.2f, 0.0f, 0.2f, 0.2f, 1.0f, 0.2f, // Reset
                        1.8f, 1.0f, 0.2f, 0.2f, 1.0f, 0.2f, 1.8f, 1.0f, 1.8f, 0.2f, 1.0f, 1.8f};

  const unsigned int buildingCount = world.getMap().getBuildingCount();
  std::vector<glm::vec3> buildingPositions;
  buildingPositions.reserve(buildingCount);

  auto buildingPositionsIt = buildingPositions.begin();
  for (data::Chunk* chunk : world.getMap().getChunks()) {
    for (data::buildings::Building building : chunk->getResidentials()) {
      *buildingPositionsIt = glm::vec3(building.x, building.level, building.y);
      buildingPositionsIt++;
    }
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
  glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * buildingCount, &buildingPositions[0], GL_STATIC_DRAW);

  glEnableVertexAttribArray(1);
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
  glDeleteBuffers(1, &terrainPositionVBO);
  glDeleteProgram(this->shaderProgram);

  glDeleteVertexArrays(1, &buildingsVAO);
  glDeleteBuffers(1, &buildingsVBO);
  glDeleteBuffers(1, &buildingsInstanceVBO);
  glDeleteProgram(this->buildingsShaderProgram);
}

void Renderer::renderWorld() {
  glClearColor(clearColor.x, clearColor.y, clearColor.z, 1.f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glm::mat4 vp = world.getCamera().getViewProjectionMatrix();

  // Terrain
  glUseProgram(shaderProgram);
  glBindVertexArray(VAO);
  glBindTexture(GL_TEXTURE_2D, texture);

  glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(vp));
  glUniform4i(selectionLoc, selection.getFrom().x, selection.getFrom().y, selection.getTo().x + 1,
              selection.getTo().y + 1);
  glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, world.getMap().getChunksCount());

  glBindTexture(GL_TEXTURE_2D, 0);

  // Buildings
  glUseProgram(buildingsShaderProgram);
  glBindVertexArray(buildingsVAO);

  glUniformMatrix4fv(buildingsTransformLoc, 1, GL_FALSE, glm::value_ptr(vp));
  glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 16, world.getMap().getBuildingCount());

  glBindVertexArray(0);

  glfwSwapBuffers(&engine.getWindowHandler().getWindow());
}
}