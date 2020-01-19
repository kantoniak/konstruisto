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
#include "DrawSingleCommand.hpp"
#include "ModelManager.hpp"
#include "Object.hpp"

namespace rendering {

class SceneRenderer {

public:
  SceneRenderer(engine::Engine& engine) noexcept;

  bool set_up(opengl::ShaderManager& shader_manager) noexcept;
  void submit_static_models(ModelManager& model_manager) noexcept;

  void draw_single(const Object& to_render) noexcept;
  void flush() noexcept;
  void clean_up() const noexcept;

protected:
  engine::Engine& engine;

  opengl::ShaderProgram shader_program;
  opengl::VertexArray vao;
  opengl::ArrayBuffer vbo;
  opengl::ArrayBuffer normals_vbo;
  opengl::ElementArrayBuffer ebo;

  CommandSet<int32_t> main_queue;

  bool set_up_shaders(opengl::ShaderManager& shader_manager) noexcept;
  bool set_up_buffers() noexcept;

};
}

#endif
