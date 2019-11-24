#ifndef RENDERING_SHADERMANAGER_HPP
#define RENDERING_SHADERMANAGER_HPP

#include <array>
#include <fstream>
#include <optional>
#include <string>
#include <vector>

#include <GL/glew.h>

#include "../engine/Logger.hpp"
#include "../opengl/Shader.hpp"
#include "../opengl/ShaderProgram.hpp"

namespace rendering {

using namespace opengl;

class ShaderManager {

public:
  static std::optional<Shader> compileShader(Shader::ShaderType shaderType, std::string filename, engine::Logger& log);

  template <size_t n>
  static std::optional<ShaderProgram> linkProgram(const std::array<Shader, n>& shaders, engine::Logger& log);
};
}

#endif
