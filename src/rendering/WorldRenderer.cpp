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

  // Terrain shader program
  {
    std::optional<Shader> vert_shader = compileShader(Shader::VERTEX_SHADER, "assets/shaders/terrain.glsl.vert");
    if (!vert_shader.has_value()) {
      return false;
    }

    std::optional<Shader> frag_shader = compileShader(Shader::FRAGMENT_SHADER, "assets/shaders/terrain.glsl.frag");
    if (!frag_shader.has_value()) {
      vert_shader.value().delete_shader();
      return false;
    }

    const std::array shaders = {std::move(vert_shader.value()), std::move(frag_shader.value())};
    std::optional<ShaderProgram> shader_program = ShaderManager::linkProgram(shaders, engine.getLogger());
    if (!shader_program.has_value()) {
      for (auto shader : shaders) {
        shader.delete_shader();
      }
      return false;
    }
    this->terrain_shader_prog = shader_program.value();

    transformLoc = glGetUniformLocation(terrain_shader_prog.get_id(), "transform");
    renderGridLoc = glGetUniformLocation(terrain_shader_prog.get_id(), "renderGrid");
    selectionLoc = glGetUniformLocation(terrain_shader_prog.get_id(), "selection");
    selectionColorLoc = glGetUniformLocation(terrain_shader_prog.get_id(), "selectionColor");
    groundTextureLoc = glGetUniformLocation(terrain_shader_prog.get_id(), "groundTexture");
    roadTextureLoc = glGetUniformLocation(terrain_shader_prog.get_id(), "roadTexture");

    for (auto shader : shaders) {
      shader.delete_shader();
    }
  }

  // Buildings/normals vertex shader
  std::optional<Shader> vert_shader = compileShader(Shader::VERTEX_SHADER, "assets/shaders/buildings.glsl.vert");
  if (!vert_shader.has_value()) {
    this->cleanup_shaders();
    return false;
  }

  // Buildings shader program
  {
    std::optional<Shader> frag_shader = compileShader(Shader::FRAGMENT_SHADER, "assets/shaders/buildings.glsl.frag");
    if (!frag_shader.has_value()) {
      this->cleanup_shaders();
      vert_shader.value().delete_shader();
      return false;
    }

    std::optional<Shader> geom_shader = compileShader(Shader::GEOMETRY_SHADER, "assets/shaders/buildings.glsl.geom");
    if (!geom_shader.has_value()) {
      this->cleanup_shaders();
      vert_shader.value().delete_shader();
      frag_shader.value().delete_shader();
      return false;
    }

    const std::array shaders = {std::move(vert_shader.value()), std::move(geom_shader.value()),
                                std::move(frag_shader.value())};
    std::optional<ShaderProgram> shader_program = ShaderManager::linkProgram(shaders, engine.getLogger());
    if (!shader_program.has_value()) {
      this->cleanup_shaders();
      for (auto shader : shaders) {
        shader.delete_shader();
      }
      return false;
    }
    this->building_shader_prog = shader_program.value();
    buildingsTransformLoc = glGetUniformLocation(building_shader_prog.get_id(), "transform");

    geom_shader.value().delete_shader();
    frag_shader.value().delete_shader();
  }

  // Building normals shader program
  {
    std::optional<Shader> frag_shader =
        compileShader(Shader::FRAGMENT_SHADER, "assets/shaders/buildings_normals.glsl.frag");
    if (!frag_shader.has_value()) {
      this->cleanup_shaders();
      vert_shader.value().delete_shader();
      return false;
    }

    std::optional<Shader> geom_shader =
        compileShader(Shader::GEOMETRY_SHADER, "assets/shaders/buildings_normals.glsl.geom");
    if (!geom_shader.has_value()) {
      this->cleanup_shaders();
      vert_shader.value().delete_shader();
      frag_shader.value().delete_shader();
      return false;
    }

    const std::array shaders = {std::move(vert_shader.value()), std::move(geom_shader.value()),
                                std::move(frag_shader.value())};
    std::optional<ShaderProgram> shader_program = ShaderManager::linkProgram(shaders, engine.getLogger());
    if (!shader_program.has_value()) {
      this->cleanup_shaders();
      for (auto shader : shaders) {
        shader.delete_shader();
      }
      return false;
    }
    this->building_normals_shader_prog = shader_program.value();
    buildingNormalsTransformLoc = glGetUniformLocation(building_normals_shader_prog.get_id(), "transform");

    for (auto shader : shaders) {
      shader.delete_shader();
    }
  }

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
    stbi_image_free(pixels);
  }

  {
    int width, height;
    unsigned char* pixels = stbi_load("assets/textures/road.png", &width, &height, nullptr, STBI_rgb_alpha);

    // Texture of the ground
    // TODO(kantoniak): Better system for atlas sizing
    glGenTextures(1, &roadTexture);
    glBindTexture(GL_TEXTURE_2D_ARRAY, roadTexture);
    glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGBA8, 320, 320, 1);
    glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, 0, 320, 320, 1, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    stbi_image_free(pixels);
  }

  return true;
}

