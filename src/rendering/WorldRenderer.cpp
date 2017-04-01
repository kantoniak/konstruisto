#include "WorldRenderer.hpp"

#include "../data/Chunk.hpp"

namespace rendering {
WorldRenderer::WorldRenderer(engine::Engine& engine, world::World& world) : Renderer(engine), world(world) {
  clearColor = glm::vec3(89, 159, 209) / 255.f;
}

bool WorldRenderer::init() {
  Renderer::init();

  if (!setupShaders() || !setupTextures()) {
    return false;
  }

  if (!setupTerrain() || !setupBuildings()) {
    return false;
  }

  glClearColor(clearColor.x, clearColor.y, clearColor.z, 1.f);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);

  return true;
}

bool WorldRenderer::setupShaders() {
  // TODO(kantoniak): Handle loader/compiler/linker failure when initializing shaders

  GLuint vertexShader = compileShader(GL_VERTEX_SHADER, "assets/shaders/terrain.vs");
  GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, "assets/shaders/terrain.fs");
  this->shaderProgram = ShaderManager::linkProgram(vertexShader, 0, fragmentShader, engine.getLogger());
  transformLoc = glGetUniformLocation(shaderProgram, "transform");
  renderGridLoc = glGetUniformLocation(shaderProgram, "renderGrid");
  selectionLoc = glGetUniformLocation(shaderProgram, "selection");
  selectionColorLoc = glGetUniformLocation(shaderProgram, "selectionColor");
  groundTextureLoc = glGetUniformLocation(shaderProgram, "groundTexture");
  roadTextureLoc = glGetUniformLocation(shaderProgram, "roadTexture");

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  GLuint buildingsVertexShader = compileShader(GL_VERTEX_SHADER, "assets/shaders/buildings.vs");
  GLuint buildingsGeomShader = compileShader(GL_GEOMETRY_SHADER, "assets/shaders/buildings.gs");
  GLuint buildingsFragmentShader = compileShader(GL_FRAGMENT_SHADER, "assets/shaders/buildings.fs");
  this->buildingsShaderProgram = ShaderManager::linkProgram(buildingsVertexShader, buildingsGeomShader,
                                                            buildingsFragmentShader, engine.getLogger());
  buildingsTransformLoc = glGetUniformLocation(buildingsShaderProgram, "transform");

  GLuint buildingNormalsGeomShader = compileShader(GL_GEOMETRY_SHADER, "assets/shaders/buildings_normals.gs");
  GLuint buildingsNormalFragmentShader = compileShader(GL_FRAGMENT_SHADER, "assets/shaders/buildings_normals.fs");
  this->buildingNormalsShaderProgram = ShaderManager::linkProgram(buildingsVertexShader, buildingNormalsGeomShader,
                                                                  buildingsNormalFragmentShader, engine.getLogger());
  buildingNormalsTransformLoc = glGetUniformLocation(buildingNormalsShaderProgram, "transform");

  glDeleteShader(buildingsVertexShader);
  glDeleteShader(buildingsGeomShader);
  glDeleteShader(buildingsVertexShader);
  glDeleteShader(buildingNormalsGeomShader);
  glDeleteShader(buildingsNormalFragmentShader);

  return true;
}

bool WorldRenderer::setupTextures() {
  {
    int width, height;
    unsigned char* pixels = stbi_load("assets/textures/grid.png", &width, &height, nullptr, STBI_rgb_alpha);

    // Texture of the ground
    glGenTextures(1, &gridTexture);
    glBindTexture(GL_TEXTURE_2D, gridTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    glGenerateMipmap(GL_TEXTURE_2D);
    delete[] pixels;
  }

  {
    int width, height;
    unsigned char* pixels = stbi_load("assets/textures/road.png", &width, &height, nullptr, STBI_rgb_alpha);

    // Texture of the ground
    glGenTextures(1, &roadTexture);
    glBindTexture(GL_TEXTURE_2D_ARRAY, roadTexture);
    glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGBA8, 160, 160, 1);
    glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, 0, 160, 160, 1, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    delete[] pixels;
  }

  return true;
}

bool WorldRenderer::setupTerrain() {
  glGenVertexArrays(1, &VAO);
  markTileDataForUpdate();
  return true;
}

