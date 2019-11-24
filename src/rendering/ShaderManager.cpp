#include "ShaderManager.hpp"

namespace rendering {

std::optional<Shader> ShaderManager::compileShader(Shader::ShaderType shader_type, std::string filename,
                                                   engine::Logger& log) {

  std::ifstream shaderFile(filename);
  if (!shaderFile.good()) {
    log.error("Could not open/read file \"%s\".", filename.c_str());
    return std::optional<Shader>();
  }

  std::string shaderSourceString((std::istreambuf_iterator<char>(shaderFile)), std::istreambuf_iterator<char>());
  const char* shaderSource = shaderSourceString.c_str();

  GLuint shader = glCreateShader(shader_type);
  glShaderSource(shader, 1, &shaderSource, nullptr);
  glCompileShader(shader);

  GLint success;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    std::array<GLchar, 512> errorMessage;
    glGetShaderInfoLog(shader, errorMessage.max_size(), nullptr, errorMessage.data());
    log.error("Shader compilation failed for \"%s\": %s", filename.c_str(), errorMessage);
    glDeleteShader(shader);
    return std::optional<Shader>();
  }
  log.info("Compiled %s \"%s\".", to_string(shader_type).c_str(), filename.c_str());

  return Shader(shader, shader_type);
}

template <size_t n>
GLuint ShaderManager::linkProgram(const std::array<Shader, n>& shaders, engine::Logger& log) {
  const GLuint shader_program = glCreateProgram();

  for (auto shader : shaders) {
    glAttachShader(shader_program, shader.get_id());
  }

  // TODO(kantoniak): Add support for glDebugMessageCallback.
  glLinkProgram(shader_program);

  GLint success;
  glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
  if (!success) {
    std::array<GLchar, 512> error_message;
    glGetProgramInfoLog(shader_program, error_message.max_size(), nullptr, error_message.data());
    log.error("Shader linking failed: %s", error_message);
    glDeleteProgram(shader_program);
    return 0;
  }

  return shader_program;
}

// As of OpenGL 4.6 there can be up to 5 shaders in use.
template GLuint ShaderManager::linkProgram(const std::array<Shader, 1>& shaders, engine::Logger& log);
template GLuint ShaderManager::linkProgram(const std::array<Shader, 2>& shaders, engine::Logger& log);
template GLuint ShaderManager::linkProgram(const std::array<Shader, 3>& shaders, engine::Logger& log);
template GLuint ShaderManager::linkProgram(const std::array<Shader, 4>& shaders, engine::Logger& log);
template GLuint ShaderManager::linkProgram(const std::array<Shader, 5>& shaders, engine::Logger& log);
}
