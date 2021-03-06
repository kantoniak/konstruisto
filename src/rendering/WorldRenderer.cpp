#include "WorldRenderer.hpp"

#include "../data/Chunk.hpp"
#include "../data/PowerLineCable.hpp"
#include "../data/PowerLinePole.hpp"
#include "../input/Tool.hpp"

namespace rendering {
WorldRenderer::WorldRenderer(engine::Engine& engine, world::World& world)
    : Renderer(engine), world(world), model_manager(/* rewrite_indices */ false),
      assimp_loader(model_manager, engine.getLogger()), renderer(engine) {
  clearColor = glm::vec3(89, 159, 209) / 255.f;
}

bool WorldRenderer::init() {
  Renderer::init();

  if (!setupShaders() || !setup_ubos() || !setupTextures()) {
    return false;
  }

  if (!setupTerrain() || !setupBuildings()) {
    return false;
  }

  if (!renderer.set_up(shader_manager) || !set_up_models()) {
    return false;
  }

  glClearColor(clearColor.x, clearColor.y, clearColor.z, 1.f);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);

  return true;
}

bool WorldRenderer::setupShaders() {
  using namespace opengl;

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
    std::optional<ShaderProgram> shader_program = shader_manager.linkProgram(shaders);
    if (!shader_program.has_value()) {
      for (auto shader : shaders) {
        shader.delete_shader();
      }
      return false;
    }
    this->terrain_shader_prog = shader_program.value();

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
    std::optional<ShaderProgram> shader_program = shader_manager.linkProgram(shaders);
    if (!shader_program.has_value()) {
      this->cleanup_shaders();
      for (auto shader : shaders) {
        shader.delete_shader();
      }
      return false;
    }
    this->building_shader_prog = shader_program.value();

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
    std::optional<ShaderProgram> shader_program = shader_manager.linkProgram(shaders);
    if (!shader_program.has_value()) {
      this->cleanup_shaders();
      for (auto shader : shaders) {
        shader.delete_shader();
      }
      return false;
    }
    this->building_normals_shader_prog = shader_program.value();

    for (auto shader : shaders) {
      shader.delete_shader();
    }
  }

  return true;
}

bool WorldRenderer::setup_ubos() {
  camera_ubo.generate();
  camera_ubo.bind_to_binding_point(
      1); // TODO(kantoniak): Read index from ShaderManager.get_uniform_block.get_binding_point
  return true;
}

bool WorldRenderer::setupTextures() {
  {
    int width, height;
    unsigned char* pixels = stbi_load("assets/textures/grid.png", &width, &height, nullptr, STBI_rgb_alpha);

    // Texture of the ground
    glGenTextures(1, &grid_texture);
    glBindTexture(GL_TEXTURE_2D, grid_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    if (GLAD_GL_EXT_texture_filter_anisotropic) {
      glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16);
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(pixels);
  }

  {
    int width, height;
    unsigned char* pixels = stbi_load("assets/textures/road.png", &width, &height, nullptr, STBI_rgb_alpha);

    // Texture of the ground
    // TODO(kantoniak): Better system for atlas sizing
    glGenTextures(1, &road_texture);
    glBindTexture(GL_TEXTURE_2D_ARRAY, road_texture);
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
  terrain_vao.generate();
  markTileDataForUpdate();
  return true;
}

bool WorldRenderer::setupBuildings() {
  std::array<GLfloat, 8 * 2 * 3> building = {
      // 8x2
      0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
      0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, // Reset
      1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f};

  buildings_vao.generate();
  buildings_vao.bind();

  // Vertex buffer
  building_mesh_vbo.generate();
  building_mesh_vbo.bind();
  building_mesh_vbo.buffer_data(building, GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)nullptr);

  // Position+size buffer
  building_positions_vbo.generate();
  building_positions_vbo.bind();

  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)nullptr);
  glVertexAttribDivisor(1, 1);

  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
  glVertexAttribDivisor(2, 1);

  ArrayBuffer::unbind();
  VertexArray::unbind();

  markBuildingDataForUpdate();
  return true;
}

