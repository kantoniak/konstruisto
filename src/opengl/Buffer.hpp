#ifndef OPENGL_BUFFER_HPP
#define OPENGL_BUFFER_HPP

#include <cassert>
#include <cstdint>

#include <glad/gl.h>

#include "../engine/Logger.hpp"

namespace opengl {

class Buffer {

public:
  enum BufferType {
    ARRAY_BUFFER = GL_ARRAY_BUFFER,
    ATOMIC_COUNTER_BUFFER = GL_ATOMIC_COUNTER_BUFFER,
    COPY_READ_BUFFER = GL_COPY_READ_BUFFER,
    COPY_WRITE_BUFFER = GL_COPY_WRITE_BUFFER,
    DISPATCH_INDIRECT_BUFFER = GL_DISPATCH_INDIRECT_BUFFER,
    DRAW_INDIRECT_BUFFER = GL_DRAW_INDIRECT_BUFFER,
    ELEMENT_ARRAY_BUFFER = GL_ELEMENT_ARRAY_BUFFER,
    PIXEL_PACK_BUFFER = GL_PIXEL_PACK_BUFFER,
    PIXEL_UNPACK_BUFFER = GL_PIXEL_UNPACK_BUFFER,
    SHADER_STORAGE_BUFFER = GL_SHADER_STORAGE_BUFFER,
    TEXTURE_BUFFER = GL_TEXTURE_BUFFER,
    TRANSFORM_FEEDBACK_BUFFER = GL_TRANSFORM_FEEDBACK_BUFFER,
    UNIFORM_BUFFER = GL_UNIFORM_BUFFER
  };

  Buffer() = delete;

  [[nodiscard]] uint32_t get_id() const noexcept;
  [[nodiscard]] BufferType get_type() const noexcept;

  void generate() noexcept;
  void bind() const noexcept;
  void orphan() noexcept;
  void delete_buffer() const noexcept;

  template <class T>
  void buffer_data(const T& data, GLenum usage) noexcept {
    assert(sizeof(T) > 0);
    this->usage = usage;
    this->data_size = sizeof(T);
    glBufferData(this->type, this->data_size, std::addressof(data), this->usage);
  }

  template <class T>
  void buffer_data(const std::vector<T>& v, GLenum usage) noexcept {
    assert(v.size() * sizeof(T) > 0);
    this->usage = usage;
    this->data_size = v.size() * sizeof(T);
    glBufferData(this->type, this->data_size, &v[0], this->usage);
  }

protected:
  uint32_t id;
  const BufferType type;
  size_t data_size;
  GLenum usage;

  static void unbind(const BufferType type) noexcept;

  Buffer(BufferType type) noexcept;
};

std::string to_string(const Buffer::BufferType& type) noexcept;
}

#endif
