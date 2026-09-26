#pragma once

#include "MeshBuffer.hpp"
#include "ModelData.hpp"
#include "Shader.hpp"
#include "Utility.hpp"
#include <vector>

/// The Mesh class is used by Model instances to help process model files, by
/// breaking them up into meshes.
class Mesh {
public:
  /// Construct a new Mesh.
  /// @param vertices Vector of vertices for this mesh.
  /// @param indices Vector of indices for this mesh.
  /// @param textures Vector of texture data for this mesh.
  Mesh(const std::vector<ModelData::Vertex> &vertices,
       const std::vector<unsigned int> &indices,
       const std::vector<ModelData::Texture> &textures)
      : m_vertices{vertices}, m_indices{indices}, m_textures{textures},
        m_buffer{vertices, indices} {}

  /// Draws the mesh to the screen.
  /// @param shader Pointer to the shader to use for drawing.
  /// @param skipTextures Controls whether to skip textures.
  /// @param drawMode Specifies how to draw the vertices in the buffer. Usually
  /// drawn as triangles by default.
  void draw(const Shader *shader, bool skipTextures = false,
            unsigned int drawMode = GL_TRIANGLES) const;

  /// Binds textures to be drawn to the screen.
  /// @param shader Pointer to the shader to be used with this texture.
  /// @param skipTextures Controls whether to skip textures.
  void bindTextures(const Shader *shader, bool skipTextures = false) const;

  /// Returns the VAO owned by this mesh.
  unsigned int getVAO() const { return m_buffer.getVAO(); }

  /// Returns the number of indices used by this mesh.
  int indicesSize() const { return m_indices.size(); }

private:
  // mesh data
  std::vector<ModelData::Vertex> m_vertices{};
  std::vector<unsigned int> m_indices{};
  std::vector<ModelData::Texture> m_textures{};

  MeshBuffer m_buffer;
};
