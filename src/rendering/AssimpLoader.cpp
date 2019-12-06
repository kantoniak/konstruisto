#include "AssimpLoader.hpp"

namespace rendering {

AssimpLoader::AssimpLoader(ModelManager& model_manager, engine::Logger& log) noexcept
    : log(log), model_manager(model_manager) {
}

bool AssimpLoader::load_model_from_file(const std::string& path) noexcept {

  const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
    log.error("Could not load model from \"%s\": %s", path.c_str(), importer.GetErrorString());

    // Delete orphaned scene
    importer.GetOrphanedScene()->~aiScene();

    return false;
  }

  // Get materials
  std::vector<Material*> materials = read_materials(*scene);

  // Get meshes
  const std::string model_name = std::filesystem::path(path).stem().string();
  Model& model = model_manager.register_model(model_name);
  process_nodes_recursive(*scene->mRootNode, *scene, model, materials);

  return true;
}

std::vector<Material*> AssimpLoader::read_materials(const aiScene& scene) const noexcept {
  std::vector<Material*> materials;

  for (unsigned int i = 0; i < scene.mNumMaterials; i++) {
    const aiMaterial& assimp_material = *scene.mMaterials[i];

    // Original types
    aiString name;
    aiColor3D ambient;
    aiColor3D diffuse;
    aiColor3D specular;
    float shininess;

    // Read values
    assimp_material.Get(AI_MATKEY_NAME, name);
    assimp_material.Get(AI_MATKEY_COLOR_AMBIENT, ambient);
    assimp_material.Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);
    assimp_material.Get(AI_MATKEY_COLOR_SPECULAR, specular);
    assimp_material.Get(AI_MATKEY_SHININESS, shininess);

    // Skip assimp default material
    std::string material_name = to_std_string(name);
    if (material_name == "DefaultMaterial") {
      materials.push_back(nullptr);
      continue;
    }

    // Skip if material with this name exists
    if (model_manager.has_material(material_name)) {
      materials.push_back(std::addressof(model_manager.get_material(material_name)));
      continue;
    }

    Material& new_material = model_manager.register_material(material_name, to_vec3(ambient), to_vec3(diffuse),
                                                             to_vec3(specular), shininess);
    materials.push_back(std::addressof(new_material));
    log.debug(to_string(new_material));
  }

  return materials;
}

Mesh AssimpLoader::read_mesh(const aiMesh& mesh, const std::vector<Material*> materials) const noexcept {
  std::vector<float> vertices;
  std::vector<float> normals;
  std::vector<uint32_t> indices;

  for (unsigned int i = 0; i < mesh.mNumVertices; i++) {
    vertices.push_back(static_cast<float>(mesh.mVertices[i].x));
    vertices.push_back(static_cast<float>(mesh.mVertices[i].y));
    vertices.push_back(static_cast<float>(mesh.mVertices[i].z));

    normals.push_back(static_cast<float>(mesh.mNormals[i].x));
    normals.push_back(static_cast<float>(mesh.mNormals[i].y));
    normals.push_back(static_cast<float>(mesh.mNormals[i].z));

    // TODO(kantoniak): load texture coords

    for (unsigned int i = 0; i < mesh.mNumFaces; i++) {
      aiFace face = mesh.mFaces[i];
      for (unsigned int j = 0; j < face.mNumIndices; j++) {
        indices.push_back(face.mIndices[j]);
      }
    }
  }

  Mesh new_mesh = model_manager.create_mesh(*(materials[mesh.mMaterialIndex]), vertices, normals, indices);
  log.debug(to_string(new_mesh));
  return new_mesh;
}

void AssimpLoader::process_nodes_recursive(const aiNode& node, const aiScene& scene, Model& model,
                                           const std::vector<Material*> materials) const noexcept {
  for (unsigned int i = 0; i < node.mNumMeshes; i++) {
    aiMesh& mesh = *scene.mMeshes[node.mMeshes[i]];
    model.add_mesh(read_mesh(mesh, materials));
  }
  for (unsigned int i = 0; i < node.mNumChildren; i++) {
    process_nodes_recursive(*node.mChildren[i], scene, model, materials);
  }
}

std::string AssimpLoader::to_std_string(const aiString& ai_string) const noexcept {
  return std::string(ai_string.C_Str());
}

glm::vec3 AssimpLoader::to_vec3(aiVector3D& vec) const noexcept {
  return glm::vec3(vec.x, vec.y, vec.z);
}

glm::vec3 AssimpLoader::to_vec3(aiColor3D& color) const noexcept {
  return glm::vec3(color[0], color[1], color[2]);
}

}