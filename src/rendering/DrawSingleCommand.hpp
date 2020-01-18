#ifndef RENDERING_DRAWSINGLECOMMAND_HPP
#define RENDERING_DRAWSINGLECOMMAND_HPP

#include <glm/glm.hpp>

#include "Material.hpp"
#include "Mesh.hpp"

namespace rendering {

typedef struct DrawSingleCommand {
  // Uniforms
  glm::mat4 model_transform;

  // Material data
  glm::vec3 ambient;
  glm::vec3 diffuse;
  glm::vec3 specular;
  float shininess;

  // Mesh data
  int32_t vertex_count;
  int32_t ebo_offset;
  int32_t vbo_offset;

  static DrawSingleCommand* create(glm::mat4 transform, const Material& material, const Mesh& mesh);
  static int32_t create_key(const Material& material);

} DrawSingleCommand;
}

#endif