bool WorldRenderer::setupBuildings() {
  GLfloat building[] = {// 8x2
                        0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
                        1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
                        0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, // Reset
                        1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f};

  glGenVertexArrays(1, &buildingsVAO);
  glBindVertexArray(buildingsVAO);

  glGenBuffers(1, &buildingsVBO);
  glBindBuffer(GL_ARRAY_BUFFER, buildingsVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(building), building, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

  glGenBuffers(1, &buildingsInstanceVBO);
  glBindBuffer(GL_ARRAY_BUFFER, buildingsInstanceVBO);

  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
  glVertexAttribDivisor(1, 1);

  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
  glVertexAttribDivisor(2, 1);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  markBuildingDataForUpdate();

  return true;
}

void WorldRenderer::cleanup() {
  glUseProgram(0);

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &terrainPositionVBO);
  glDeleteProgram(this->shaderProgram);

  for (auto it = chunks.begin(); it != chunks.end(); it++) {
    glDeleteBuffers(1, &(it->second));
  }

  glDeleteVertexArrays(1, &buildingsVAO);
  glDeleteBuffers(1, &buildingsVBO);
  glDeleteBuffers(1, &buildingsInstanceVBO);
  glDeleteProgram(this->buildingsShaderProgram);

  glDeleteProgram(this->buildingNormalsShaderProgram);

  Renderer::cleanup();
}

void WorldRenderer::markBuildingDataForUpdate() {
  resendBuildingData = true;
}

void WorldRenderer::markTileDataForUpdate() {
  resendTileData = true;
}

void WorldRenderer::renderWorld(const input::Selection& selection) {

  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);

  const glm::mat4 vp = world.getCamera().getViewProjectionMatrix();

  // Terrain
  if (resendTileData) {
    sendTileData();
    resendTileData = false;
  }

  glUseProgram(shaderProgram);
  glBindVertexArray(VAO);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, gridTexture);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D_ARRAY, roadTexture);
  glUniform1i(groundTextureLoc, 0);
  glUniform1i(roadTextureLoc, 1);

  glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(vp));
  glUniform1i(renderGridLoc, engine.getSettings().world.showGrid);
  glUniform4i(selectionLoc, selection.getFrom().x, selection.getFrom().y, selection.getTo().x + 1,
              selection.getTo().y + 1);
  glUniform4f(selectionColorLoc, selection.getColor().x, selection.getColor().y, selection.getColor().z,
              engine.getSettings().rendering.renderSelection ? selection.getColor().w : 0);

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  for (data::Chunk* chunk : world.getMap().getChunks()) {
    glBindBuffer(GL_ARRAY_BUFFER, chunks[std::make_pair(chunk->getPosition().x, chunk->getPosition().y)]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));

    glDrawArrays(GL_TRIANGLES, 0, data::Chunk::SIDE_LENGTH * data::Chunk::SIDE_LENGTH * 2 * 3);
  }
  glDisableVertexAttribArray(1);
  glDisableVertexAttribArray(0);

  // Buildings
  if (resendBuildingData) {
    sendBuildingData();
    resendBuildingData = false;
  }

  glUseProgram(buildingsShaderProgram);
  glBindVertexArray(buildingsVAO);

  glUniformMatrix4fv(buildingsTransformLoc, 1, GL_FALSE, glm::value_ptr(vp));
  glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 16, world.getMap().getBuildingCount());

  glBindVertexArray(0);
  glFlush();
}

void WorldRenderer::renderDebug() {
  if (engine.getSettings().rendering.renderNormals) {
    const glm::mat4 vp = world.getCamera().getViewProjectionMatrix();

    glUseProgram(buildingNormalsShaderProgram);
    glBindVertexArray(buildingsVAO);

    glUniformMatrix4fv(buildingsTransformLoc, 1, GL_FALSE, glm::value_ptr(vp));
    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 16, world.getMap().getBuildingCount());

    glBindVertexArray(0);
    glFlush();
  }
}

