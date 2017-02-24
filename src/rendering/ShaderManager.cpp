#include "ShaderManager.hpp"

namespace rendering {

std::string ShaderManager::shaderTypeToString(GLenum shaderType) {
  switch (shaderType) {
  case GL_VERTEX_SHADER:
    return "GL_VERTEX_SHADER";
  case GL_FRAGMENT_SHADER:
    return "GL_FRAGMENT_SHADER";
  case GL_GEOMETRY_SHADER:
    return "GL_GEOMETRY_SHADER";
  }
  return "UNKNOWN TYPE SHADER";
}

GLuint ShaderManager::compileShader(GLenum shaderType, std::string filename, engine::Logger& log) {

  std::ifstream shaderFile(filename);
  if (!shaderFile.good()) {
    log.error("Could not open/read file \"%s\".", filename.c_str());
    return 0;
  }

  std::string shaderSourceString((std::istreambuf_iterator<char>(shaderFile)), std::istreambuf_iterator<char>());
  const char* shaderSource = shaderSourceString.c_str();

  GLuint shader = glCreateShader(shaderType);
  glShaderSource(shader, 1, &shaderSource, NULL);
  glCompileShader(shader);

  GLint success;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    GLchar errorMessage[512];
    glGetShaderInfoLog(shader, 512, NULL, errorMessage);
    log.error("Shader compilation failed for \"%s\": %s", filename.c_str(), errorMessage);
    return 0;
  }
  log.info("Compiled %s \"%s\".", ShaderManager::shaderTypeToString(shaderType).c_str(), filename.c_str());

  return shader;
}

GLuint ShaderManager::linkProgram(GLuint vertexShader, GLuint geomShader, GLuint fragmentShader, engine::Logger& log) {
  GLuint shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  if (0 != geomShader) {
    glAttachShader(shaderProgram, geomShader);
  }
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  GLint success;
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success) {
    GLchar errorMessage[512];
    glGetProgramInfoLog(shaderProgram, 512, NULL, errorMessage);
    log.error("Shader linking failed: %s", errorMessage);
    return 0;
  }

  return shaderProgram;
}
}
