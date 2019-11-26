#include "ShaderManager.hpp"

namespace rendering {

std::optional<Shader> ShaderManager::compileShader(Shader::ShaderType shader_type, std::string filename,
                                                   engine::Logger& log) {
  Shader shader = Shader::create(shader_type);

  if (shader.get_id() == 0) {
    log.error("Shader creating failed.");
    return std::nullopt;
  }

  std::ifstream shaderFile(filename);
  if (!shaderFile.good()) {
    log.error("Could not open/read file \"%s\".", filename.c_str());
    return std::nullopt;
  }

  std::string source_string((std::istreambuf_iterator<char>(shaderFile)), std::istreambuf_iterator<char>());
  if (!shader.compile(source_string)) {
    log.error("Shader compilation failed for \"%s\": %s", filename.c_str(), shader.get_info_log().data());
    shader.delete_shader();
    return std::nullopt;
  }

  log.debug("Compiled %s \"%s\".", to_string(shader_type).c_str(), filename.c_str());
  return shader;
}

template <size_t n>
std::optional<ShaderProgram> ShaderManager::linkProgram(const std::array<Shader, n>& shaders, engine::Logger& log) {
  ShaderProgram program;
  program.generate();

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
