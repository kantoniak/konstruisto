#ifndef OPENGL_SHADERPROGRAM_HPP
#define OPENGL_SHADERPROGRAM_HPP

#include <array>
#include <cstdint>
#include <unordered_map>
#include <vector>

#include <glad/gl.h>

#include "Shader.hpp"
#include "Type.hpp"
#include "Uniform.hpp"

namespace opengl {

class ShaderProgram {

public:
  static uint32_t bound_id;

  static void unbind() noexcept;

  ShaderProgram() noexcept;

  [[nodiscard]] uint32_t get_id() const noexcept;

  void generate() noexcept;
  void attach(const Shader& shader) const noexcept;
  [[nodiscard]] bool link() noexcept;
  [[nodiscard]] const std::vector<char> get_info_log() const noexcept;

  [[nodiscard]] const Uniform& get_uniform(const std::string& name) const noexcept;

  void use() const noexcept;
  void delete_program() const noexcept;

  template <typename T>
  void submit(const std::string& uniform, const T& data) const noexcept {
    uniforms.at(uniform).submit<T>(data);
  }

protected:
  uint32_t id;
  std::unordered_map<std::string, Uniform> uniforms;

  void read_uniforms() noexcept;
};
}

#endif