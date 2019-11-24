#ifndef OPENGL_SHADERPROGRAM_HPP
#define OPENGL_SHADERPROGRAM_HPP

#include <cstdint>
#include <vector>

#include <glad/gl.h>

#include "../engine/Logger.hpp"
#include "Shader.hpp"

namespace opengl {

class ShaderProgram {

public:
  static ShaderProgram create() noexcept;
  ShaderProgram() noexcept;

  [[nodiscard]] uint32_t get_id() const noexcept;

  void attach(const Shader& shader);
  bool link();
  [[nodiscard]] const std::vector<char> get_info_log();

  [[nodiscard]] int32_t get_uniform_loc(const char* name);

  void use();
  void delete_program();

protected:
  ShaderProgram(uint32_t id) noexcept;

  uint32_t id;
};

}

#endif