void WorldRenderer::renderUI() {
  const glm::vec2 viewport = engine.getWindowHandler().getViewportSize();
  NVGcontext* context = engine.getUI().getContext();

  constexpr unsigned char topbarHeight = 36;
  constexpr unsigned char topbarInnerMargin = 12;
  constexpr unsigned char topbarOuterMargin = 6;

  const NVGcolor iconBackgroundColor = nvgRGB(68, 68, 68);

  const data::City& city = world.getMap().getCurrentCity();
  const std::string date = world.getTimer().getDate();
  const std::string people = "People: " + std::to_string(city.people);
  const std::string money = "€" + std::to_string(city.money);

  nvgTextAlign(context, NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);
  nvgFontFace(context, rendering::UI::FONT_SSP_BOLD);
  nvgFontSize(context, 22.0f);
  float cityNameWidth = nvgTextBounds(context, 0, 0, city.name.c_str(), nullptr, nullptr);

  nvgFontFace(context, rendering::UI::FONT_SSP_REGULAR);
  nvgFontSize(context, 19.0f);
  float dateWidth = nvgTextBounds(context, 0, 0, date.c_str(), nullptr, nullptr);
  float peopleWidth = nvgTextBounds(context, 0, 0, people.c_str(), nullptr, nullptr);
  float moneyWidth = nvgTextBounds(context, 0, 0, money.c_str(), nullptr, nullptr);

  const unsigned short cityNameBlockWidth = cityNameWidth + 2 * topbarInnerMargin;
  const unsigned short dateBlockWidth =
      (1 + world.getTimer().getMaxSpeed()) * (UI::ICON_SIDE + topbarInnerMargin) + dateWidth + 2 * topbarInnerMargin;
  const unsigned short cityNumbersBlockWidth = peopleWidth + moneyWidth + 3 * topbarInnerMargin;
  const unsigned short topbarWidth =
      cityNameBlockWidth + dateBlockWidth + cityNumbersBlockWidth + 2 * topbarOuterMargin;

  nvgBeginPath(context);
  nvgRect(context, viewport.x / 2 - topbarWidth / 2, 0, cityNameBlockWidth, topbarHeight);
  nvgRect(context, viewport.x / 2 - topbarWidth / 2 + cityNameBlockWidth + topbarOuterMargin, 0, dateBlockWidth,
          topbarHeight);
  nvgRect(context, viewport.x / 2 - topbarWidth / 2 + cityNameBlockWidth + dateBlockWidth + 2 * topbarOuterMargin, 0,
          cityNumbersBlockWidth, topbarHeight);
  nvgFillColor(context, engine.getUI().getBackgroundColor());
  nvgFill(context);

  // Icon backgrounds
  short speedIcon = world.getTimer().paused() ? 0 : world.getTimer().getSpeed();
  nvgBeginPath(context);
  nvgRect(context, viewport.x / 2 - topbarWidth / 2 + cityNameBlockWidth + topbarOuterMargin + dateWidth +
                       2 * topbarInnerMargin + speedIcon * (UI::ICON_SIDE + topbarInnerMargin),
          topbarHeight / 2 - UI::ICON_SIDE / 2, UI::ICON_SIDE, UI::ICON_SIDE);
  nvgFillColor(context, iconBackgroundColor);
  nvgFill(context);

  nvgFillColor(context, engine.getUI().getPrimaryTextColor());

  nvgFontFace(context, rendering::UI::FONT_SSP_BOLD);
  nvgFontSize(context, 22.0f);
  nvgText(context, viewport.x / 2 - topbarWidth / 2 + topbarInnerMargin, topbarHeight / 2, city.name.c_str(), nullptr);

  nvgFontFace(context, rendering::UI::FONT_SSP_REGULAR);
  nvgFontSize(context, 19.0f);
  nvgText(context, viewport.x / 2 - topbarWidth / 2 + cityNameBlockWidth + topbarOuterMargin + topbarInnerMargin,
          topbarHeight / 2, date.c_str(), nullptr);
  nvgText(context, viewport.x / 2 - topbarWidth / 2 + cityNameBlockWidth + dateBlockWidth + 2 * topbarOuterMargin +
                       topbarInnerMargin,
          topbarHeight / 2, people.c_str(), nullptr);
  nvgText(context, viewport.x / 2 - topbarWidth / 2 + cityNameBlockWidth + dateBlockWidth + 2 * topbarOuterMargin +
                       topbarInnerMargin * 2 + peopleWidth,
          topbarHeight / 2, money.c_str(), nullptr);

  // Speed numbers
  {
    int x =
        viewport.x / 2 - topbarWidth / 2 + cityNameBlockWidth + topbarOuterMargin + 2 * topbarInnerMargin + dateWidth;
    const int y = topbarHeight / 2 - UI::ICON_SIDE / 2;

    engine.getUI().renderIcon(UI::ICON_SPEED_0, x, y);
    x += UI::ICON_SIDE + topbarInnerMargin;
    engine.getUI().renderIcon(UI::ICON_SPEED_1, x, y);
    x += UI::ICON_SIDE + topbarInnerMargin;
    engine.getUI().renderIcon(UI::ICON_SPEED_2, x, y);
    x += UI::ICON_SIDE + topbarInnerMargin;
    engine.getUI().renderIcon(UI::ICON_SPEED_3, x, y);
  }

  // Left menu
  {
    const unsigned short optionsCount = 3;
    const unsigned short buttonSide = 3 * UI::ICON_SIDE;
    int x = 0;
    int y = viewport.y / 2 - optionsCount * buttonSide / 2.f;

    nvgBeginPath(context);
    nvgRect(context, x, y, buttonSide, buttonSide * optionsCount);
    nvgFillColor(context, engine.getUI().getBackgroundColor());
    nvgFill(context);

    if (leftMenuActiveIcon >= 0) {
      nvgBeginPath(context);
      nvgRect(context, x, y + buttonSide * leftMenuActiveIcon, buttonSide, buttonSide);
      nvgFillColor(context, iconBackgroundColor);
      nvgFill(context);
    }

    x = (buttonSide - UI::ICON_SIDE) / 2;
    y += x;

    engine.getUI().renderIcon(UI::ICON_BUILDING, x, y);
    y += buttonSide;
    engine.getUI().renderIcon(UI::ICON_ROAD, x, y);
    y += buttonSide;
    engine.getUI().renderIcon(UI::ICON_MORE, x, y);
  }
}

