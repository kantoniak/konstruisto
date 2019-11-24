#ifndef OPENGL_SHADER_HPP
#define OPENGL_SHADER_HPP

#include <cstdint>

#include <glad/gl.h>

#include "../engine/Logger.hpp"

namespace opengl {

class Shader {

public:
  enum ShaderType {
    COMPUTE_SHADER = GL_COMPUTE_SHADER,
    VERTEX_SHADER = GL_VERTEX_SHADER,
    CONTROL_SHADER = GL_TESS_CONTROL_SHADER,
    EVALUATION_SHADER = GL_TESS_EVALUATION_SHADER,
    GEOMETRY_SHADER = GL_GEOMETRY_SHADER,
    FRAGMENT_SHADER = GL_FRAGMENT_SHADER
  };

  static Shader create(ShaderType type) noexcept;

  [[nodiscard]] uint32_t get_id() const;
  [[nodiscard]] ShaderType get_type() const;

  bool compile(const std::string& source);
  [[nodiscard]] const std::vector<char> get_info_log();

  void delete_shader();

protected:
  Shader(uint32_t id, ShaderType type);
  const uint32_t id;
  const ShaderType type;
};

std::string to_string(const Shader::ShaderType& type);
}

#endif
