#include "UniformBuffer.hpp"

namespace opengl {

void UniformBuffer::unbind() noexcept {
  Buffer::unbind(BufferType::UNIFORM_BUFFER);
}

UniformBuffer::UniformBuffer() noexcept : Buffer(BufferType::UNIFORM_BUFFER) {
}

void UniformBuffer::bind_to_binding_point(uint32_t binding_point) const noexcept {
  glBindBufferBase(this->type, binding_point, this->id);
}
}