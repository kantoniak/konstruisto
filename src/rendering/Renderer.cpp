#include "Renderer.hpp"

#include "../data/Chunk.hpp"

namespace rendering {
Renderer::Renderer(engine::Engine& engine, world::World& world, input::Selection& selection)
    : engine(engine), world(world), selection(selection) {
  clearColor = glm::vec3(89, 159, 209) / 255.f;
}

bool Renderer::init() {
  // TODO(kantoniak): Renderer::init(): clean it up

  // TODO(kantoniak): Handle loader/compiler/linker failure when initializing shaders
  GLuint vertexShader = ShaderManager::compileShader(GL_VERTEX_SHADER, "assets/shaders/terrain.vs", engine.getLogger());
  GLuint fragmentShader =
      ShaderManager::compileShader(GL_FRAGMENT_SHADER, "assets/shaders/terrain.fs", engine.getLogger());
  this->shaderProgram = ShaderManager::linkProgram(vertexShader, 0, fragmentShader, engine.getLogger());
  transformLoc = glGetUniformLocation(shaderProgram, "transform");
  renderGridLoc = glGetUniformLocation(shaderProgram, "renderGrid");
  selectionLoc = glGetUniformLocation(shaderProgram, "selection");
  selectionColorLoc = glGetUniformLocation(shaderProgram, "selectionColor");
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
    glBindTexture(GL_TEXTURE_2D, roadTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    glGenerateMipmap(GL_TEXTURE_2D);
    delete[] pixels;
  }

  // Buildings
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

  GLuint buildingsVertexShader =
      ShaderManager::compileShader(GL_VERTEX_SHADER, "assets/shaders/buildings.vs", engine.getLogger());
  GLuint buildingsGeomShader =
      ShaderManager::compileShader(GL_GEOMETRY_SHADER, "assets/shaders/buildings.gs", engine.getLogger());
  GLuint buildingsFragmentShader =
      ShaderManager::compileShader(GL_FRAGMENT_SHADER, "assets/shaders/buildings.fs", engine.getLogger());
  this->buildingsShaderProgram = ShaderManager::linkProgram(buildingsVertexShader, buildingsGeomShader,
                                                            buildingsFragmentShader, engine.getLogger());
  buildingsTransformLoc = glGetUniformLocation(buildingsShaderProgram, "transform");

  GLuint buildingNormalsGeomShader =
      ShaderManager::compileShader(GL_GEOMETRY_SHADER, "assets/shaders/buildings_normals.gs", engine.getLogger());
  GLuint buildingsNormalFragmentShader =
      ShaderManager::compileShader(GL_FRAGMENT_SHADER, "assets/shaders/buildings_normals.fs", engine.getLogger());
  this->buildingNormalsShaderProgram = ShaderManager::linkProgram(buildingsVertexShader, buildingNormalsGeomShader,
                                                                  buildingsNormalFragmentShader, engine.getLogger());
  buildingNormalsTransformLoc = glGetUniformLocation(buildingNormalsShaderProgram, "transform");

  glDeleteShader(buildingsVertexShader);
  glDeleteShader(buildingsGeomShader);
  glDeleteShader(buildingsVertexShader);
  glDeleteShader(buildingNormalsGeomShader);
  glDeleteShader(buildingsNormalFragmentShader);

  glClearColor(clearColor.x, clearColor.y, clearColor.z, 1.f);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
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

  glDeleteProgram(this->buildingNormalsShaderProgram);
}

void Renderer::markBuildingDataForUpdate() {
  resendBuildingData = true;
}

void Renderer::prepareFrame() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void Renderer::renderWorld() {

  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);

  const glm::mat4 vp = world.getCamera().getViewProjectionMatrix();

  // Terrain
  glUseProgram(shaderProgram);
  glBindVertexArray(VAO);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, gridTexture);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, roadTexture);

  glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(vp));
  glUniform1i(renderGridLoc, engine.getSettings().world.showGrid);
  glUniform4i(selectionLoc, selection.getFrom().x, selection.getFrom().y, selection.getTo().x + 1,
              selection.getTo().y + 1);
  glUniform4f(selectionColorLoc, selection.getColor().x, selection.getColor().y, selection.getColor().z,
              engine.getSettings().rendering.renderSelection ? selection.getColor().w : 0);
  glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, world.getMap().getChunksCount());

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

void Renderer::renderDebug() {
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

void Renderer::renderUI() {
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
  char speedLabel = '0';
  nvgFontSize(context, 16.0f);
  nvgTextAlign(context, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);
  for (int i = 0; i <= world.getTimer().getMaxSpeed(); i++) {
    nvgText(context, viewport.x / 2 - topbarWidth / 2 + cityNameBlockWidth + topbarOuterMargin + 2 * topbarInnerMargin +
                         dateWidth + UI::ICON_SIDE / 2 + (speedLabel - '0') * (UI::ICON_SIDE + topbarInnerMargin),
            topbarHeight / 2, &speedLabel, nullptr);
    speedLabel++;
  }
}

void Renderer::renderDebugUI() {
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

void Renderer::sendFrame() {
  glfwSwapBuffers(&engine.getWindowHandler().getWindow());
}

void Renderer::sendBuildingData() {
  const unsigned int buildingCount = world.getMap().getBuildingCount();
  std::vector<glm::vec3> buildingPositions;
  buildingPositions.reserve(2 * buildingCount);

  constexpr float buildingMargin = 0.2f;
  auto buildingPositionsIt = buildingPositions.begin();
  for (data::Chunk* chunk : world.getMap().getChunks()) {
    for (data::buildings::Building building : chunk->getResidentials()) {
      *buildingPositionsIt = glm::vec3(building.x + buildingMargin, 0, building.y + buildingMargin);
      buildingPositionsIt++;
      *buildingPositionsIt =
          glm::vec3(building.width - 2 * buildingMargin, building.level, building.length - 2 * buildingMargin);
      buildingPositionsIt++;
    }
  }

  glBindVertexArray(buildingsVAO);
  glBindBuffer(GL_ARRAY_BUFFER, buildingsInstanceVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * 2 * buildingCount, &buildingPositions[0], GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}
}