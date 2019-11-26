#ifndef OPENGL_VERTEXARRAY_HPP
#define OPENGL_VERTEXARRAY_HPP

#include <cstdint>

#include <glad/gl.h>

#include "../engine/Logger.hpp"

namespace opengl {

class VertexArray {

public:
  VertexArray() noexcept;

  [[nodiscard]] uint32_t get_id() const;

  void generate();
  void bind();
  void unbind();
  void delete_vertex_array();

protected:
  uint32_t id;
};
}

#endif