void WorldRenderer::renderDebugUI() {
  NVGcontext* context = engine.getUI().getContext();

  constexpr unsigned short margin = 10;
  constexpr unsigned short textMargin = 4;
  constexpr unsigned short lineHeight = 18;

  const std::string fps = "FPS " + std::to_string(engine.getDebugInfo().getFPS());
  const std::string frame = "Frame: " + std::to_string(engine.getDebugInfo().getFrameTime()) + " ms";
  const std::string render = "Render: " + std::to_string(engine.getDebugInfo().getRenderTime()) + " ms";
  const std::string renderWorld = "  World: " + std::to_string(engine.getDebugInfo().getRenderWorldTime()) + " ms";
  const std::string renderUI = "  UI: " + std::to_string(engine.getDebugInfo().getRenderUITime()) + " ms";

  nvgBeginPath(context);
  nvgRect(context, margin, margin, 100, 2 * textMargin + 5 * lineHeight);
  nvgFillColor(context, engine.getUI().getBackgroundColor());
  nvgFill(context);

  nvgFontSize(context, 16.0f);
  nvgFontFace(context, rendering::UI::FONT_SSP_REGULAR);
  nvgTextAlign(context, NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);
  nvgFillColor(context, engine.getUI().getPrimaryTextColor());
  nvgText(context, 1.8f * margin, margin + textMargin + lineHeight / 2.f, fps.c_str(), nullptr);
  nvgText(context, 1.8f * margin, margin + textMargin + lineHeight / 2.f + lineHeight, frame.c_str(), nullptr);
  nvgText(context, 1.8f * margin, margin + textMargin + lineHeight / 2.f + 2 * lineHeight, render.c_str(), nullptr);
  nvgText(context, 1.8f * margin, margin + textMargin + lineHeight / 2.f + 3 * lineHeight, renderWorld.c_str(),
          nullptr);
  nvgText(context, 1.8f * margin, margin + textMargin + lineHeight / 2.f + 4 * lineHeight, renderUI.c_str(), nullptr);
}

void WorldRenderer::setLeftMenuActiveIcon(int index) {
  leftMenuActiveIcon = index;
}