bool WorldRenderer::set_up_models() {
  if (!assimp_loader.load_model_from_file("assets/models/tree.obj", "tree-green") ||
      !assimp_loader.load_model_from_file("assets/models/tree-2.obj", "tree-2") ||
      !assimp_loader.load_model_from_file("assets/models/power-line-pole.obj") ||
      !assimp_loader.load_model_from_file("assets/models/power-line-cable.obj") ||
      !assimp_loader.load_model_from_file("assets/models/debug-aabb.obj") ||
      !assimp_loader.load_model_from_file("assets/models/debug-cylinder.obj")) {
    return false;
  }

  // Register orange tree
  const Model& tree_model_green = model_manager.get_model("tree-green");
  Model& tree_model_orange = model_manager.register_model("tree-orange");
  {
    const Material& orange_branches = model_manager.get_material("tree_orange_branches");
    const size_t mesh_count = tree_model_green.get_meshes().size();
    for (size_t i = 0; i < mesh_count; i++) {
      const Mesh& orig_mesh = tree_model_green.get_meshes()[i];
      const Material& orig_material = tree_model_green.get_materials()[i];
      if (orig_material.get_name() == "tree_green_branches") {
        tree_model_orange.add_mesh(orig_mesh, orange_branches);
      } else {
        tree_model_orange.add_mesh(orig_mesh, orig_material);
      }
    }
  }

  // Register invalid variants
  register_invalid_model_variant("power-line-pole");
  register_invalid_model_variant("power-line-cable");

  renderer.submit_static_models(model_manager);
  return true;
}

void WorldRenderer::register_invalid_model_variant(const std::string& model_name) noexcept {
  const Model& model = model_manager.get_model(model_name);
  Model& model_invalid = model_manager.register_model(model.get_name() + "-invalid");
  const size_t mesh_count = model.get_meshes().size();
  for (size_t i = 0; i < mesh_count; i++) {
    const Mesh& mesh = model.get_meshes()[i];
    const Material& material = model.get_materials()[i];
    std::string invalid_material_name = material.get_name() + "_invalid";
    const Material& material_invalid = model_manager.get_material(invalid_material_name);
    model_invalid.add_mesh(mesh, material_invalid);
  }
}

void WorldRenderer::cleanup() {
  glUseProgram(0);
  terrain_vao.delete_vertex_array();

  for (auto& chunk : chunk_to_vbo) {
    chunk.second.delete_buffer();
  }

  buildings_vao.delete_vertex_array();
  building_mesh_vbo.delete_buffer();
  building_positions_vbo.delete_buffer();

  renderer.clean_up();
  cleanup_ubos();
  cleanup_shaders();
  Renderer::cleanup();
}