bool WorldRenderer::setupTerrain() {
  glGenVertexArrays(1, &VAO);
  markTileDataForUpdate();
  return true;
}

bool WorldRenderer::setupBuildings() {
  std::array<GLfloat, 8 * 2 * 3> building = {
      // 8x2
      0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
      0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, // Reset
      1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f};

  glGenVertexArrays(1, &buildingsVAO);
  glBindVertexArray(buildingsVAO);

  glGenBuffers(1, &buildingsVBO);
  glBindBuffer(GL_ARRAY_BUFFER, buildingsVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(building), building.data(), GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)nullptr);

  glGenBuffers(1, &buildingsInstanceVBO);
  glBindBuffer(GL_ARRAY_BUFFER, buildingsInstanceVBO);

  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)nullptr);
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

  for (auto& chunk : chunks) {
    glDeleteBuffers(1, &(chunk.second));
  }

  glDeleteVertexArrays(1, &buildingsVAO);
  glDeleteBuffers(1, &buildingsVBO);
  glDeleteBuffers(1, &buildingsInstanceVBO);

  this->cleanup_shaders();
  Renderer::cleanup();
}

void WorldRenderer::cleanup_shaders() {
  this->terrain_shader_prog.delete_program();
  this->building_shader_prog.delete_program();
  this->building_normals_shader_prog.delete_program();
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

  terrain_shader_prog.use();
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)nullptr);
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

  if (world.getMap().getBuildingCount() > 0) {
    building_shader_prog.use();
    glBindVertexArray(buildingsVAO);

    glUniformMatrix4fv(buildingsTransformLoc, 1, GL_FALSE, glm::value_ptr(vp));
    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 16, world.getMap().getBuildingCount());
  }

  glBindVertexArray(0);
  glFlush();
}

