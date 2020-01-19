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
  vbo.buffer_data(model_manager.get_vertices(), GL_STATIC_DRAW);

  normals_vbo.bind();
  normals_vbo.buffer_data(model_manager.get_normals(), GL_STATIC_DRAW);

  ebo.bind();
  ebo.buffer_data(model_manager.get_indices(), GL_STATIC_DRAW);

  opengl::VertexArray::unbind();
}

void SceneRenderer::draw_single(const Object& object) noexcept {
  const size_t mesh_count = object.get_model().get_meshes().size();
  for (size_t i = 0; i < mesh_count; i++) {
    const Mesh& mesh = object.get_model().get_meshes()[i];
    const Material& material = object.get_model().get_materials()[i];

    CommandSet<int32_t>::Element packet;
    packet.type = DRAW_SINGLE;
    packet.key = DrawSingleCommand::create_key(material);
    packet.data = DrawSingleCommand::create(object.get_transform(), material, mesh);
    main_queue.push(packet);
  }
}

void SceneRenderer::flush() noexcept {
  main_queue.sort();

  shader_program.use();
  vao.bind();
  vbo.bind();

  bool first = true;
  int32_t previous_key;

  const opengl::Uniform& u_material_ambient = shader_program.get_uniform("material.ambient");
  const opengl::Uniform& u_material_diffuse = shader_program.get_uniform("material.diffuse");
  const opengl::Uniform& u_material_specular = shader_program.get_uniform("material.specular");
  const opengl::Uniform& u_material_shininess = shader_program.get_uniform("material.shininess");
  const opengl::Uniform& u_model = shader_program.get_uniform("model");

  while (!main_queue.empty()) {
    const CommandSet<int32_t>::Element& packet = main_queue.next();

    switch (packet.type) {
    case DRAW_SINGLE:
      DrawSingleCommand* command_ptr = static_cast<DrawSingleCommand*>(packet.data);
      DrawSingleCommand& command = *command_ptr;

      if (first || previous_key != packet.key) {
        // Only material differs
        u_material_ambient.submit(command.ambient);
        u_material_diffuse.submit(command.diffuse);
        u_material_specular.submit(command.specular);
        u_material_shininess.submit(command.shininess * 128);
      }

      u_model.submit(command.model_transform);
      glDrawElementsBaseVertex(GL_TRIANGLES, command.vertex_count, GL_UNSIGNED_INT,
                               (GLvoid*)(command.ebo_offset * sizeof(uint32_t)), command.vbo_offset);

      delete command_ptr;
      break;
    }

    first = false;
    previous_key = packet.key;
    main_queue.pop();
  }

  main_queue.clear();
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