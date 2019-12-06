#ifndef RENDERING_ASSIMPLOADER_HPP
#define RENDERING_ASSIMPLOADER_HPP

#include <filesystem>
#include <string>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include "../engine/Logger.hpp"
#include "Mesh.hpp"
#include "Model.hpp"
#include "ModelManager.hpp"

namespace rendering {

class AssimpLoader {

public:
  AssimpLoader(ModelManager& model_manager, engine::Logger& log) noexcept;

  bool load_model_from_file(const std::string& path) noexcept;

protected:
  engine::Logger& log;
  Assimp::Importer importer;
  ModelManager& model_manager;

  std::vector<Material*> read_materials(const aiScene& scene) const noexcept;
  Mesh read_mesh(const aiMesh& mesh, const std::vector<Material*> materials) const noexcept;

  void process_nodes_recursive(const aiNode& node, const aiScene& scene, Model& model,
                               const std::vector<Material*> materials) const noexcept;

  std::string to_std_string(const aiString& ai_string) const noexcept;
  glm::vec3 to_vec3(aiVector3D& vec) const noexcept;
  glm::vec3 to_vec3(aiColor3D& color) const noexcept;
};
}

#endif
