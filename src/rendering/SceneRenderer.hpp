#ifndef RENDERING_SCENERENDERER_HPP
#define RENDERING_SCENERENDERER_HPP

#include "../engine/Engine.hpp"
#include "../opengl/ArrayBuffer.hpp"
#include "../opengl/ElementArrayBuffer.hpp"
#include "../opengl/Shader.hpp"
#include "../opengl/ShaderManager.hpp"
#include "../opengl/ShaderProgram.hpp"
#include "../opengl/VertexArray.hpp"
#include "CommandSet.hpp"
#include "DrawInstancedCommand.hpp"
#include "DrawSingleCommand.hpp"
#include "ModelManager.hpp"
#include "Object.hpp"

namespace rendering {

class SceneRenderer {
  // Key is [command type (8 bytes) | material index (8 bytes)]
  using MainCommandSetKey = uint16_t;
  using unicref = std::reference_wrapper<const opengl::Uniform>&;

public:
  SceneRenderer(engine::Engine& engine) noexcept;

  bool set_up(opengl::ShaderManager& shader_manager) noexcept;
  void submit_static_models(ModelManager& model_manager) noexcept;

  void draw_single(const Object& to_render) noexcept;
  void draw_instanced(const Model& model, const std::vector<glm::mat4>& transforms) noexcept;

  void flush() noexcept;
  void clean_up() const noexcept;

protected:
  engine::Engine& engine;

  opengl::ShaderProgram shader_program;
  opengl::ShaderProgram shader_program_instancing;
  opengl::VertexArray vao_single_draw;
  opengl::VertexArray vao_instancing;
  opengl::ArrayBuffer vbo;
  opengl::ArrayBuffer normals_vbo;
  opengl::ArrayBuffer instancing_transform_vbo;
  opengl::ElementArrayBuffer ebo;

  CommandSet<MainCommandSetKey> main_queue;

  bool set_up_shaders(opengl::ShaderManager& shader_manager) noexcept;
  bool set_up_vaos() noexcept;
  bool set_up_single_draw_vao() noexcept;
  bool set_up_instancing_vao() noexcept;

  void switch_to_single_draw(unicref u_material_ambient, unicref u_material_diffuse, unicref u_material_specular,
                             unicref u_material_shininess, unicref u_model) noexcept;
  void switch_to_instanced_draw(unicref u_material_ambient, unicref u_material_diffuse, unicref u_material_specular,
                                unicref u_material_shininess) noexcept;
  void reset_instancing_transform_vbo() noexcept;

  static MainCommandSetKey create_main_queue_key(Command::Type type, const Material& material) noexcept;
  static bool key_type_changed(MainCommandSetKey previous, MainCommandSetKey current) noexcept;
};
}

#endif
