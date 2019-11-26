#include "VertexArray.hpp"

namespace opengl {

void VertexArray::unbind() noexcept {
  glBindVertexArray(0);
}

VertexArray::VertexArray() noexcept : id(0) {
}

uint32_t VertexArray::get_id() const noexcept {
  return id;
}

void VertexArray::generate() noexcept {
  glGenVertexArrays(1, &(this->id));
}

void VertexArray::bind() const noexcept {
  glBindVertexArray(this->id);
}

void VertexArray::delete_vertex_array() const noexcept {
  glDeleteVertexArrays(1, &(this->id));
}
}