void WorldRenderer::cleanup_ubos() {
  UniformBuffer::unbind();
  camera_ubo.delete_buffer();
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

void WorldRenderer::renderWorld(const input::Selection& selection, const std::shared_ptr<input::Brush>& brush,
                                const std::shared_ptr<input::Tool>& tool) {

  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);

  glFrontFace(GL_CCW);
  glCullFace(GL_BACK);

  // UBO
  const opengl::UniformBlock camera_ub = shader_manager.get_uniform_block("Camera");
  std::vector<uint8_t> camera_buffer_data(camera_ub.get_size(), 0);

  const glm::mat4 vp = world.getCamera().getViewProjectionMatrix();
  camera_ub.get_uniform("Camera.vp").memcpy(camera_buffer_data, vp);
  camera_ub.get_uniform("Camera.position").memcpy(camera_buffer_data, world.getCamera().getPosition());

  camera_ubo.bind();
  camera_ubo.buffer_data(camera_buffer_data, GL_DYNAMIC_DRAW);
  UniformBuffer::unbind();

  // Terrain
  if (resendTileData) {
    sendTileData();
    resendTileData = false;
  }

  terrain_shader_prog.use();
  terrain_vao.bind();
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, grid_texture);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D_ARRAY, road_texture);
  terrain_shader_prog.submit("groundTexture", 0);
  terrain_shader_prog.submit("roadTexture", 1);

  glm::ivec4 selection_coords(selection.getFrom().x, selection.getFrom().y, selection.getTo().x + 1,
                              selection.getTo().y + 1);

  glm::vec4 selection_color = selection.getColor();
  if (!engine.getSettings().rendering.renderSelection) {
    selection_color.w = 0;
  }

  terrain_shader_prog.submit("renderGrid", engine.getSettings().world.showGrid);
  terrain_shader_prog.submit("selection", selection_coords);
  terrain_shader_prog.submit("selectionColor", selection_color);

  // Brush
  if (brush_dirty) {
    update_brush_appearance(brush);
  }
  if (brush) {
    terrain_shader_prog.submit("brush_center", static_cast<glm::vec2>(brush->get_center().getGlobal()));
  }

  for (data::Chunk* chunk : world.getMap().getChunks()) {
    const auto chunk_pos = std::make_pair(chunk->getPosition().x, chunk->getPosition().y);
    chunk_to_vbo[chunk_pos].bind();
    glDrawArrays(GL_TRIANGLES, 0, data::Chunk::SIDE_LENGTH * data::Chunk::SIDE_LENGTH * 2 * 3);
  }

  // Buildings
  if (resendBuildingData) {
    sendBuildingData();
    resendBuildingData = false;
  }

  if (world.getMap().get_building_count() > 0) {
    building_shader_prog.use();
    buildings_vao.bind();
    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 16, world.getMap().get_building_count());
  }

  // Trees
  {
    using data::Tree;

    const std::unordered_map<Tree::Type, const Model&> type_to_model = {
        {Tree::Type::GREEN, model_manager.get_model("tree-green")},
        {Tree::Type::ORANGE, model_manager.get_model("tree-orange")},
        {Tree::Type::MODEL2, model_manager.get_model("tree-2")}};

    for (Tree::Type type : Tree::TYPES) {
      std::vector<glm::mat4> transforms;
      for (data::Chunk* chunk : world.getMap().getChunks()) {
        transforms.reserve(transforms.size() + chunk->get_trees().get_count(type));
        for (const auto& tree_ptr : chunk->get_trees().get_trees(type)) {
          transforms.push_back(tree_ptr->get_transform());
        }
      }
      if (transforms.size() > 0) {
        renderer.draw_instanced(type_to_model.at(type), transforms);
      }
    }
  }

  // Electricity poles
  {
    const Model& power_line_pole_model = model_manager.get_model("power-line-pole");

    std::vector<glm::mat4> transforms(world.getMap().get_electricity_grid().get_pole_count());
    for (const auto& pole_ptr : world.getMap().get_electricity_grid().get_poles()) {
      transforms.push_back(pole_ptr->get_transform());
    }

    if (transforms.size() > 0) {
      renderer.draw_instanced(power_line_pole_model, transforms);
    }
  }

  // Electricity cables
  {
    const Model& model = model_manager.get_model("power-line-cable");

    std::vector<glm::mat4> transforms(world.getMap().get_electricity_grid().get_cable_count());
    for (const auto& cable_ptr : world.getMap().get_electricity_grid().get_cables()) {
      // TODO(kantoniak): Update cable transform when pole changes
      cable_ptr->update_transform();
      const auto& cable_transforms = cable_ptr->get_transforms();
      transforms.insert(transforms.end(), cable_transforms.begin(), cable_transforms.end());
    }

    if (transforms.size() > 0) {
      renderer.draw_instanced(model, transforms);
    }
  }

  // Tools
  if (tool) {
    tool->render(renderer);
  }
}

void WorldRenderer::renderWorldDebug() {
  std::unordered_map<geometry::Collidable::layer_key, std::unordered_set<geometry::Collidable::ptr>> objects;
  const auto& collidables = world.get_collision_space().get_collidables();
  for (auto ptr : collidables) {
    auto it = objects.find(ptr->get_layer_key());

    if (it == objects.end()) {
      objects[ptr->get_layer_key()] = std::unordered_set<geometry::Collidable::ptr>();
    }

     objects[ptr->get_layer_key()].insert(ptr);
  }


  const glm::mat4 identity(1.f);
  std::for_each(objects.begin(), objects.end(), [this, &identity](const auto& pair) {
    std::vector<glm::mat4> transforms;
    const Model& model = this->model_manager.get_model("debug-cylinder");

    for (const auto& collidable : pair.second) {
      switch (pair.first) {
        case data::CollisionLayer::TREES: {
          const glm::vec2 translation = collidable->get_transform();
          transforms.push_back(glm::translate(identity, glm::vec3(translation.x, 0, translation.y)));
          break;
        }
      }
    }

    this->renderer.draw_instanced(model, transforms);
  });
}

void WorldRenderer::flushWorld() {
  renderer.flush();
}

