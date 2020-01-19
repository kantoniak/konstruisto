#ifndef RENDERING_DRAWSINGLECOMMAND_HPP
#define RENDERING_DRAWSINGLECOMMAND_HPP

#include <glm/glm.hpp>

#include "Command.hpp"
#include "Material.hpp"
#include "Mesh.hpp"

namespace rendering {

using DrawSingleCommand = struct DrawSingleCommand : Command {
  // Uniforms
  glm::mat4 model_transform;

  static DrawSingleCommand* create(glm::mat4 transform, const Material& material, const Mesh& mesh) {
    auto* result = new DrawSingleCommand();
    result->init(material, mesh);
    result->model_transform = transform;
    return result;
  }
};
}

#endif
