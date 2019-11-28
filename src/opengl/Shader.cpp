#include "Shader.hpp"

namespace opengl {

Shader Shader::create(ShaderType type) noexcept {
  return Shader(glCreateShader(type), type);
}

Shader::Shader(uint32_t id, Shader::ShaderType type) : id(id), type(type) {
}

uint32_t Shader::get_id() const noexcept {
  return id;
}

Shader::ShaderType Shader::get_type() const noexcept {
  return type;
}

bool Shader::compile(const std::string& source_string) const noexcept {
  const char* source = source_string.c_str();
  glShaderSource(this->id, 1, &source, nullptr);
  glCompileShader(this->id);

  int32_t success;
  glGetShaderiv(this->id, GL_COMPILE_STATUS, &success);
  return success;
}

const std::vector<char> Shader::get_info_log() const noexcept {
  int32_t message_length;
  glGetProgramiv(this->id, GL_INFO_LOG_LENGTH, &message_length);

  std::vector<char> message(message_length);
  glGetShaderInfoLog(this->id, message_length, nullptr, message.data());
  return message;
}

void Shader::delete_shader() const noexcept {
  glDeleteShader(this->id);
}

std::string to_string(const Shader::ShaderType& type) noexcept {
  switch (type) {
  case Shader::ShaderType::COMPUTE_SHADER:
    return "GL_COMPUTE_SHADER";
  case Shader::ShaderType::VERTEX_SHADER:
    return "GL_VERTEX_SHADER";
  case Shader::ShaderType::CONTROL_SHADER:
    return "GL_TESS_CONTROL_SHADER";
  case Shader::ShaderType::EVALUATION_SHADER:
    return "GL_TESS_EVALUATION_SHADER";
  case Shader::ShaderType::GEOMETRY_SHADER:
    return "GL_GEOMETRY_SHADER";
  case Shader::ShaderType::FRAGMENT_SHADER:
    return "GL_FRAGMENT_SHADER";
  default:
    return "UNKNOWN SHADER TYPE";
  }
}
}