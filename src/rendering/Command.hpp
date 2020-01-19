#ifndef RENDERING_COMMAND_HPP
#define RENDERING_COMMAND_HPP

#include <glm/glm.hpp>

#include "Material.hpp"
#include "Mesh.hpp"

namespace rendering {

using Command = struct Command {
  enum Type { DRAW_SINGLE = 0, DRAW_INSTANCED = 1 };

  // Material data
  glm::vec3 ambient;
  glm::vec3 diffuse;
  glm::vec3 specular;
  float shininess;

  // Mesh data
  int32_t vertex_count;
  int32_t ebo_offset;
  int32_t vbo_offset;

  void init(const Material& material, const Mesh& mesh) noexcept {
    ambient = material.get_ambient();
    diffuse = material.get_diffuse();
    specular = material.get_specular();
    shininess = material.get_shininess();

    vertex_count = mesh.get_vertex_count();
    vbo_offset = mesh.get_vbo_offset();
    ebo_offset = mesh.get_ebo_offset();
  }
};
}

#endif
