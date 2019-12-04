#ifndef RENDERING_OBJECT_HPP
#define RENDERING_OBJECT_HPP

#include <string>
#include <vector>

#include <glm/glm.hpp>

#include "Model.hpp"

namespace rendering {

class Object {

public:
  Object() = delete;
  Object(const Model& model, glm::mat4 transform) noexcept;

  const Model& get_model() const noexcept;
  const glm::mat4& get_transform() const noexcept;

protected:
  const Model& model;
  glm::mat4 transform;
};
}

#endif
