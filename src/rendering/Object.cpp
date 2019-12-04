#include "Object.hpp"

namespace rendering {
Object::Object(const Model& model, glm::mat4 transform) noexcept : model(model), transform(transform) {
}

const Model& Object::get_model() const noexcept {
  return model;
}

const glm::mat4& Object::get_transform() const noexcept {
  return transform;
}
}