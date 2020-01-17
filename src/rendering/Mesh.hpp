#ifndef RENDERING_MESH_HPP
#define RENDERING_MESH_HPP

#include <sstream>

#include "Material.hpp"

namespace rendering {

class Mesh {

public:
  Mesh() = delete;
  Mesh(int32_t vertex_count, int32_t vbo_offset, int32_t ebo_offset) noexcept;

  [[nodiscard]] int32_t get_vertex_count() const noexcept;
  [[nodiscard]] int32_t get_vbo_offset() const noexcept;
  [[nodiscard]] int32_t get_ebo_offset() const noexcept;

  friend std::ostream& operator<<(std::ostream& os, const Mesh& mesh) noexcept;
  friend std::string to_string(const Mesh& mesh) noexcept;

protected:
  int32_t vertex_count;
  int32_t vbo_offset;
  int32_t ebo_offset;
};
}

#endif
