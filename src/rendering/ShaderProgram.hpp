#ifndef RENDERING_SHADERPROGRAM_HPP
#define RENDERING_SHADERPROGRAM_HPP

#include <array>
#include <cstdint>
#include <fstream>
#include <string>
#include <vector>

#include <GL/glew.h>

#include "../engine/Logger.hpp"
#include "Shader.hpp"

namespace rendering {

class ShaderProgram {

public:
  ShaderProgram() noexcept;
  ShaderProgram(uint32_t id) noexcept;

  [[nodiscard]] uint32_t get_id() const noexcept;

  void attach(const Shader& shader);
  bool link();
  [[nodiscard]] const std::vector<char> get_info_log();

  void use();
  void delete_program();

protected:
  uint32_t id;
};

}

#endif
