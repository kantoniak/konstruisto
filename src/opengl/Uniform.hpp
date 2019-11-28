#ifndef OPENGL_UNIFORM_HPP
#define OPENGL_UNIFORM_HPP

#include <cassert>
#include <cstdint>
#include <ostream>
#include <sstream>

#include <glad/gl.h>
#include <glm/ext.hpp>
#include <glm/glm.hpp>

#include "Type.hpp"

namespace opengl {

class Uniform {

public:
  Uniform() = delete;
  Uniform(const Type type, std::string name, const int32_t location, const uint32_t owner_id) noexcept;

  bool operator==(const Uniform& other) const noexcept;

  [[nodiscard]] Type get_type() const noexcept;
  [[nodiscard]] const std::string& get_name() const noexcept;
  [[nodiscard]] int32_t get_location() const noexcept;

  template <typename T>
  void submit(const T& data) const noexcept;

  friend std::ostream& operator<<(std::ostream& os, const Uniform& uniform);
  friend std::string to_string(const Uniform& uniform) noexcept;

protected:
  Type type;
  std::string name;
  int32_t location;
  uint32_t owner_id;
};
}

#endif
