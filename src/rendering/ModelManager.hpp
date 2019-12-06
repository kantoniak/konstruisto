#ifndef RENDERING_MODELMANAGER_HPP
#define RENDERING_MODELMANAGER_HPP

#include <algorithm>
#include <cassert>
#include <string>
#include <unordered_map>
#include <vector>

#include "../opengl/ArrayBuffer.hpp"
#include "../opengl/ElementArrayBuffer.hpp"
#include "../opengl/VertexArray.hpp"
#include "Material.hpp"
#include "Mesh.hpp"
#include "Model.hpp"

namespace rendering {

class ModelManager {

public:
  ModelManager(bool rewrite_indices) noexcept;

  // Materials
  Material& register_material(std::string name, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
                              float shininess) noexcept;
  bool has_material(std::string name) const noexcept;
  Material& get_material(std::string name) noexcept;

  // Models
  Model& register_model(std::string name) noexcept;
  const Model& get_model(std::string name) const noexcept;
  const std::unordered_map<std::string, Model>& get_models() const noexcept;

  // Meshes
  Mesh create_mesh(const Material& material, const std::vector<float>& vertices, const std::vector<float>& normals,
                   const std::vector<uint32_t>& indices) noexcept;
  const std::vector<float>& get_vertices() const noexcept;
  const std::vector<float>& get_normals() const noexcept;
  const std::vector<uint32_t>& get_indices() const noexcept;
  uint32_t get_indices_size() const noexcept;

protected:
  /// Whether to add VBO offset to EBO values
  bool rewrite_indices;

  // Materials
  unsigned int next_material_index;
  std::unordered_map<std::string, Material> materials;

  // Models
  std::unordered_map<std::string, Model> models;

  // Mesh storage
  uint32_t vertices_size;
  uint32_t indices_size;
  std::vector<float> vertices;
  std::vector<float> normals;
  std::vector<uint32_t> indices;
};
}

#endif
