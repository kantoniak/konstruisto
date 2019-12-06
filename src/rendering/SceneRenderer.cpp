#include "SceneRenderer.hpp"

namespace rendering {

SceneRenderer::SceneRenderer(engine::Engine& engine) noexcept : engine(engine) {
}

bool SceneRenderer::set_up(opengl::ShaderManager& shader_manager) noexcept {
  return set_up_shaders(shader_manager) && set_up_buffers();
}

bool SceneRenderer::set_up_shaders(opengl::ShaderManager& shader_manager) noexcept {
  std::optional<opengl::Shader> vert_shader =
      shader_manager.compileShader(opengl::Shader::VERTEX_SHADER, "assets/shaders/scene_renderer.glsl.vert");
  if (!vert_shader.has_value()) {
    return false;
  }

  std::optional<opengl::Shader> frag_shader =
      shader_manager.compileShader(opengl::Shader::FRAGMENT_SHADER, "assets/shaders/scene_renderer.glsl.frag");
  if (!frag_shader.has_value()) {
    vert_shader.value().delete_shader();
    return false;
  }

  const std::array shaders = {vert_shader.value(), frag_shader.value()};
  std::optional<opengl::ShaderProgram> shader_program = shader_manager.linkProgram(shaders);
  if (!shader_program.has_value()) {
    for (auto shader : shaders) {
      shader.delete_shader();
    }
    return false;
  }
  this->shader_program = shader_program.value();

  for (auto shader : shaders) {
    shader.delete_shader();
  }

  return true;
}

bool SceneRenderer::set_up_buffers() noexcept {
  // VAO
  vao.generate();
  vao.bind();

  // Vertex buffer
  vbo.generate();
  vbo.bind();

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)nullptr);

  // Normal buffer
  normals_vbo.generate();
  normals_vbo.bind();

  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)nullptr);

  // Element array buffer
  ebo.generate();
  ebo.bind();

  opengl::VertexArray::unbind();
  return true;
}

void SceneRenderer::submit_static_models(ModelManager& model_manager) const noexcept {
  vao.bind();

  vbo.bind();
  glBufferDataVector(GL_ARRAY_BUFFER, model_manager.get_vertices(), GL_STATIC_DRAW);

  normals_vbo.bind();
  glBufferDataVector(GL_ARRAY_BUFFER, model_manager.get_normals(), GL_STATIC_DRAW);

  ebo.bind();
  glBufferDataVector(GL_ELEMENT_ARRAY_BUFFER, model_manager.get_indices(), GL_STATIC_DRAW);

  opengl::VertexArray::unbind();
}

void SceneRenderer::render(std::vector<std::reference_wrapper<Object>> to_render) const noexcept {
  vao.bind();
  shader_program.use();

  // glm::vec3 light_pos = glm::vec3(3) * (float)sin(engine.getDeltaSinceStart().count() / 2000.f);
  // shader_program.submit("light_pos", light_pos);

  vbo.bind();
  for (const auto& ref : to_render) {
    Object& object = ref.get();
    shader_program.submit("model", object.get_transform());
    for (auto const mesh : object.get_model().get_meshes()) {
      mesh.get_material().set_in(shader_program);
      glDrawElementsBaseVertex(GL_TRIANGLES, mesh.get_vertex_count(), GL_UNSIGNED_INT,
                               (GLvoid*)(mesh.get_ebo_offset() * sizeof(uint32_t)), mesh.get_vbo_offset());
    }
  }
}

void SceneRenderer::clean_up() const noexcept {

  // Shaders
  opengl::ShaderProgram::unbind();
  shader_program.delete_program();

  // Buffers
  opengl::ElementArrayBuffer::unbind();
  opengl::ArrayBuffer::unbind();
  opengl::VertexArray::unbind();

  ebo.delete_buffer();
  vbo.delete_buffer();
  normals_vbo.delete_buffer();
  vao.delete_vertex_array();
}
}