void WorldRenderer::renderDebug() {
  if (engine.getSettings().rendering.renderNormals) {
    const glm::mat4 vp = world.getCamera().getViewProjectionMatrix();

    building_normals_shader_prog.use();
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
  nvgRect(context,
          viewport.x / 2 - topbarWidth / 2 + cityNameBlockWidth + topbarOuterMargin + dateWidth +
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
  nvgText(context,
          viewport.x / 2 - topbarWidth / 2 + cityNameBlockWidth + dateBlockWidth + 2 * topbarOuterMargin +
              topbarInnerMargin,
          topbarHeight / 2, people.c_str(), nullptr);
  nvgText(context,
          viewport.x / 2 - topbarWidth / 2 + cityNameBlockWidth + dateBlockWidth + 2 * topbarOuterMargin +
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
    const unsigned short optionsCount = 5;
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

    x = (buttonSide - UI::ICON_SIDE_24) / 2;
    y += x;

    engine.getUI().renderIcon(UI::ICON_BUILDING, x, y, UI::ICON_SIDE_24);
    y += buttonSide;
    engine.getUI().renderIcon(UI::ICON_ZONES, x, y, UI::ICON_SIDE_24);
    y += buttonSide;
    engine.getUI().renderIcon(UI::ICON_ROAD, x, y, UI::ICON_SIDE_24);
    y += buttonSide;
    engine.getUI().renderIcon(UI::ICON_BULDOZER, x, y, UI::ICON_SIDE_24);
    y += buttonSide;
    engine.getUI().renderIcon(UI::ICON_MORE, x, y, UI::ICON_SIDE_24);
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
  if (buildingCount < 1) {
    return;
  }

  std::vector<glm::vec3> buildingPositions;
  buildingPositions.reserve(2 * buildingCount);

  constexpr float buildingMargin = 0.2f;
  for (data::Chunk* chunk : world.getMap().getChunks()) {
    for (data::buildings::Building building : chunk->getResidentials()) {
      buildingPositions.emplace_back(building.x + buildingMargin, 0, building.y + buildingMargin);
      buildingPositions.emplace_back(building.width - 2 * buildingMargin, building.level,
                                     building.length - 2 * buildingMargin);
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
    this->paintOnTiles(*chunk, chunk->getPosition(), tiles);

    // Concatenate
    for (unsigned int i = 0; i < verticesCount; i++) {
      toBuffer[i * 4] = positions[i].x;
      toBuffer[i * 4 + 1] = positions[i].y;
      toBuffer[i * 4 + 2] = positions[i].z;
      toBuffer[i * 4 + 3] = tiles[i];
    }

    // Send buffer
    auto key = std::make_pair(chunk->getPosition().x, chunk->getPosition().y);
    if (chunks.find(key) == chunks.end()) {
      glGenBuffers(1, &chunkVBO);
      chunks[key] = chunkVBO;
    } else {
      chunkVBO = chunks[key];
    }
    glBindBuffer(GL_ARRAY_BUFFER, chunkVBO);
    glBufferDataVector(GL_ARRAY_BUFFER, toBuffer, GL_STATIC_DRAW);
  }

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

unsigned int WorldRenderer::getTile(int x, int y) const {
  return y * ATLAS_SIDE + x + 1;
}

void WorldRenderer::setTile(std::vector<GLfloat>& tiles, int x, int y, unsigned int tile) {
  if (x < 0 || (int)data::Chunk::SIDE_LENGTH <= x || y < 0 || (int)data::Chunk::SIDE_LENGTH <= y) {
    return;
  }
  for (int i = 0; i < 6; i++) {
    tiles[(y * data::Chunk::SIDE_LENGTH + x) * 6 + i] = tile;
  }
}

void WorldRenderer::paintOnTiles(const data::Chunk& chunk, const glm::ivec2& position, std::vector<GLfloat>& tiles) {
  this->paintRoadsOnTiles(chunk.getRoadGraph(), tiles);

  for (data::Lot lot : chunk.getLots()) {
    this->paintLotOnTiles(lot, position, tiles);
  }
}

void WorldRenderer::paintLotOnTiles(const data::Lot& lot, const glm::ivec2& position, std::vector<GLfloat>& tiles) {

  int minX = lot.position.getLocal(position).x;
  int minY = lot.position.getLocal(position).y;
  int maxX = minX + lot.size.x - 1;
  int maxY = minY + lot.size.y - 1;
  for (int x = minX + 1; x < maxX; x++) {
    setTile(tiles, x, minY, getTile(6, 2));
    for (int y = minY + 1; y < maxY; y++) {
      setTile(tiles, x, y, getTile(6, 3));
    }
    setTile(tiles, x, maxY, getTile(6, 4));
  }
  for (int y = minY + 1; y < maxY; y++) {
    setTile(tiles, minX, y, getTile(5, 3));
    setTile(tiles, maxX, y, getTile(7, 3));
  }
  setTile(tiles, minX, minY, getTile(5, 2));
  setTile(tiles, maxX, minY, getTile(7, 2));
  setTile(tiles, minX, maxY, getTile(5, 4));
  setTile(tiles, maxX, maxY, getTile(7, 4));

  // Render markers
  if (data::Direction::N == lot.direction) {
    setTile(tiles, minX, maxY, getTile(5, 1));
    for (int x = minX + 1; x < maxX; x++) {
      setTile(tiles, x, maxY, getTile(6, 1));
    }
    setTile(tiles, maxX, maxY, getTile(7, 1));
  }

  if (data::Direction::S == lot.direction) {
    setTile(tiles, minX, minY, getTile(5, 0));
    for (int x = minX + 1; x < maxX; x++) {
      setTile(tiles, x, minY, getTile(6, 0));
    }
    setTile(tiles, maxX, minY, getTile(7, 0));
  }

  if (data::Direction::W == lot.direction) {
    setTile(tiles, maxX, minY, getTile(9, 0));
    for (int y = minY + 1; y < maxY; y++) {
      setTile(tiles, maxX, y, getTile(9, 1));
    }
    setTile(tiles, maxX, maxY, getTile(9, 2));
  }

  if (data::Direction::E == lot.direction) {
    setTile(tiles, minX, minY, getTile(8, 0));
    for (int y = minY + 1; y < maxY; y++) {
      setTile(tiles, minX, y, getTile(8, 1));
    }
    setTile(tiles, minX, maxY, getTile(8, 2));
  }
}

void WorldRenderer::paintRoadsOnTiles(const data::RoadGraph<data::Chunk::SIDE_LENGTH>& roads,
                                      std::vector<GLfloat>& tiles) {

  for (unsigned int x = 0; x < data::Chunk::SIDE_LENGTH; x++) {
    for (unsigned int y = 0; y < data::Chunk::SIDE_LENGTH; y++) {
      switch (roads.getLayerData()[y * data::Chunk::SIDE_LENGTH + x]) {

      case data::RoadGraph<data::Chunk::SIDE_LENGTH>::ROAD_NONE:
        setTile(tiles, x, y, getTile(0, 3));
        break;

      case data::RoadGraph<data::Chunk::SIDE_LENGTH>::ROAD_N:
        setTile(tiles, x, y, getTile(3, 2));
        break;

      case data::RoadGraph<data::Chunk::SIDE_LENGTH>::ROAD_S:
        setTile(tiles, x, y, getTile(1, 2));
        break;

      case data::RoadGraph<data::Chunk::SIDE_LENGTH>::ROAD_W:
        setTile(tiles, x, y, getTile(2, 2));
        break;

      case data::RoadGraph<data::Chunk::SIDE_LENGTH>::ROAD_E:
        setTile(tiles, x, y, getTile(4, 2));
        break;

      case data::RoadGraph<data::Chunk::SIDE_LENGTH>::ROAD_WE:
        setTile(tiles, x, y, getTile(0, 1));
        break;

      case data::RoadGraph<data::Chunk::SIDE_LENGTH>::ROAD_SE:
        setTile(tiles, x, y, getTile(4, 1));
        break;

      case data::RoadGraph<data::Chunk::SIDE_LENGTH>::ROAD_SW:
        setTile(tiles, x, y, getTile(3, 1));
        break;

      case data::RoadGraph<data::Chunk::SIDE_LENGTH>::ROAD_NE:
        setTile(tiles, x, y, getTile(4, 0));
        break;

      case data::RoadGraph<data::Chunk::SIDE_LENGTH>::ROAD_NW:
        setTile(tiles, x, y, getTile(3, 0));
        break;

      case data::RoadGraph<data::Chunk::SIDE_LENGTH>::ROAD_NS:
        setTile(tiles, x, y, getTile(2, 1));
        break;

      case data::RoadGraph<data::Chunk::SIDE_LENGTH>::ROAD_NSW:
        setTile(tiles, x, y, getTile(1, 0));
        break;

      case data::RoadGraph<data::Chunk::SIDE_LENGTH>::ROAD_NSE:
        setTile(tiles, x, y, getTile(2, 0));
        break;

      case data::RoadGraph<data::Chunk::SIDE_LENGTH>::ROAD_NWE:
        setTile(tiles, x, y, getTile(0, 0));
        break;

      case data::RoadGraph<data::Chunk::SIDE_LENGTH>::ROAD_SWE:
        setTile(tiles, x, y, getTile(0, 2));
        break;

      case data::RoadGraph<data::Chunk::SIDE_LENGTH>::ROAD_NSWE:
        setTile(tiles, x, y, getTile(1, 1));
        break;

      default:
        break;
      }
    }
  }
}
}