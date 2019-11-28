#include "ShaderProgram.hpp"

namespace opengl {

uint32_t ShaderProgram::bound_id = 0;

void ShaderProgram::unbind() noexcept {
  glUseProgram(0);
  bound_id = 0;
}

ShaderProgram::ShaderProgram() noexcept : id(0) {
}

uint32_t ShaderProgram::get_id() const noexcept {
  return id;
}

void ShaderProgram::generate() noexcept {
  this->id = glCreateProgram();
}

void ShaderProgram::attach(const Shader& shader) const noexcept {
  glAttachShader(this->id, shader.get_id());
}

bool ShaderProgram::link() noexcept {
  glLinkProgram(this->id);
  int32_t success;
  glGetProgramiv(this->id, GL_LINK_STATUS, &success);

  read_uniforms();

  return success;
}

const std::vector<char> ShaderProgram::get_info_log() const noexcept {
  int32_t message_length;
  glGetProgramiv(this->id, GL_INFO_LOG_LENGTH, &message_length);

  std::vector<char> message(message_length);
  glGetProgramInfoLog(this->id, message_length, nullptr, message.data());
  return message;
}

[[nodiscard]] const Uniform& ShaderProgram::get_uniform(const std::string& name) const noexcept {
  return uniforms.at(name);
}

[[nodiscard]] int32_t ShaderProgram::get_uniform_loc(const char* name) const noexcept {
  return glGetUniformLocation(this->id, name);
}

[[nodiscard]] uint32_t ShaderProgram::get_uniform_block_index(const char* name) const noexcept {
  return glGetUniformBlockIndex(this->id, name);
}

void ShaderProgram::bind_uniform_block(const char* name, uint32_t binding_point) const noexcept {
  uint32_t uniform_block_index = get_uniform_block_index(name);
  glUniformBlockBinding(this->id, uniform_block_index, binding_point);
}

void ShaderProgram::use() const noexcept {
  glUseProgram(id);
  bound_id = id;
}

void ShaderProgram::delete_program() const noexcept {
  glDeleteProgram(this->id);
}

void ShaderProgram::read_uniforms() noexcept {
  int32_t uniform_count;
  glGetProgramInterfaceiv(id, GL_UNIFORM, GL_ACTIVE_RESOURCES, &uniform_count);

  const unsigned int to_read_size = 5;
  std::array<uint32_t, to_read_size> to_read = {GL_BLOCK_INDEX, GL_TYPE, GL_NAME_LENGTH, GL_LOCATION, GL_OFFSET};
  for (int uniform = 0; uniform < uniform_count; uniform++) {
    // Fetch data
    std::array<int32_t, to_read_size> values;
    glGetProgramResourceiv(id, GL_UNIFORM, uniform, to_read_size, to_read.data(), to_read_size, nullptr, values.data());

    // Currently skip uniforms from uniform blocks
    if (values[0] > -1) {
      continue;
    }

    // Read uniform name
    std::string name(values[2], 0);
    glGetProgramResourceName(id, GL_UNIFORM, uniform, name.size(), nullptr, name.data());
    name.pop_back();

    Uniform to_insert = Uniform(Type(values[1]), name, values[3], id);
    uniforms.emplace(to_insert.get_name(), to_insert);
  }
}
}