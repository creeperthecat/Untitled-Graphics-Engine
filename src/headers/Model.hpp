#pragma once

#include "FileManager.hpp"
#include "Mesh.hpp"
#include "ModelData.hpp"
#include "Object.hpp"
#include "Shader.hpp"
#include "TexParams.hpp"
#include "assimp/Importer.hpp"
#include "assimp/material.h"
#include "assimp/mesh.h"
#include "assimp/scene.h"
#include "assimp/types.h"
#include <string_view>
#include <vector>

/// The Model class implements drawing a 3d model file to the screen.
/// Uses assimp to load and process the obj file.
class Model : public Object {
public:
  /// Creates and processes a model file for drawing.
  /// @param path Name of the model file.
  /// @param shader Pointer to the shader to use to draw the model.
  Model(std::string_view path, Shader *shader) {
    setShader(shader);
    loadModel(FileManager::getModelFilePath(path));
  }

  /// Draws model to the screen.
  /// @param drawMode Specifies how to draw the vertices in the buffer. Usually
  /// drawn as triangles by default.
  void draw(unsigned int drawMode = GL_TRIANGLES) const override {
    draw(false, drawMode);
  }

  /// Draws model to the screen.
  /// @param skipTextures Controls whether to skip textures during rendering.
  /// @param drawMode Specifies how to draw the vertices in the buffer. Usually
  /// drawn as triangles by default.
  void draw(bool skipTextures, unsigned int drawMode = GL_TRIANGLES) const;

protected:
  /// Vector of meshes in the model.
  std::vector<Mesh> m_meshes{};
  /// Vector of the textures in the model which have been successfully loaded.
  std::vector<ModelData::Texture> m_texturesLoaded{};

private:
  // model data
  std::string m_directory{};

  void loadModel(std::string_view path);
  void processNode(aiNode *node, const aiScene *scene);
  Mesh processMesh(aiMesh *mesh, const aiScene *scene);
  std::vector<ModelData::Texture>
  loadMaterialTextures(aiMaterial *mat, aiTextureType type,
                       std::string_view typeName);
  bool isLoaded(const aiString &string,
                std::vector<ModelData::Texture> &textures);
};
