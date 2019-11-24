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
std::optional<ShaderProgram> ShaderManager::linkProgram(const std::array<Shader, n>& shaders, engine::Logger& log) {
  ShaderProgram program = ShaderProgram::create();

  if (program.get_id() == 0) {
    log.error("Shader program creating failed.");
    return std::nullopt;
  }

  for (auto shader : shaders) {
    program.attach(shader);
  }

  if (!program.link()) {
    log.error("Shader program linking failed: %s", program.get_info_log().data());
    program.delete_program();
    return std::nullopt;
  }

  log.debug("Compiled shader program.");
  return program;
}

// As of OpenGL 4.6 there can be up to 5 shaders in use.
template std::optional<ShaderProgram> ShaderManager::linkProgram(const std::array<Shader, 1>& shaders,
                                                                 engine::Logger& log);
template std::optional<ShaderProgram> ShaderManager::linkProgram(const std::array<Shader, 2>& shaders,
                                                                 engine::Logger& log);
template std::optional<ShaderProgram> ShaderManager::linkProgram(const std::array<Shader, 3>& shaders,
                                                                 engine::Logger& log);
template std::optional<ShaderProgram> ShaderManager::linkProgram(const std::array<Shader, 4>& shaders,
                                                                 engine::Logger& log);
template std::optional<ShaderProgram> ShaderManager::linkProgram(const std::array<Shader, 5>& shaders,
                                                                 engine::Logger& log);
}
