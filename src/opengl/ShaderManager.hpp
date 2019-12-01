#ifndef OPENGL_SHADERMANAGER_HPP
#define OPENGL_SHADERMANAGER_HPP

#include <array>
#include <fstream>
#include <optional>
#include <vector>

#include <glad/gl.h>

#include "../engine/Logger.hpp"
#include "Shader.hpp"
#include "ShaderProgram.hpp"
#include "UniformBlock.hpp"

namespace opengl {

class ShaderManager {
  using uniform_blocks_iterator = std::unordered_map<std::string, UniformBlock>::iterator;

public:
  std::optional<Shader> compileShader(Shader::ShaderType shaderType, std::string filename, engine::Logger& log);

  template <size_t n>
  std::optional<ShaderProgram> linkProgram(const std::array<Shader, n>& shaders, engine::Logger& log);

  const UniformBlock& get_uniform_block(std::string name) const noexcept;

protected:
  uint32_t uniform_buffer_next_binding_block = 1; // Start from 1 because NanoVG uses 0

  std::unordered_map<std::string, UniformBlock> uniform_blocks;

  void read_uniform_blocks(const ShaderProgram& shader_program) noexcept;
  inline std::string get_uniform_block_name(const std::string& uniform_name) const noexcept;
};
}

#endif
