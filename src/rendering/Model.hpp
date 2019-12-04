#ifndef RENDERING_MODEL_HPP
#define RENDERING_MODEL_HPP

#include <string>
#include <vector>

#include "Mesh.hpp"

namespace rendering {

class Model {

public:
  Model() = delete;
  Model(std::string name) noexcept;

  [[nodiscard]] std::string get_name() const noexcept;
  [[nodiscard]] const std::vector<Mesh>& get_meshes() const noexcept;

  void add_mesh(const Mesh) noexcept;

protected:
  std::string name;
  std::vector<Mesh> meshes;
};
}

#endif
