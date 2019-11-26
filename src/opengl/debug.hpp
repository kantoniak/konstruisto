#ifndef OPENGL_DEBUG_HPP
#define OPENGL_DEBUG_HPP

#include <string>

#include <glad/gl.h>

namespace opengl {

const std::string gl_debug_source_to_string(const uint32_t source) noexcept {
  switch (source) {
  case GL_DEBUG_SOURCE_API:
    return "API";
  case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
    return "Window system";
  case GL_DEBUG_SOURCE_SHADER_COMPILER:
    return "Shader compiler";
  case GL_DEBUG_SOURCE_THIRD_PARTY:
    return "Third party";
  case GL_DEBUG_SOURCE_APPLICATION:
    return "Application";
  case GL_DEBUG_SOURCE_OTHER:
    return "Other";
  default:
    return "Unknown";
  }
}

const std::string gl_debug_type_to_string(const uint32_t type) noexcept {
  switch (type) {
  case GL_DEBUG_TYPE_ERROR:
    return "Error";
  case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
    return "Deprecated Behaviour";
  case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
    return "Undefined Behaviour";
  case GL_DEBUG_TYPE_PORTABILITY:
    return "Portability";
  case GL_DEBUG_TYPE_PERFORMANCE:
    return "Performance";
  case GL_DEBUG_TYPE_MARKER:
    return "Marker";
  case GL_DEBUG_TYPE_PUSH_GROUP:
    return "Push Group";
  case GL_DEBUG_TYPE_POP_GROUP:
    return "Pop Group";
  case GL_DEBUG_TYPE_OTHER:
    return "Other";
  default:
    return "Unknown";
  }
}

const std::string gl_debug_severity_to_string(const uint32_t severity) noexcept {
  switch (severity) {
  case GL_DEBUG_SEVERITY_HIGH:
    return "High";
  case GL_DEBUG_SEVERITY_MEDIUM:
    return "Medium";
  case GL_DEBUG_SEVERITY_LOW:
    return "Low";
  case GL_DEBUG_SEVERITY_NOTIFICATION:
    return "Notification";
  default:
    return "Unknown";
  }
}
}
#endif
