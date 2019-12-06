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

std::ostream& operator<<(std::ostream& os, const Mesh& mesh) noexcept {
  return os << "Mesh [vertex_count=" << mesh.vertex_count << ", vbo_offset=" << mesh.vbo_offset
            << ", ebo_offset=" << mesh.ebo_offset << ", material=" << to_string(mesh.material) << "]";
}

std::string to_string(const Mesh& mesh) noexcept {
  std::stringstream result;
  result << mesh;
  return result.str();
}
}