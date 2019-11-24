#ifndef RENDERING_SHADER_HPP
#define RENDERING_SHADER_HPP

#include <array>
#include <cstdint>
#include <fstream>
#include <string>
#include <vector>

#include <GL/glew.h>

#include "../engine/Logger.hpp"

namespace rendering {

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

  Shader(uint32_t id, ShaderType type);

  [[nodiscard]] uint32_t get_id() const;
  [[nodiscard]] ShaderType get_type() const;

  void delete_shader();

protected:
  const uint32_t id;
  const ShaderType type;
};

std::string to_string(const Shader::ShaderType& type);
}

#endif
