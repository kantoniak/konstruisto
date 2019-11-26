#include "ShaderProgram.hpp"

namespace opengl {

void ShaderProgram::unbind() noexcept {
  glUseProgram(0);
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

bool ShaderProgram::link() const noexcept {
  glLinkProgram(this->id);
  int32_t success;
  glGetProgramiv(this->id, GL_LINK_STATUS, &success);
  return success;
}

const std::vector<char> ShaderProgram::get_info_log() const noexcept {
  int32_t message_length;
  glGetProgramiv(this->id, GL_INFO_LOG_LENGTH, &message_length);

  std::vector<char> message(message_length);
  glGetProgramInfoLog(this->id, message_length, nullptr, message.data());
  return message;
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
  glUseProgram(this->id);
}

void ShaderProgram::delete_program() const noexcept {
  glDeleteProgram(this->id);
}
}