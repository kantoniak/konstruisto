#ifndef OPENGL_ELEMENTARRAYBUFFER_HPP
#define OPENGL_ELEMENTARRAYBUFFER_HPP

#include "Buffer.hpp"

namespace opengl {

class ElementArrayBuffer : public Buffer {

public:
  static void unbind() noexcept;

  ElementArrayBuffer() noexcept;
};
}

#endif
