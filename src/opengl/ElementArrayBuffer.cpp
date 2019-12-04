#include "ElementArrayBuffer.hpp"

namespace opengl {

void ElementArrayBuffer::unbind() noexcept {
  Buffer::unbind(BufferType::ELEMENT_ARRAY_BUFFER);
}

ElementArrayBuffer::ElementArrayBuffer() noexcept : Buffer(BufferType::ELEMENT_ARRAY_BUFFER) {
}
}