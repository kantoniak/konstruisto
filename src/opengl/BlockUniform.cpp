#include "BlockUniform.hpp"

namespace opengl {

BlockUniform::BlockUniform(const Type type, std::string name, const int32_t offset) noexcept
    : type(type), name(std::move(name)), offset(offset) {
}

bool BlockUniform::operator==(const BlockUniform& other) const noexcept {
  return other.name == name;
}

Type BlockUniform::get_type() const noexcept {
  return type;
}

const std::string& BlockUniform::get_name() const noexcept {
  return name;
}

int32_t BlockUniform::get_offset() const noexcept {
  return offset;
}

template <typename T>
void BlockUniform::memcpy(std::vector<uint8_t>& destination, const T& data) const noexcept {
  std::memcpy(&destination[offset], &data, sizeof(T));
}

template <glm::length_t C, glm::length_t R, typename T, glm::qualifier Q>
void BlockUniform::memcpy(std::vector<uint8_t>& destination, const glm::mat<C, R, T, Q>& data) const noexcept {
  std::memcpy(&destination[offset], glm::value_ptr(data), C * R * sizeof(T));
}

std::ostream& operator<<(std::ostream& os, const BlockUniform& uniform) {
  return os << "BlockUniform [name=\"" << uniform.name << "\", type=" << to_string(uniform.type)
            << ", offset=" << uniform.offset << "]";
}

std::string to_string(const BlockUniform& uniform) noexcept {
  std::stringstream result;
  result << uniform;
  return result.str();
}

// unsigned int
template void BlockUniform::memcpy<unsigned int>(std::vector<uint8_t>& destination, const unsigned int& data) const
    noexcept;

// glm::mat4
template void BlockUniform::memcpy<4, 4, glm::f32, glm::packed_highp>(
    std::vector<uint8_t>& destination, const glm::mat<4, 4, glm::f32, glm::packed_highp>& data) const noexcept;
}