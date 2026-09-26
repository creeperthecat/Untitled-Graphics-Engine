#pragma once

#include "Buffer.hpp"
#include "ModelData.hpp"
#include "Shape.hpp"
#include "Stride.hpp"
#include "Utility.hpp"
#include <memory>
#include <vector>

/// Implementation of Buffer which is used for Mesh instances.
class MeshBuffer : public Buffer {
public:
  /// Construct a MeshBuffer using model data.
  /// @param vertices Vertex data from model.
  /// @param indices Indices from model.
  MeshBuffer(const std::vector<ModelData::Vertex> &vertices,
             const std::vector<unsigned int> &indices);

  /// Copy constructor for MeshBuffer.
  MeshBuffer(const MeshBuffer &buffer) = default;
  /// Move constructor for MeshBuffer.
  MeshBuffer(MeshBuffer &&buffer) noexcept = default;
  /// Destructor for MeshBuffer. frees memory held, if not used by other
  /// instances.
  ~MeshBuffer() noexcept override { free(m_VAO, m_VBO, m_EBO); }

  /// Copy assignment operator for MeshBuffer.
  const MeshBuffer &operator=(const MeshBuffer &buffer);
  /// Move assignment operator for MeshBuffer.
  const MeshBuffer &operator=(MeshBuffer &&buffer) noexcept;

  /// Clones this instance of MeshBuffer.
  /// @return Pointer to a new MeshBuffer copied from this InstancedArray.
  MeshBuffer *clone() const override;

  /// Function which binds vertex array and draws to screen.
  void draw(unsigned int drawMode = GL_TRIANGLES) const override;

private:
  // Shared pointers for arrays
  unsigned int m_VBO{};
  unsigned int m_EBO{};

  int m_vertexCount = 0;

  // Functions for initialisation.
  void generateBuffer();
  void assignAttributes();
  void bindMesh(const std::vector<ModelData::Vertex> &vertices,
                const std::vector<unsigned int> &indices);

  /// Frees memory held by internal arrays, if not owned by another Buffer.
  static void free(std::shared_ptr<unsigned int> VAO, unsigned int VBO,
                   unsigned int EBO);
};
