#include "Type.hpp"

namespace opengl {

uint32_t get_size(const Type& type) noexcept {
  switch (type) {
  case BOOL:
    return 1 * sizeof(GLboolean);
  case BOOL_VEC2:
    return 2 * sizeof(GLboolean);
  case BOOL_VEC3:
    return 3 * sizeof(GLboolean);
  case BOOL_VEC4:
    return 4 * sizeof(GLboolean);
  case BYTE:
    return 1 * sizeof(GLbyte);
  case DOUBLE:
    return 1 * sizeof(GLdouble);
  case DOUBLE_MAT2:
    return 2 * 2 * sizeof(GLdouble);
  case DOUBLE_MAT2x3:
    return 2 * 3 * sizeof(GLdouble);
  case DOUBLE_MAT2x4:
    return 2 * 4 * sizeof(GLdouble);
  case DOUBLE_MAT3:
    return 3 * 3 * sizeof(GLdouble);
  case DOUBLE_MAT3x2:
    return 3 * 2 * sizeof(GLdouble);
  case DOUBLE_MAT3x4:
    return 3 * 4 * sizeof(GLdouble);
  case DOUBLE_MAT4:
    return 4 * 4 * sizeof(GLdouble);
  case DOUBLE_MAT4x2:
    return 4 * 2 * sizeof(GLdouble);
  case DOUBLE_MAT4x3:
    return 4 * 3 * sizeof(GLdouble);
  case DOUBLE_VEC2:
    return 2 * sizeof(GLdouble);
  case DOUBLE_VEC3:
    return 3 * sizeof(GLdouble);
  case DOUBLE_VEC4:
    return 4 * sizeof(GLdouble);
  case FIXED:
    return 1 * sizeof(GLfixed);
  case FLOAT:
    return 1 * sizeof(GLfloat);
  case FLOAT_MAT2:
    return 2 * 2 * sizeof(GLfloat);
  case FLOAT_MAT2x3:
    return 2 * 3 * sizeof(GLfloat);
  case FLOAT_MAT2x4:
    return 2 * 4 * sizeof(GLfloat);
  case FLOAT_MAT3:
    return 3 * 3 * sizeof(GLfloat);
  case FLOAT_MAT3x2:
    return 3 * 2 * sizeof(GLfloat);
  case FLOAT_MAT3x4:
    return 3 * 4 * sizeof(GLfloat);
  case FLOAT_MAT4:
    return 4 * 4 * sizeof(GLfloat);
  case FLOAT_MAT4x2:
    return 4 * 2 * sizeof(GLfloat);
  case FLOAT_MAT4x3:
    return 4 * 3 * sizeof(GLfloat);
  case FLOAT_VEC2:
    return 2 * sizeof(GLfloat);
  case FLOAT_VEC3:
    return 3 * sizeof(GLfloat);
  case FLOAT_VEC4:
    return 4 * sizeof(GLfloat);
  case HALF_FLOAT:
    return 1 * sizeof(GLhalf);
  case INT:
    return 1 * sizeof(GLint);
  case INT_VEC2:
    return 2 * sizeof(GLint);
  case INT_VEC3:
    return 3 * sizeof(GLint);
  case INT_VEC4:
    return 4 * sizeof(GLint);
  case SAMPLER_1D:
    return 1 * sizeof(GLint);
  case SAMPLER_1D_ARRAY:
    return 1 * sizeof(GLint);
  case SAMPLER_2D:
    return 1 * sizeof(GLint);
  case SAMPLER_2D_ARRAY:
    return 1 * sizeof(GLint);
  case SAMPLER_3D:
    return 1 * sizeof(GLint);
  case SHORT:
    return 1 * sizeof(GLshort);
  case UNSIGNED_BYTE:
    return 1 * sizeof(GLubyte);
  case UNSIGNED_INT:
    return 1 * sizeof(GLuint);
  case UNSIGNED_INT_VEC2:
    return 2 * sizeof(GLuint);
  case UNSIGNED_INT_VEC3:
    return 3 * sizeof(GLuint);
  case UNSIGNED_INT_VEC4:
    return 4 * sizeof(GLuint);
  case UNSIGNED_SHORT:
    return 1 * sizeof(GLushort);
  default:
    assert(false);
    return 0;
  }
}

std::string to_string(const Type& type) noexcept {
  switch (type) {
  case BOOL:
    return "GL_BOOL";
  case BOOL_VEC2:
    return "GL_BOOL_VEC2";
  case BOOL_VEC3:
    return "GL_BOOL_VEC3";
  case BOOL_VEC4:
    return "GL_BOOL_VEC4";
  case BYTE:
    return "GL_BYTE";
  case DOUBLE:
    return "GL_DOUBLE";
  case DOUBLE_MAT2:
    return "GL_DOUBLE_MAT2";
  case DOUBLE_MAT2x3:
    return "GL_DOUBLE_MAT2x3";
  case DOUBLE_MAT2x4:
    return "GL_DOUBLE_MAT2x4";
  case DOUBLE_MAT3:
    return "GL_DOUBLE_MAT3";
  case DOUBLE_MAT3x2:
    return "GL_DOUBLE_MAT3x2";
  case DOUBLE_MAT3x4:
    return "GL_DOUBLE_MAT3x4";
  case DOUBLE_MAT4:
    return "GL_DOUBLE_MAT4";
  case DOUBLE_MAT4x2:
    return "GL_DOUBLE_MAT4x2";
  case DOUBLE_MAT4x3:
    return "GL_DOUBLE_MAT4x3";
  case DOUBLE_VEC2:
    return "GL_DOUBLE_VEC2";
  case DOUBLE_VEC3:
    return "GL_DOUBLE_VEC3";
  case DOUBLE_VEC4:
    return "GL_DOUBLE_VEC4";
  case FIXED:
    return "GL_FIXED";
  case FLOAT:
    return "GL_FLOAT";
  case FLOAT_MAT2:
    return "GL_FLOAT_MAT2";
  case FLOAT_MAT2x3:
    return "GL_FLOAT_MAT2x3";
  case FLOAT_MAT2x4:
    return "GL_FLOAT_MAT2x4";
  case FLOAT_MAT3:
    return "GL_FLOAT_MAT3";
  case FLOAT_MAT3x2:
    return "GL_FLOAT_MAT3x2";
  case FLOAT_MAT3x4:
    return "GL_FLOAT_MAT3x4";
  case FLOAT_MAT4:
    return "GL_FLOAT_MAT4";
  case FLOAT_MAT4x2:
    return "GL_FLOAT_MAT4x2";
  case FLOAT_MAT4x3:
    return "GL_FLOAT_MAT4x3";
  case FLOAT_VEC2:
    return "GL_FLOAT_VEC2";
  case FLOAT_VEC3:
    return "GL_FLOAT_VEC3";
  case FLOAT_VEC4:
    return "GL_FLOAT_VEC4";
  case HALF_FLOAT:
    return "GL_HALF_FLOAT";
  case INT:
    return "GL_INT";
  case INT_VEC2:
    return "GL_INT_VEC2";
  case INT_VEC3:
    return "GL_INT_VEC3";
  case INT_VEC4:
    return "GL_INT_VEC4";
  case SAMPLER_1D:
    return "GL_SAMPLER_1D";
  case SAMPLER_1D_ARRAY:
    return "GL_SAMPLER_1D_ARRAY";
  case SAMPLER_2D:
    return "GL_SAMPLER_2D";
  case SAMPLER_2D_ARRAY:
    return "GL_SAMPLER_2D_ARRAY";
  case SAMPLER_3D:
    return "GL_SAMPLER_3D";
  case SHORT:
    return "GL_SHORT";
  case UNSIGNED_BYTE:
    return "GL_UNSIGNED_BYTE";
  case UNSIGNED_INT:
    return "GL_UNSIGNED_INT";
  case UNSIGNED_INT_VEC2:
    return "GL_UNSIGNED_INT_VEC2";
  case UNSIGNED_INT_VEC3:
    return "GL_UNSIGNED_INT_VEC3";
  case UNSIGNED_INT_VEC4:
    return "GL_UNSIGNED_INT_VEC4";
  case UNSIGNED_SHORT:
    return "GL_UNSIGNED_SHORT";
  default:
    std::stringstream stream;
    stream << "Unknown (0x" << std::hex << type << ")";
    return stream.str();
  }
}
}