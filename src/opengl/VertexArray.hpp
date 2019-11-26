#ifndef OPENGL_VERTEXARRAY_HPP
#define OPENGL_VERTEXARRAY_HPP

#include <cstdint>

#include <glad/gl.h>

#include "../engine/Logger.hpp"

namespace opengl {

class VertexArray {

public:
  static void unbind() noexcept;

  VertexArray() noexcept;

  [[nodiscard]] uint32_t get_id() const noexcept;

  void generate() noexcept;
  void bind() const noexcept;
  void delete_vertex_array() const noexcept;

protected:
  uint32_t id;
};
}

#endif
