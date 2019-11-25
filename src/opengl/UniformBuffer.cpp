#include "UniformBuffer.hpp"

namespace opengl {

UniformBuffer::UniformBuffer() noexcept : Buffer(BufferType::UNIFORM_BUFFER) {
}

void UniformBuffer::bind_to_binding_point(uint32_t binding_point) {
  glBindBufferBase(this->type, binding_point, this->id);
}
}