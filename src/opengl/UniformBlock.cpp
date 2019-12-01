#include "UniformBlock.hpp"

#include "ShaderProgram.hpp"

namespace opengl {

UniformBlock::UniformBlock(std::string name, uint32_t global_binding_point) noexcept
    : name(std::move(name)), binding_point(global_binding_point) {
}

bool UniformBlock::operator==(const UniformBlock& other) const noexcept {
  return other.binding_point == binding_point;
}

[[nodiscard]] const std::string& UniformBlock::get_name() const noexcept {
  return name;
}

[[nodiscard]] uint32_t UniformBlock::get_global_binding_point() const noexcept {
  return binding_point;
}

[[nodiscard]] uint32_t UniformBlock::get_size() const noexcept {
  return size;
}

void UniformBlock::bind_program(const ShaderProgram& program, const uint32_t uniform_block_index) noexcept {
  glUniformBlockBinding(program.get_id(), uniform_block_index, binding_point);
  program_to_block_index.emplace(program.get_id(), uniform_block_index);
}

void UniformBlock::register_uniform(BlockUniform uniform) noexcept {
  name_to_uniform.emplace(uniform.get_name(), uniform);

  // Update uniform block size
  if (uniform.get_offset() + opengl::get_size(uniform.get_type()) > size) {
    size = uniform.get_offset() + opengl::get_size(uniform.get_type());
  }
}

const BlockUniform& UniformBlock::get_uniform(std::string name) const noexcept {
  return name_to_uniform.at(name);
}

std::ostream& operator<<(std::ostream& os, const UniformBlock& uniform_block) noexcept {
  return os << "UniformBlock [name=\"" << uniform_block.name << "\", binding_point=" << uniform_block.binding_point
            << "]";
}

std::string to_string(const UniformBlock& uniform_block) noexcept {
  std::stringstream result;
  result << uniform_block;
  return result.str();
}
}