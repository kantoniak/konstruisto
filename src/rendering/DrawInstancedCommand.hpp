#ifndef RENDERING_DRAWINSTANCEDCOMMAND_HPP
#define RENDERING_DRAWINSTANCEDCOMMAND_HPP

#include <glm/glm.hpp>

#include "Command.hpp"
#include "Material.hpp"
#include "Mesh.hpp"

namespace rendering {

using DrawInstancedCommand = struct DrawInstancedCommand : Command {

  // Per-instance data
  std::vector<glm::mat4> model_transforms;

  static DrawInstancedCommand* create(const std::vector<glm::mat4>& transforms, const Material& material,
                                      const Mesh& mesh) {
    auto* result = new DrawInstancedCommand();
    result->init(material, mesh);
    result->model_transforms = transforms;
    return result;
  }
};
}

#endif
