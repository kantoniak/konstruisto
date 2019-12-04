#include "Material.hpp"

namespace rendering {
Material::Material(std::string name, unsigned int index, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
                   float shininess) noexcept
    : name(name), index(index), ambient(ambient), diffuse(diffuse), specular(specular), shininess(shininess) {
}

std::string Material::get_name() const noexcept {
  return name;
}

unsigned int Material::get_index() const noexcept {
  return index;
}

void Material::set_in(const opengl::ShaderProgram& shader) const noexcept {
  shader.submit("material.ambient", ambient);
  shader.submit("material.diffuse", diffuse);
  shader.submit("material.specular", specular);
  shader.submit("material.shininess", shininess * 128);
}
}