void WorldRenderer::sendBuildingData() {
  const unsigned int buildingCount = world.getMap().getBuildingCount();
  std::vector<glm::vec3> buildingPositions;
  buildingPositions.reserve(2 * buildingCount);

  constexpr float buildingMargin = 0.2f;
  for (data::Chunk* chunk : world.getMap().getChunks()) {
    for (data::buildings::Building building : chunk->getResidentials()) {
      buildingPositions.push_back(glm::vec3(building.x + buildingMargin, 0, building.y + buildingMargin));
      buildingPositions.push_back(
          glm::vec3(building.width - 2 * buildingMargin, building.level, building.length - 2 * buildingMargin));
    }
  }
  glBindVertexArray(buildingsVAO);
  glBindBuffer(GL_ARRAY_BUFFER, buildingsInstanceVBO);
  glBufferDataVector(GL_ARRAY_BUFFER, buildingPositions, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void WorldRenderer::sendTileData() {
  glBindVertexArray(VAO);

  constexpr unsigned long verticesCount = data::Chunk::SIDE_LENGTH * data::Chunk::SIDE_LENGTH * 2 * 3;
  const std::vector<glm::vec3> fieldBase{glm::vec3(1, 0, 0), glm::vec3(0, 0, 0), glm::vec3(1, 0, 1),
                                   glm::vec3(1, 0, 1), glm::vec3(0, 0, 0), glm::vec3(0, 0, 1)};

  // Generate buffers
  std::vector<glm::vec3> positions;
  positions.resize(verticesCount);

  std::vector<GLfloat> tiles;
  tiles.resize(verticesCount);

  GLuint chunkVBO = 0;
  std::vector<GLfloat> toBuffer;
  toBuffer.resize(verticesCount * (3 + 1));

  for (data::Chunk* chunk : world.getMap().getChunks()) {
    glm::vec3 chunkPosition =
        glm::vec3(chunk->getPosition().x, 0, chunk->getPosition().y) * (float)data::Chunk::SIDE_LENGTH;

    // Generate positions
    for (unsigned int x = 0; x < data::Chunk::SIDE_LENGTH; x++) {
      for (unsigned int y = 0; y < data::Chunk::SIDE_LENGTH; y++) {
        for (unsigned int i = 0; i < 6; i++) {
          positions[y * data::Chunk::SIDE_LENGTH * 6 + x * 6 + i] = chunkPosition + glm::vec3(x, 0, y) + fieldBase[i];
        }
      }
    }

    // Generate tiles
    std::fill(tiles.begin(), tiles.end(), 0);
    for (data::roads::Road road : chunk->getRoads()) {
      this->paintRoadOnTiles(road, tiles);
    }

    // Concatenate
    for (unsigned int i = 0; i < verticesCount; i++) {
      toBuffer[i * 4] = positions[i].x;
      toBuffer[i * 4 + 1] = positions[i].y;
      toBuffer[i * 4 + 2] = positions[i].z;
      toBuffer[i * 4 + 3] = tiles[i];
    }

    // Send buffer
    glGenBuffers(1, &chunkVBO);
    glBindBuffer(GL_ARRAY_BUFFER, chunkVBO);
    glBufferDataVector(GL_ARRAY_BUFFER, toBuffer, GL_STATIC_DRAW);
    chunks[std::make_pair(chunk->getPosition().x, chunk->getPosition().y)] = chunkVBO;
  }

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void WorldRenderer::paintRoadOnTiles(data::roads::Road& road, std::vector<GLfloat>& tiles) {

  if (road.direction == data::roads::Direction::W) {
    for (long x = road.x; x < road.x + road.length; x++) {
      for (int i = 0; i < 6; i++) {
        unsigned int index = road.y * data::Chunk::SIDE_LENGTH * 6 + x * 6 + i;
        if (x == road.x) {
          tiles[index] = 1;
        } else if (x == road.x + road.length - 1) {
          tiles[index] = 3;
        } else {
          tiles[index] = 2;
        }
        index = (road.y + 1) * data::Chunk::SIDE_LENGTH * 6 + x * 6 + i;
        if (x == road.x) {
          tiles[index] = 11;
        } else if (x == road.x + road.length - 1) {
          tiles[index] = 13;
        } else {
          tiles[index] = 12;
        }
      }
    }
  }

  if (road.direction == data::roads::Direction::N) {
    for (long y = road.y; y < road.y + road.length; y++) {
      for (int i = 0; i < 6; i++) {
        unsigned int index = y * data::Chunk::SIDE_LENGTH * 6 + (road.x) * 6 + i;
        if (tiles[index] == 2) {
          tiles[index] = 4;
        } else if (tiles[index] == 12) {
          tiles[index] = 9;
        } else {
          if (y == road.y) {
            tiles[index] = 1;
          } else if (y == road.y + road.length - 1) {
            tiles[index] = 11;
          } else {
            tiles[index] = 6;
          }
        }
        index = y * data::Chunk::SIDE_LENGTH * 6 + (road.x + 1) * 6 + i;
        if (tiles[index] == 2) {
          tiles[index] = 5;
        } else if (tiles[index] == 12) {
          tiles[index] = 10;
        } else {
          if (y == road.y) {
            tiles[index] = 3;
          } else if (y == road.y + road.length - 1) {
            tiles[index] = 13;
          } else {
            tiles[index] = 8;
          }
        }
      }
    }
  }
}
}