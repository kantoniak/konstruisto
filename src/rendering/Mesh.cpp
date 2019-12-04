#include "Mesh.hpp"

namespace rendering {
Mesh::Mesh(int32_t vertex_count, int32_t vbo_offset, int32_t ebo_offset, const Material& material) noexcept
    : vertex_count(vertex_count), vbo_offset(vbo_offset), ebo_offset(ebo_offset), material(material) {
}

int32_t Mesh::get_vertex_count() const noexcept {
  return vertex_count;
}

int32_t Mesh::get_vbo_offset() const noexcept {
  return vbo_offset;
}

int32_t Mesh::get_ebo_offset() const noexcept {
  return ebo_offset;
}

const Material& Mesh::get_material() const noexcept {
  return material;
}
}