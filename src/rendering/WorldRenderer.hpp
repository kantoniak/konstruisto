#ifndef RENDERING_WORLDRENDERER_HPP
#define RENDERING_WORLDRENDERER_HPP

#include <map>
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
#include "Renderer.hpp"
#include "ShaderManager.hpp"
#include "UI.hpp"
#include "stb_image.h"

namespace rendering {

class WorldRenderer : public Renderer {

public:
  WorldRenderer(engine::Engine& engine, world::World& world);

  virtual bool init();
  bool setupShaders();
  bool setupTextures();
  bool setupTerrain();
  bool setupBuildings();
  virtual void cleanup();

  void markBuildingDataForUpdate();

  void renderWorld(const input::Selection& selection);
  void renderDebug();

  // TODO(kantoniak): Get rid of Renderer::renderUI() and  Renderer::renderDebugUI()
  void renderUI();
  void renderDebugUI();

  void setLeftMenuActiveIcon(int index);

protected:
  world::World& world;

  bool resendBuildingData = false;
  void sendBuildingData();

  void paintRoadOnTiles(data::roads::Road& road, std::vector<GLfloat>& tiles);

  // Terrain
  GLuint shaderProgram;
  GLuint transformLoc, terrainPositionLoc, renderGridLoc, selectionLoc, selectionColorLoc, groundTextureLoc,
      roadTextureLoc;
  GLuint VBO, VAO, terrainPositionVBO;
  GLuint gridTexture, roadTexture;
  std::map<std::pair<int, int>, GLuint> chunks; // chunk -> VBO (roadTile)

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

  // Left menu
  int leftMenuActiveIcon = -1;
};
}

#endif
