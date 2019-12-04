#ifndef RENDERING_MATERIAL_HPP
#define RENDERING_MATERIAL_HPP

#include <string>

#include <glm/glm.hpp>

#include "../opengl/ShaderProgram.hpp"

namespace rendering {

class Material {

public:
  Material() = delete;
  Material(std::string name, unsigned int index, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
           float shininess) noexcept;

  [[nodiscard]] std::string get_name() const noexcept;
  [[nodiscard]] unsigned int get_index() const noexcept;
  void set_in(const opengl::ShaderProgram& shader) const noexcept;

protected:
  std::string name;
  unsigned int index;

  glm::vec3 ambient;
  glm::vec3 diffuse;
  glm::vec3 specular;
  float shininess;
};
}

#endif
