#include "DrawSingleCommand.hpp"

namespace rendering {

DrawSingleCommand* DrawSingleCommand::create(glm::mat4 transform, const Material& material, const Mesh& mesh) {
  DrawSingleCommand* result = new DrawSingleCommand();

  result->model_transform = transform;

  result->ambient = material.get_ambient();
  result->diffuse = material.get_diffuse();
  result->specular = material.get_specular();
  result->shininess = material.get_shininess();

  result->vertex_count = mesh.get_vertex_count();
  result->vbo_offset = mesh.get_vbo_offset();
  result->ebo_offset = mesh.get_ebo_offset();

  return result;
}

int32_t DrawSingleCommand::create_key(const Material& material) {
  return material.get_index();
}

}