#ifndef OPENGL_BLOCKUNIFORM_HPP
#define OPENGL_BLOCKUNIFORM_HPP

#include <cstdint>
#include <cstring>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>

#include <glad/gl.h>
#include <glm/detail/qualifier.hpp>
#include <glm/ext.hpp>
#include <glm/glm.hpp>

#include "Type.hpp"

namespace opengl {

class BlockUniform {

public:
  BlockUniform() = delete;
  BlockUniform(const Type type, std::string name, const int32_t offset) noexcept;

  bool operator==(const BlockUniform& other) const noexcept;

  [[nodiscard]] Type get_type() const noexcept;
  [[nodiscard]] const std::string& get_name() const noexcept;
  [[nodiscard]] int32_t get_offset() const noexcept;

  template <typename T>
  void memcpy(std::vector<uint8_t>& destination, const T& data) const noexcept;

  template <glm::length_t L, typename T, glm::qualifier Q>
  void memcpy(std::vector<uint8_t>& destination, const glm::vec<L, T, Q>& data) const noexcept;

  template <glm::length_t C, glm::length_t R, typename T, glm::qualifier Q>
  void memcpy(std::vector<uint8_t>& destination, const glm::mat<C, R, T, Q>& data) const noexcept;

  friend std::ostream& operator<<(std::ostream& os, const BlockUniform& uniform);
  friend std::string to_string(const BlockUniform& uniform) noexcept;

protected:
  Type type;
  std::string name;
  int32_t offset;
};
}

#endif
