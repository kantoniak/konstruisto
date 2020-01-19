#include "SceneRenderer.hpp"

namespace rendering {

SceneRenderer::SceneRenderer(engine::Engine& engine) noexcept : engine(engine) {
}

bool SceneRenderer::set_up(opengl::ShaderManager& shader_manager) noexcept {
  return set_up_shaders(shader_manager) && set_up_vaos();
}

bool SceneRenderer::set_up_shaders(opengl::ShaderManager& shader_manager) noexcept {
  std::optional<opengl::Shader> vert_shader =
      shader_manager.compileShader(opengl::Shader::VERTEX_SHADER, "assets/shaders/scene_renderer.glsl.vert");
  if (!vert_shader.has_value()) {
    return false;
  }

  std::optional<opengl::Shader> vert_shader_instancing =
      shader_manager.compileShader(opengl::Shader::VERTEX_SHADER, "assets/shaders/scene_renderer_instancing.glsl.vert");
  if (!vert_shader.has_value()) {
    vert_shader.value().delete_shader();
    return false;
  }

  std::optional<opengl::Shader> frag_shader =
      shader_manager.compileShader(opengl::Shader::FRAGMENT_SHADER, "assets/shaders/scene_renderer.glsl.frag");
  if (!frag_shader.has_value()) {
    vert_shader.value().delete_shader();
    vert_shader_instancing.value().delete_shader();
    return false;
  }

  // Shader program for single draws
  const std::array shaders = {vert_shader.value(), frag_shader.value()};
  std::optional<opengl::ShaderProgram> shader_program = shader_manager.linkProgram(shaders);
  if (!shader_program.has_value()) {
    vert_shader.value().delete_shader();
    vert_shader_instancing.value().delete_shader();
    frag_shader.value().delete_shader();
    return false;
  }
  this->shader_program = shader_program.value();

  // Shader program for instancing
  const std::array shaders_instancing = {vert_shader_instancing.value(), frag_shader.value()};
  std::optional<opengl::ShaderProgram> shader_program_instancing = shader_manager.linkProgram(shaders_instancing);
  if (!shader_program_instancing.has_value()) {
    this->shader_program.delete_program();
    vert_shader.value().delete_shader();
    vert_shader_instancing.value().delete_shader();
    frag_shader.value().delete_shader();
    return false;
  }
  this->shader_program_instancing = shader_program_instancing.value();

  // Delete unused shaders
  vert_shader.value().delete_shader();
  vert_shader_instancing.value().delete_shader();
  frag_shader.value().delete_shader();

  return true;
}

bool SceneRenderer::set_up_vaos() noexcept {
  // Generate everything
  vao_single_draw.generate();
  vao_instancing.generate();
  vbo.generate();
  normals_vbo.generate();
  instancing_transform_vbo.generate();
  ebo.generate();

  return set_up_single_draw_vao() && set_up_instancing_vao();
}

bool SceneRenderer::set_up_single_draw_vao() noexcept {
  // Single draw VAO
  vao_single_draw.bind();

  // Vertex buffer
  vbo.bind();
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)nullptr);

  // Normal buffer
  normals_vbo.bind();
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)nullptr);

  // Element array buffer
  ebo.bind();

  opengl::VertexArray::unbind();
  return true;
}

bool SceneRenderer::set_up_instancing_vao() noexcept {
  // Single draw VAO
  vao_instancing.bind();

  // Vertex buffer
  vbo.bind();
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)nullptr);

  // Normal buffer
  normals_vbo.bind();
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)nullptr);

  // Array buffer for model transforms
  // Using 4 times glm::vec4 because vec4 is the maximum vertex attribute can hold.
  instancing_transform_vbo.bind();
  glEnableVertexAttribArray(2);
  glEnableVertexAttribArray(3);
  glEnableVertexAttribArray(4);
  glEnableVertexAttribArray(5);

  GLsizei vec4_size = sizeof(glm::vec4);
  glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 4 * vec4_size, (void*)nullptr);
  glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4_size, (void*)(std::uintptr_t)(1 * vec4_size));
  glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4_size, (void*)(std::uintptr_t)(2 * vec4_size));
  glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4_size, (void*)(std::uintptr_t)(3 * vec4_size));

  glVertexAttribDivisor(2, 1);
  glVertexAttribDivisor(3, 1);
  glVertexAttribDivisor(4, 1);
  glVertexAttribDivisor(5, 1);

  // Element array buffer
  ebo.bind();

  opengl::VertexArray::unbind();
  return true;
}

void SceneRenderer::switch_to_single_draw(unicref u_material_ambient, unicref u_material_diffuse,
                                          unicref u_material_specular, unicref u_material_shininess,
                                          unicref u_model) noexcept {
  shader_program.use();
  vao_single_draw.bind();

  u_material_ambient = std::ref(shader_program.get_uniform("material.ambient"));
  u_material_diffuse = std::ref(shader_program.get_uniform("material.diffuse"));
  u_material_specular = std::ref(shader_program.get_uniform("material.specular"));
  u_material_shininess = std::ref(shader_program.get_uniform("material.shininess"));
  u_model = std::ref(shader_program.get_uniform("model"));
}

void SceneRenderer::switch_to_instanced_draw(unicref u_material_ambient, unicref u_material_diffuse,
                                             unicref u_material_specular, unicref u_material_shininess) noexcept {
  shader_program_instancing.use();
  vao_instancing.bind();

  u_material_ambient = std::ref(shader_program_instancing.get_uniform("material.ambient"));
  u_material_diffuse = std::ref(shader_program_instancing.get_uniform("material.diffuse"));
  u_material_specular = std::ref(shader_program_instancing.get_uniform("material.specular"));
  u_material_shininess = std::ref(shader_program_instancing.get_uniform("material.shininess"));
}

