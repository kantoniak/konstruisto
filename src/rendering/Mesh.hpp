#ifndef RENDERING_MESH_HPP
#define RENDERING_MESH_HPP

#include "Material.hpp"

namespace rendering {

class Mesh {

public:
  Mesh() = delete;
  Mesh(int32_t vertex_count, int32_t vbo_offset, int32_t ebo_offset, const Material& material) noexcept;

  [[nodiscard]] int32_t get_vertex_count() const noexcept;
  [[nodiscard]] int32_t get_vbo_offset() const noexcept;
  [[nodiscard]] int32_t get_ebo_offset() const noexcept;
  [[nodiscard]] const Material& get_material() const noexcept;

protected:
  int32_t vertex_count;
  int32_t vbo_offset;
  int32_t ebo_offset;
  const Material& material;
};
}

#endif
