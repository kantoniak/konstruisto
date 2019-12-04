#include "Model.hpp"

#include <utility>

namespace rendering {
Model::Model(std::string name) noexcept : name(std::move(name)) {
}

std::string Model::get_name() const noexcept {
  return name;
}

const std::vector<Mesh>& Model::get_meshes() const noexcept {
  return meshes;
}

void Model::add_mesh(const Mesh mesh) noexcept {
  meshes.push_back(std::move(mesh));
}
}