void WorldRenderer::renderDebug() {
  if (engine.getSettings().rendering.renderNormals) {
    building_normals_shader_prog.use();
    buildings_vao.bind();
    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 16, world.getMap().get_building_count());
    VertexArray::unbind();
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
    const unsigned short option_count = 7;
    const unsigned short button_side = 3 * UI::ICON_SIDE;
    int x = 0;
    int y = viewport.y / 2 - option_count * button_side / 2.f;

    nvgBeginPath(context);
    nvgRect(context, x, y, button_side, button_side * option_count);
    nvgFillColor(context, engine.getUI().getBackgroundColor());
    nvgFill(context);

    if (leftMenuActiveIcon >= 0) {
      nvgBeginPath(context);
      nvgRect(context, x, y + button_side * leftMenuActiveIcon, button_side, button_side);
      nvgFillColor(context, iconBackgroundColor);
      nvgFill(context);
    }

    x = (button_side - UI::ICON_SIDE_24) / 2;
    y += x;

    engine.getUI().renderIcon(UI::ICON_BUILDING, x, y, UI::ICON_SIDE_24);
    y += button_side;
    engine.getUI().renderIcon(UI::ICON_ZONES, x, y, UI::ICON_SIDE_24);
    y += button_side;
    engine.getUI().renderIcon(UI::ICON_ELECTRICITY, x, y, UI::ICON_SIDE_24);
    y += button_side;
    engine.getUI().renderIcon(UI::ICON_ROAD, x, y, UI::ICON_SIDE_24);
    y += button_side;
    engine.getUI().renderIcon(UI::ICON_NATURE, x, y, UI::ICON_SIDE_24);
    y += button_side;
    engine.getUI().renderIcon(UI::ICON_BULDOZER, x, y, UI::ICON_SIDE_24);
    y += button_side;
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

const rendering::ModelManager& WorldRenderer::get_model_manager() const noexcept {
  return model_manager;
}

void WorldRenderer::mark_brush_dirty() noexcept {
  brush_dirty = true;
}

void WorldRenderer::update_brush_appearance(const std::shared_ptr<input::Brush>& brush) noexcept {
  if (!brush) {
    terrain_shader_prog.submit("brush_radius", 0.f);
    brush_dirty = false;
    return;
  }

  terrain_shader_prog.submit("brush_radius", brush->get_radius());
  terrain_shader_prog.submit("brush_border_width", brush->get_border_width());

  if (brush->is_active()) {
    terrain_shader_prog.submit("brush_fill_color", brush->get_fill_color_active());
    terrain_shader_prog.submit("brush_border_color", brush->get_border_color_active());
  } else {
    terrain_shader_prog.submit("brush_fill_color", brush->get_fill_color_base());
    terrain_shader_prog.submit("brush_border_color", brush->get_border_color_base());
  }

  brush_dirty = false;
}

void WorldRenderer::sendBuildingData() {
  const unsigned int buildingCount = world.getMap().get_building_count();
  if (buildingCount < 1) {
    return;
  }

  std::vector<glm::vec3> buildingPositions;
  buildingPositions.reserve(2 * buildingCount);

  constexpr float buildingMargin = 0.2f;
  for (data::Chunk* chunk : world.getMap().getChunks()) {
    for (const data::Building::ptr& building : chunk->get_buildings()) {
      buildingPositions.emplace_back(building->x + buildingMargin, 0, building->y + buildingMargin);
      buildingPositions.emplace_back(building->width - 2 * buildingMargin, building->level,
                                     building->length - 2 * buildingMargin);
    }
  }

  buildings_vao.bind();
  building_positions_vbo.bind();
  building_positions_vbo.buffer_data(buildingPositions, GL_STATIC_DRAW);
  ArrayBuffer::unbind();
  VertexArray::unbind();
}

void WorldRenderer::sendTileData() {
  terrain_vao.bind();

  constexpr unsigned long verticesCount = data::Chunk::SIDE_LENGTH * data::Chunk::SIDE_LENGTH * 2 * 3;
  const std::vector<glm::vec3> fieldBase{glm::vec3(1, 0, 0), glm::vec3(0, 0, 0), glm::vec3(1, 0, 1),
                                         glm::vec3(1, 0, 1), glm::vec3(0, 0, 0), glm::vec3(0, 0, 1)};

  // Generate buffers
  std::vector<glm::vec3> positions;
  positions.resize(verticesCount);

  std::vector<GLfloat> tiles;
  tiles.resize(verticesCount);

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
    if (chunk_to_vbo.find(key) == chunk_to_vbo.end()) {
      opengl::ArrayBuffer vbo;
      vbo.generate();
      chunk_to_vbo.emplace(key, vbo);
    }
    chunk_to_vbo[key].bind();
    chunk_to_vbo[key].buffer_data(toBuffer, GL_STATIC_DRAW);
  }

  // Lazy VAO setup because there was no bound buffer before
  if (!terrain_vao_pointers_setup) {
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)nullptr);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));

    terrain_vao_pointers_setup = true;
  }

  ArrayBuffer::unbind();
  VertexArray::unbind();
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