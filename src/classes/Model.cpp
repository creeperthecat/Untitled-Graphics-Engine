#include "Model.hpp"
#include "FileManager.hpp"
#include "Mesh.hpp"
#include "ModelData.hpp"
#include "Shader.hpp"
#include "assimp/Importer.hpp"
#include "assimp/material.h"
#include "assimp/mesh.h"
#include "assimp/postprocess.h"
#include "assimp/scene.h"
#include "assimp/types.h"
#include <cstring>
#include <glm/ext/vector_float3.hpp>
#include <glm/fwd.hpp>
#include <iostream>
#include <string_view>
#include <vector>

using namespace ModelData;
void Model::draw(bool skipTextures, unsigned int drawMode) const {
  getShader()->use();
  for (auto &mesh : m_meshes) {
    mesh.draw(getShader(), skipTextures, drawMode);
  }
}

void Model::loadModel(std::string_view path) {
  Assimp::Importer import{};
  const aiScene *scene{import.ReadFile(
      path.data(),
      aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs)};

  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
      !scene->mRootNode) {
    std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << "\n";
    return;
  }

  m_directory = FileManager::getFileDirectory(path);
  processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode *node, const aiScene *scene) {
  for (unsigned int i{0}; i < node->mNumMeshes; ++i) {
    aiMesh *mesh{scene->mMeshes[node->mMeshes[i]]};
    m_meshes.push_back(processMesh(mesh, scene));
  }
  for (unsigned int i{0}; i < node->mNumChildren; ++i) {
    processNode(node->mChildren[i], scene);
  }
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene) {
  std::vector<Vertex> vertices{};
  std::vector<unsigned int> indices{};
  std::vector<Texture> textures{};

  for (unsigned int i{0}; i < mesh->mNumVertices; ++i) {
    Vertex vertex{};
    glm::vec3 vector{};

    vector.x = mesh->mVertices[i].x;
    vector.y = mesh->mVertices[i].y;
    vector.z = mesh->mVertices[i].z;
    vertex.position = vector;

    vector.x = mesh->mNormals[i].x;
    vector.y = mesh->mNormals[i].y;
    vector.z = mesh->mNormals[i].z;
    vertex.normal = vector;

    if (mesh->mTextureCoords[0]) {
      vector.x = mesh->mNormals[i].x;

      glm::vec2 vector{};
      vector.x = mesh->mTextureCoords[0][i].x;
      vector.y = mesh->mTextureCoords[0][i].y;
      vertex.texCoords = vector;
    } else {
      vertex.texCoords = glm::vec2{0.0f, 0.0f};
    }
    vertices.push_back(vertex);
  }

  for (unsigned int i{0}; i < mesh->mNumFaces; ++i) {
    aiFace face{mesh->mFaces[i]};
    for (unsigned int index{0}; index < face.mNumIndices; ++index)
      indices.push_back(face.mIndices[index]);
  }

  if (mesh->mMaterialIndex >= 0) {
    aiMaterial *material{scene->mMaterials[mesh->mMaterialIndex]};
    std::vector<Texture> diffuseMaps{loadMaterialTextures(
        material, aiTextureType_DIFFUSE, "texture_diffuse")};
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    std::vector<Texture> specularMaps{loadMaterialTextures(
        material, aiTextureType_SPECULAR, "texture_specular")};
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
  }

  return Mesh{vertices, indices, textures};
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial *mat,
                                                 aiTextureType type,
                                                 std::string_view typeName) {
  std::vector<Texture> textures{};
  for (unsigned int i{0}; i < mat->GetTextureCount(type); ++i) {
    aiString string{};
    mat->GetTexture(type, i, &string);
    bool skip{isLoaded(string, textures)};

    if (!skip) {
      Texture texture{};
      texture.id = FileManager::loadTexture(string.C_Str(), m_directory);
      texture.type = typeName;
      texture.path = string.C_Str();
      textures.push_back(texture);
      m_texturesLoaded.push_back(texture);
    }
  }

  return textures;
}

bool Model::isLoaded(const aiString &string, std::vector<Texture> &textures) {
  for (unsigned int i{0}; i < m_texturesLoaded.size(); ++i) {
    if (std::strcmp(m_texturesLoaded[i].path.data(), string.C_Str()) == 0) {
      textures.push_back(m_texturesLoaded[i]);
      return true;
    }
  }
  return false;
}
