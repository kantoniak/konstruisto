#include "Buffer.hpp"

namespace opengl {

Buffer::Buffer(Buffer::BufferType type) noexcept : type(type) {
}

uint32_t Buffer::get_id() const {
  return id;
}

Buffer::BufferType Buffer::get_type() const {
  return type;
}

void Buffer::generate() {
  glGenBuffers(1, &(this->id));
}

void Buffer::bind() {
  glBindBuffer(this->type, this->id);
}

void Buffer::unbind() {
  glBindBuffer(this->type, 0);
}

void Buffer::delete_buffer() {
  glDeleteBuffers(1, &(this->id));
}

std::string to_string(const Buffer::BufferType& type) {
  switch (type) {
  case Buffer::BufferType::ARRAY_BUFFER:
    return "ARRAY_BUFFER";
  case Buffer::BufferType::ATOMIC_COUNTER_BUFFER:
    return "ATOMIC_COUNTER_BUFFER";
  case Buffer::BufferType::COPY_READ_BUFFER:
    return "COPY_READ_BUFFER";
  case Buffer::BufferType::COPY_WRITE_BUFFER:
    return "COPY_WRITE_BUFFER";
  case Buffer::BufferType::DISPATCH_INDIRECT_BUFFER:
    return "DISPATCH_INDIRECT_BUFFER";
  case Buffer::BufferType::DRAW_INDIRECT_BUFFER:
    return "DRAW_INDIRECT_BUFFER";
  case Buffer::BufferType::ELEMENT_ARRAY_BUFFER:
    return "ELEMENT_ARRAY_BUFFER";
  case Buffer::BufferType::PIXEL_PACK_BUFFER:
    return "PIXEL_PACK_BUFFER";
  case Buffer::BufferType::PIXEL_UNPACK_BUFFER:
    return "PIXEL_UNPACK_BUFFER";
  case Buffer::BufferType::SHADER_STORAGE_BUFFER:
    return "SHADER_STORAGE_BUFFER";
  case Buffer::BufferType::TEXTURE_BUFFER:
    return "TEXTURE_BUFFER";
  case Buffer::BufferType::TRANSFORM_FEEDBACK_BUFFER:
    return "TRANSFORM_FEEDBACK_BUFFER";
  case Buffer::BufferType::UNIFORM_BUFFER:
    return "GL_UNIFORM_BUFFER";
  default:
    return "Unknown";
  }
}
}