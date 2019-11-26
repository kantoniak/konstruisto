#include "ArrayBuffer.hpp"

namespace opengl {

void ArrayBuffer::unbind() noexcept {
  Buffer::unbind(BufferType::ARRAY_BUFFER);
}

ArrayBuffer::ArrayBuffer() noexcept : Buffer(BufferType::ARRAY_BUFFER) {
}
}