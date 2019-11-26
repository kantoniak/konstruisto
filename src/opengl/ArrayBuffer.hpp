#ifndef OPENGL_ARRAYBUFFER_HPP
#define OPENGL_ARRAYBUFFER_HPP

#include "Buffer.hpp"

namespace opengl {

class ArrayBuffer : public Buffer {

public:
  static void unbind() noexcept;

  ArrayBuffer() noexcept;
};
}

#endif
