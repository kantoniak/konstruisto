#ifndef RENDERING_WORLDRENDERER_HPP
#define RENDERING_WORLDRENDERER_HPP

#include <map>
#include <string>
#include <vector>

#include <nanovg.h>

#include <glad/gl.h>
#include <glm/ext.hpp>
#include <glm/glm.hpp>

#include "../data/RoadGraph.hpp"
#include "../engine/DebugInfo.hpp"
#include "../engine/Engine.hpp"
#include "../input/Selection.hpp"
#include "../input/WindowHandler.hpp"
#include "../opengl/ArrayBuffer.hpp"
#include "../opengl/Shader.hpp"
#include "../opengl/ShaderProgram.hpp"
#include "../opengl/VertexArray.hpp"
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

  bool init() override;
  bool setupShaders();
  bool setupTextures();
  bool setupTerrain();
  bool setupBuildings();
  void cleanup() override;
  void cleanup_shaders();

  void markBuildingDataForUpdate();
  void markTileDataForUpdate();

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

  bool resendTileData = false;
  void sendTileData();

  // Tiles
  const int ATLAS_SIDE = 10;
  [[nodiscard]] unsigned int getTile(int x, int y) const;
  void setTile(std::vector<GLfloat>& tiles, int x, int y, unsigned int tile);

  void paintOnTiles(const data::Chunk& chunk, const glm::ivec2& position, std::vector<GLfloat>& tiles);
  void paintLotOnTiles(const data::Lot& lot, const glm::ivec2& position, std::vector<GLfloat>& tiles);
  void paintRoadsOnTiles(const data::RoadGraph<data::Chunk::SIDE_LENGTH>& roads, std::vector<GLfloat>& tiles);

  // Terrain
  opengl::ShaderProgram terrain_shader_prog;
  int32_t transform_loc;
  int32_t terrain_position_loc;
  int32_t render_grid_loc;
  int32_t selection_loc;
  int32_t selection_color_loc;
  int32_t ground_texture_loc;
  int32_t road_texture_loc;
  opengl::VertexArray terrain_vao;
  uint32_t grid_texture;
  uint32_t road_texture;
  std::map<std::pair<int, int>, opengl::ArrayBuffer> chunk_to_vbo;

  // Buildings
  opengl::ShaderProgram building_shader_prog;
  int32_t buildings_transform_loc;
  opengl::VertexArray buildings_vao;
  opengl::ArrayBuffer building_mesh_vbo;
  opengl::ArrayBuffer building_positions_vbo;
  const unsigned int buildingsCount = 100;
  const unsigned int sideSize = 20;
  const unsigned int maxHeight = 6;

  // Buildings - normals
  opengl::ShaderProgram building_normals_shader_prog;
  int32_t building_normals_transform_loc;

  // Left menu
  int leftMenuActiveIcon = -1;
};
}

#endif
