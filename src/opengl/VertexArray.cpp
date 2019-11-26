#include "VertexArray.hpp"

namespace opengl {

VertexArray::VertexArray() noexcept : id(0) {
}

uint32_t VertexArray::get_id() const {
  return id;
}

void VertexArray::generate() {
  glGenVertexArrays(1, &(this->id));
}

void VertexArray::bind() {
  glBindVertexArray(this->id);
}

void VertexArray::unbind() {
  glBindVertexArray(0);
}

void VertexArray::delete_vertex_array() {
  glDeleteVertexArrays(1, &(this->id));
}
}