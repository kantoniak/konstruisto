#include "Shader.hpp"

namespace rendering {

Shader::Shader(uint32_t id, Shader::ShaderType type) : id(id), type(type) {
}

uint32_t Shader::get_id() const {
  return id;
}

Shader::ShaderType Shader::get_type() const {
  return type;
}

void Shader::delete_shader() {
  glDeleteShader(this->id);
}

std::string to_string(const Shader::ShaderType& type) {
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