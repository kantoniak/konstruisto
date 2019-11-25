#include "ShaderProgram.hpp"

namespace opengl {

ShaderProgram ShaderProgram::create() noexcept {
  return ShaderProgram(glCreateProgram());
}

ShaderProgram::ShaderProgram() noexcept : ShaderProgram(0) {
}

ShaderProgram::ShaderProgram(uint32_t id) noexcept : id(id) {
}

uint32_t ShaderProgram::get_id() const noexcept {
  return id;
}

void ShaderProgram::attach(const Shader& shader) {
  glAttachShader(this->id, shader.get_id());
}

bool ShaderProgram::link() {
  glLinkProgram(this->id);
  int32_t success;
  glGetProgramiv(this->id, GL_LINK_STATUS, &success);
  return success;
}

const std::vector<char> ShaderProgram::get_info_log() {
  int32_t message_length;
  glGetProgramiv(this->id, GL_INFO_LOG_LENGTH, &message_length);

  std::vector<char> message(message_length);
  glGetProgramInfoLog(this->id, message_length, nullptr, message.data());
  return message;
}

[[nodiscard]] int32_t ShaderProgram::get_uniform_loc(const char* name) {
  return glGetUniformLocation(this->id, name);
}

[[nodiscard]] uint32_t ShaderProgram::get_uniform_block_index(const char* name) {
  return glGetUniformBlockIndex(this->id, name);
}

void ShaderProgram::bind_uniform_block(const char* name, uint32_t binding_point) {
  uint32_t uniform_block_index = get_uniform_block_index(name);
  glUniformBlockBinding(this->id, uniform_block_index, binding_point);
}

void ShaderProgram::use() {
  glUseProgram(this->id);
}

void ShaderProgram::delete_program() {
  glDeleteProgram(this->id);
}
}