void SceneRenderer::reset_instancing_transform_vbo() noexcept {
  instancing_transform_vbo.bind();
  instancing_transform_vbo.orphan();
}

void SceneRenderer::submit_static_models(ModelManager& model_manager) noexcept {
  vao_single_draw.bind();

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

    CommandSet<MainCommandSetKey>::Element packet;
    packet.type = Command::Type::DRAW_SINGLE;
    packet.key = create_main_queue_key(packet.type, material);
    packet.data = DrawSingleCommand::create(object.get_transform(), material, mesh);
    main_queue.push(packet);
  }
}

void SceneRenderer::draw_instanced(const Model& model, const std::vector<glm::mat4>& transforms) noexcept {
  const size_t mesh_count = model.get_meshes().size();
  for (size_t i = 0; i < mesh_count; i++) {
    const Mesh& mesh = model.get_meshes()[i];
    const Material& material = model.get_materials()[i];

    CommandSet<MainCommandSetKey>::Element packet;
    packet.type = Command::Type::DRAW_INSTANCED;
    packet.key = create_main_queue_key(packet.type, material);
    packet.data = DrawInstancedCommand::create(transforms, material, mesh);
    main_queue.push(packet);
  }
}

void SceneRenderer::flush() noexcept {
  main_queue.sort();

  bool first = true;
  MainCommandSetKey previous_key;

  auto u_material_ambient = std::ref(shader_program.get_uniform("material.ambient"));
  auto u_material_diffuse = std::ref(shader_program.get_uniform("material.diffuse"));
  auto u_material_specular = std::ref(shader_program.get_uniform("material.specular"));
  auto u_material_shininess = std::ref(shader_program.get_uniform("material.shininess"));
  auto u_model = std::ref(shader_program.get_uniform("model"));

  while (!main_queue.empty()) {
    const CommandSet<MainCommandSetKey>::Element& packet = main_queue.next();

    switch (packet.type) {
    case Command::Type::DRAW_SINGLE: {
      auto* command_ptr = static_cast<DrawSingleCommand*>(packet.data);
      DrawSingleCommand& command = *command_ptr;

      if (first || previous_key != packet.key) {
        if (first || key_type_changed(previous_key, packet.key)) {
          switch_to_single_draw(u_material_ambient, u_material_diffuse, u_material_specular, u_material_shininess,
                                u_model);
        }

        // Material differs
        u_material_ambient.get().submit(command.ambient);
        u_material_diffuse.get().submit(command.diffuse);
        u_material_specular.get().submit(command.specular);
        u_material_shininess.get().submit(command.shininess * 128);
      }

      u_model.get().submit(command.model_transform);
      glDrawElementsBaseVertex(GL_TRIANGLES, command.vertex_count, GL_UNSIGNED_INT,
                               (GLvoid*)(command.ebo_offset * sizeof(uint32_t)), command.vbo_offset);

      delete command_ptr;
      break;
    }

    case Command::Type::DRAW_INSTANCED: {
      auto* command_ptr = static_cast<DrawInstancedCommand*>(packet.data);
      DrawInstancedCommand& command = *command_ptr;

      if (first || previous_key != packet.key) {
        if (first || key_type_changed(previous_key, packet.key)) {
          switch_to_instanced_draw(u_material_ambient, u_material_diffuse, u_material_specular, u_material_shininess);
        }

        // Material differs
        u_material_ambient.get().submit(command.ambient);
        u_material_diffuse.get().submit(command.diffuse);
        u_material_specular.get().submit(command.specular);
        u_material_shininess.get().submit(command.shininess * 128);
      }

      const GLsizei instance_count = command.model_transforms.size();
      reset_instancing_transform_vbo();
      instancing_transform_vbo.buffer_data(command.model_transforms, GL_DYNAMIC_DRAW);
      glDrawElementsInstancedBaseVertex(GL_TRIANGLES, command.vertex_count, GL_UNSIGNED_INT,
                                        (GLvoid*)(command.ebo_offset * sizeof(uint32_t)), instance_count,
                                        command.vbo_offset);

      delete command_ptr;
      break;
    }
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
  shader_program_instancing.delete_program();

  // Buffers
  opengl::ElementArrayBuffer::unbind();
  opengl::ArrayBuffer::unbind();
  opengl::VertexArray::unbind();

  ebo.delete_buffer();
  instancing_transform_vbo.delete_buffer();
  normals_vbo.delete_buffer();
  vbo.delete_buffer();
  vao_instancing.delete_vertex_array();
  vao_single_draw.delete_vertex_array();
}

SceneRenderer::MainCommandSetKey SceneRenderer::create_main_queue_key(Command::Type type,
                                                                      const Material& material) noexcept {
  static const SceneRenderer::MainCommandSetKey bytes_8 = 0b11111111;
  uint16_t result = 0;
  result |= static_cast<uint16_t>(type) & bytes_8;
  result <<= 8;
  result |= material.get_index() & bytes_8;
  return result;
}

bool SceneRenderer::key_type_changed(SceneRenderer::MainCommandSetKey previous,
                                     SceneRenderer::MainCommandSetKey current) noexcept {
  static const uint16_t leftmost_8 = 0b1111111100000000;
  return (previous & leftmost_8) != (current & leftmost_8);
}
}