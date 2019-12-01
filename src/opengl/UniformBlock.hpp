#ifndef OPENGL_UNIFORMBLOCK_HPP
#define OPENGL_UNIFORMBLOCK_HPP

#include <cstdint>
#include <unordered_map>

#include <glad/gl.h>

#include "BlockUniform.hpp"
#include "Type.hpp"

namespace opengl {

class ShaderProgram;

class UniformBlock {

public:
  UniformBlock(std::string name, uint32_t global_binding_point) noexcept;

  bool operator==(const UniformBlock& other) const noexcept;

  [[nodiscard]] const std::string& get_name() const noexcept;
  [[nodiscard]] uint32_t get_global_binding_point() const noexcept;
  [[nodiscard]] uint32_t get_size() const noexcept;

  void bind_program(const ShaderProgram& program, const uint32_t uniform_block_index) noexcept;
  void register_uniform(BlockUniform uniform) noexcept;
  const BlockUniform& get_uniform(std::string name) const noexcept;

  friend std::ostream& operator<<(std::ostream& os, const UniformBlock& uniform_block) noexcept;
  friend std::string to_string(const UniformBlock& uniform_block) noexcept;

protected:
  std::string name;
  uint32_t binding_point; // These are global binding points
  uint32_t size = 0;
  std::unordered_map<uint32_t, uint32_t> program_to_block_index;
  std::unordered_map<std::string, BlockUniform> name_to_uniform;
};
}

#endif
