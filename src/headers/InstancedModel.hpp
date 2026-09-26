#pragma once

#include "FileManager.hpp"
#include "Mesh.hpp"
#include "Model.hpp"
#include "ModelData.hpp"
#include "ModelInstanceBuffer.hpp"
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

/// Variation of Model which allows for instancing.
template <typename T> class InstancedModel : public Model {
public:
  /// Creates and processes a model file for drawing.
  /// @param path Name of the model file.
  /// @param shader Pointer to the shader to use to draw the model.
  /// @param instanceData Vector of data to pass to each instance.
  /// @param dataStride Stride object specifying how to split T into floats,
  /// e.g. 4,4,4,4 for a 4x4 matrix
  InstancedModel(std::string_view path, Shader *shader,
                 std::vector<T> &instanceData,
                 const Stride &dataStride = {sizeof(T) / sizeof(float)});

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

private:
  // model data
  ModelInstanceBuffer<T> m_instanceBuffer;
};

#include "InstancedModel.tpp"
