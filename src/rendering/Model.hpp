#ifndef RENDERING_MODEL_HPP
#define RENDERING_MODEL_HPP

#include <string>
#include <vector>

#include "Material.hpp"
#include "Mesh.hpp"

namespace rendering {

class Model {

public:
  Model() = delete;
  Model(std::string name) noexcept;

  [[nodiscard]] std::string get_name() const noexcept;
  [[nodiscard]] const std::vector<Mesh>& get_meshes() const noexcept;
  [[nodiscard]] const std::vector<std::reference_wrapper<const Material>>& get_materials() const noexcept;

  void add_mesh(const Mesh, const Material& material) noexcept;

protected:
  std::string name;
  std::vector<Mesh> meshes;
  std::vector<std::reference_wrapper<const Material>> materials;
};
}

#endif
