#ifndef RENDERING_WORLDRENDERER_HPP
#define RENDERING_WORLDRENDERER_HPP

#include <algorithm>
#include <map>
#include <string>
#include <vector>
#include <unordered_map>

#include <nanovg.h>

#include <glad/gl.h>
#include <glm/ext.hpp>
#include <glm/glm.hpp>

#include "../data/CollisionLayer.hpp"
#include "../data/RoadGraph.hpp"
#include "../engine/DebugInfo.hpp"
#include "../engine/Engine.hpp"
#include "../input/Brush.hpp"
#include "../input/Selection.hpp"
#include "../input/WindowHandler.hpp"
#include "../opengl/ArrayBuffer.hpp"
#include "../opengl/ElementArrayBuffer.hpp"
#include "../opengl/Shader.hpp"
#include "../opengl/ShaderManager.hpp"
#include "../opengl/ShaderProgram.hpp"
#include "../opengl/UniformBuffer.hpp"
#include "../opengl/VertexArray.hpp"
#include "../settings.hpp"
#include "../world/World.hpp"
#include "AssimpLoader.hpp"
#include "Material.hpp"
#include "ModelManager.hpp"
#include "Object.hpp"
#include "Renderer.hpp"
#include "SceneRenderer.hpp"
#include "UI.hpp"
#include "stb_image.h"

namespace input {
class Tool;
}

namespace rendering {

class WorldRenderer : public Renderer {

public:
  WorldRenderer(engine::Engine& engine, world::World& world);

  bool init() override;
  bool setupShaders();
  bool setup_ubos();
  bool setupTextures();
  bool setupTerrain();
  bool setupBuildings();

  void cleanup() override;
  void cleanup_ubos();
  void cleanup_shaders();

  void markBuildingDataForUpdate();
  void markTileDataForUpdate();
  void mark_brush_dirty() noexcept;

  void renderWorld(const input::Selection& selection, const std::shared_ptr<input::Brush>& brush,
                   const std::shared_ptr<input::Tool>& tool);
  void renderWorldDebug();
  void flushWorld();
  void renderDebug();

  // TODO(kantoniak): Get rid of Renderer::renderUI() and  Renderer::renderDebugUI()
  void renderUI();
  void renderDebugUI();

  void setLeftMenuActiveIcon(int index);

  const rendering::ModelManager& get_model_manager() const noexcept;

protected:
  world::World& world;

  bool resendBuildingData = false;
  void sendBuildingData();

  bool resendTileData = false;
  void sendTileData();

  bool brush_dirty = true;
  void update_brush_appearance(const std::shared_ptr<input::Brush>& brush) noexcept;

  // Shared
  opengl::UniformBuffer camera_ubo;

  // Tiles
  const int ATLAS_SIDE = 10;
  [[nodiscard]] unsigned int getTile(int x, int y) const;
  void setTile(std::vector<GLfloat>& tiles, int x, int y, unsigned int tile);

  void paintOnTiles(const data::Chunk& chunk, const glm::ivec2& position, std::vector<GLfloat>& tiles);
  void paintLotOnTiles(const data::Lot& lot, const glm::ivec2& position, std::vector<GLfloat>& tiles);
  void paintRoadsOnTiles(const data::RoadGraph<data::Chunk::SIDE_LENGTH>& roads, std::vector<GLfloat>& tiles);

  // Terrain
  opengl::ShaderProgram terrain_shader_prog;
  opengl::VertexArray terrain_vao;
  bool terrain_vao_pointers_setup = false;
  uint32_t grid_texture;
  uint32_t road_texture;
  std::map<std::pair<int, int>, opengl::ArrayBuffer> chunk_to_vbo;

  // Renderer
  ModelManager model_manager;
  AssimpLoader assimp_loader;
  SceneRenderer renderer;
  bool set_up_models();
  void register_invalid_model_variant(const std::string& model_name) noexcept;

  // Buildings
  opengl::ShaderProgram building_shader_prog;
  opengl::VertexArray buildings_vao;
  opengl::ArrayBuffer building_mesh_vbo;
  opengl::ArrayBuffer building_positions_vbo;
  const unsigned int buildingsCount = 100;
  const unsigned int sideSize = 20;
  const unsigned int maxHeight = 6;

  // Buildings - normals
  opengl::ShaderProgram building_normals_shader_prog;

  // Trees
  std::map<std::pair<int, int>, std::vector<Object>> chunk_to_tree_objects;

  // Left menu
  int leftMenuActiveIcon = -1;
};
}

#endif
