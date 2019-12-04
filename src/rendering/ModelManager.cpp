#include "ModelManager.hpp"

namespace rendering {
ModelManager::ModelManager(bool rewrite_indices) noexcept
    : next_material_index(0), vertices_size(0), indices_size(0), rewrite_indices(rewrite_indices) {
}

Material& ModelManager::register_material(std::string name, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
                                          float shininess) noexcept {
  auto it = materials.find(name);
  if (it != materials.end()) {
    return (*it).second;
  }

  Material& new_material =
      (*materials.emplace(name, Material(name, next_material_index, ambient, diffuse, specular, shininess)).first)
          .second;
  next_material_index++;
  return new_material;
}

Model& ModelManager::register_model(std::string name) noexcept {
  auto it = models.find(name);
  if (it != models.end()) {
    return (*it).second;
  }

  Model& new_model = (*models.emplace(name, Model(name)).first).second;
  return new_model;
}

const Model& ModelManager::get_model(std::string name) const noexcept {
  return models.at(name);
}

const std::unordered_map<std::string, Model>& ModelManager::get_models() const noexcept {
  return models;
}

Mesh ModelManager::create_mesh(const Material& material, const std::vector<float>& vertices,
                               const std::vector<float>& normals, const std::vector<uint32_t>& indices) noexcept {
  const size_t added_vertices_size = vertices.size();
  const size_t added_indices_size = indices.size();
  const size_t previous_vertices_size = this->vertices_size;
  const size_t previous_indices_size = this->indices_size;
  assert(added_vertices_size == normals.size());
  assert(added_vertices_size % 3 == 0);
  assert(added_indices_size % 3 == 0);

  Mesh result(added_indices_size, previous_vertices_size / 3, previous_indices_size, material);

  this->vertices.insert(this->vertices.end(), vertices.begin(), vertices.end());
  this->normals.insert(this->normals.end(), normals.begin(), normals.end());
  this->vertices_size += added_vertices_size;

  this->indices.insert(this->indices.end(), indices.begin(), indices.end());
  if (rewrite_indices) {
    std::for_each(this->indices.begin() + previous_indices_size, this->indices.end(),
                  [&previous_vertices_size](uint32_t& v) { v += previous_vertices_size / 3; });
  }
  this->indices_size += added_indices_size;

  return result;
}

const std::vector<float>& ModelManager::get_vertices() const noexcept {
  return vertices;
}

const std::vector<float>& ModelManager::get_normals() const noexcept {
  return normals;
}

const std::vector<uint32_t>& ModelManager::get_indices() const noexcept {
  return indices;
}

uint32_t ModelManager::get_indices_size() const noexcept {
  return indices_size;
}
}