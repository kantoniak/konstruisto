#ifndef RENDERING_SHADERMANAGER_HPP
#define RENDERING_SHADERMANAGER_HPP

#include <array>
#include <fstream>
#include <string>
#include <vector>

#include <GL/glew.h>

#include "../engine/Logger.hpp"

namespace rendering {

class ShaderManager {

public:
  static GLuint compileShader(GLenum shaderType, std::string filename, engine::Logger& log);
  static GLuint linkProgram(GLuint vertexShader, GLuint geomShader, GLuint fragmentShader, engine::Logger& log);

protected:
  static std::string shaderTypeToString(GLenum shaderType);
};
}

#endif
