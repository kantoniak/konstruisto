#ifndef RENDERING_MATERIAL_HPP
#define RENDERING_MATERIAL_HPP

#include <string>

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

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

  friend std::ostream& operator<<(std::ostream& os, const Material& material) noexcept;
  friend std::string to_string(const Material& material) noexcept;

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
