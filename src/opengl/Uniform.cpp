#include "Uniform.hpp"

#include "ShaderProgram.hpp"
#include <utility>

namespace opengl {

Uniform::Uniform(const Type type, std::string name, const int32_t location, const uint32_t owner_id) noexcept
    : type(type), name(std::move(name)), location(location), owner_id(owner_id) {
}

bool Uniform::operator==(const Uniform& other) const noexcept {
  return other.owner_id == owner_id && other.location == location;
}

Type Uniform::get_type() const noexcept {
  return type;
}

const std::string& Uniform::get_name() const noexcept {
  return name;
}

int32_t Uniform::get_location() const noexcept {
  return location;
}

template <>
void Uniform::submit<bool>(const bool& data) const noexcept {
  assert(ShaderProgram::bound_id == owner_id);
  glUniform1i(location, data);
}

template <>
void Uniform::submit<int>(const int& data) const noexcept {
  assert(ShaderProgram::bound_id == owner_id);
  glUniform1i(location, data);
}

template <>
void Uniform::submit<glm::vec4>(const glm::vec4& data) const noexcept {
  assert(ShaderProgram::bound_id == owner_id);
  glUniform4fv(location, 1, glm::value_ptr(data));
}

template <>
void Uniform::submit<glm::ivec4>(const glm::ivec4& data) const noexcept {
  assert(ShaderProgram::bound_id == owner_id);
  glUniform4iv(location, 1, glm::value_ptr(data));
}

template <>
void Uniform::submit<glm::mat4>(const glm::mat4& data) const noexcept {
  assert(ShaderProgram::bound_id == owner_id);
  glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(data));
}

std::ostream& operator<<(std::ostream& os, const Uniform& uniform) {
  return os << "Uniform [name=\"" << uniform.name << "\", type=" << to_string(uniform.type)
            << ", location=" << uniform.location << ", owner_id=" << uniform.owner_id << "]";
}

std::string to_string(const Uniform& uniform) noexcept {
  std::stringstream result;
  result << uniform;
  return result.str();
}
}