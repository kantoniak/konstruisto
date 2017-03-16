#ifndef RENDERING_RENDERER_HPP
#define RENDERING_RENDERER_HPP

#include <string>
#include <vector>

#include <nanovg.h>

#include <GL/glew.h>
#include <glm/ext.hpp>
#include <glm/glm.hpp>

#include "../engine/DebugInfo.hpp"
#include "../engine/Engine.hpp"
#include "../input/Selection.hpp"
#include "../input/WindowHandler.hpp"
#include "../settings.hpp"
#include "../world/World.hpp"
#include "ShaderManager.hpp"
#include "UI.hpp"
#include "stb_image.h"

namespace rendering {

class Renderer {

public:
  Renderer(engine::Engine& engine, world::World& world, input::Selection& selection);

  bool init();
  void cleanup();

  void markBuildingDataForUpdate();

  void prepareFrame();
  void renderWorld();
  void renderDebug();

  // TODO(kantoniak): Get rid of Renderer::renderUI() and  Renderer::renderDebugUI()
  void renderUI();
  void renderDebugUI();

  void sendFrame();

protected:
  engine::Engine& engine;
  world::World& world;
  input::Selection& selection;

  glm::vec3 clearColor;

  bool resendBuildingData = false;
  void sendBuildingData();

  // Terrain
  GLuint shaderProgram;
  GLuint transformLoc, terrainPositionLoc, renderGridLoc, selectionLoc, selectionColorLoc;
  GLuint VBO, VAO, terrainPositionVBO;
  GLuint gridTexture, roadTexture;

  // Buildings
  GLuint buildingsVAO, buildingsVBO;
  GLuint buildingsInstanceVBO;
  GLuint buildingsShaderProgram;
  GLuint buildingsTransformLoc;
  const unsigned int buildingsCount = 100;
  const unsigned int sideSize = 20;
  const unsigned int maxHeight = 6;

  // Buildings - normals
  GLuint buildingNormalsTransformLoc;
  GLuint buildingNormalsShaderProgram;
};
}

#endif
