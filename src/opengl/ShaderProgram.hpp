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
  static void unbind() noexcept;

  ShaderProgram() noexcept;

  [[nodiscard]] uint32_t get_id() const noexcept;

  void generate() noexcept;
  void attach(const Shader& shader) const noexcept;
  [[nodiscard]] bool link() const noexcept;

  [[nodiscard]] const std::vector<char> get_info_log() const noexcept;
  [[nodiscard]] int32_t get_uniform_loc(const char* name) const noexcept;
  [[nodiscard]] uint32_t get_uniform_block_index(const char* name) const noexcept;

  void bind_uniform_block(const char* name, uint32_t binding_point) const noexcept;
  void use() const noexcept;
  void delete_program() const noexcept;

protected:
  uint32_t id;
};
}

#endif