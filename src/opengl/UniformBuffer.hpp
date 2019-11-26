#ifndef OPENGL_UNIFORMBUFFER_HPP
#define OPENGL_UNIFORMBUFFER_HPP

#include <cstdint>

#include <glad/gl.h>

#include "Buffer.hpp"

namespace opengl {

class UniformBuffer : public Buffer {

public:
  static void unbind() noexcept;

  UniformBuffer() noexcept;

  void bind_to_binding_point(uint32_t binding_point) const noexcept;
};
}

#endif
