#ifndef OPENGL_UNIFORMBUFFER_HPP
#define OPENGL_UNIFORMBUFFER_HPP

#include <cstdint>

#include <glad/gl.h>

#include "Buffer.hpp"

namespace opengl {

class UniformBuffer : public Buffer {

public:
  UniformBuffer() noexcept;

  void bind_to_binding_point(uint32_t binding_point);
};

}

#endif
