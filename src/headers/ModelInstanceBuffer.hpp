#pragma once

#include "Buffer.hpp"
#include "Mesh.hpp"
#include "Stride.hpp"
#include "Utility.hpp"
#include <cstdlib>
#include <memory>
#include <vector>

/// The ModelInstanceBuffer is used by InstancedModel to allow for instancing of
/// the model.
template <typename T> class ModelInstanceBuffer {
public:
  static_assert(sizeof(T) % sizeof(float) == 0);
  /// Creates buffers for instancing.
  /// @param instanceData Vector of data to pass to each instance.
  /// @param dataStride Stride object specifying how to split T into floats,
  /// e.g. 4,4,4,4 for a 4x4 matrix
  ModelInstanceBuffer(const std::vector<T> &instanceData,
                      const Stride &dataStride = {sizeof(T) / sizeof(float)});

  /// Copy constructor for ModelInstanceBuffer.
  ModelInstanceBuffer(const ModelInstanceBuffer &buffer) = default;
  /// Move constructor for ModelInstanceBuffer.
  ModelInstanceBuffer(ModelInstanceBuffer &&buffer) = default;
  /// Destructor for ModelInstanceBuffer. Frees memory held by buffers, if not
  /// held by other instances.
  ~ModelInstanceBuffer() { free(m_instanceBuffer); }

  /// Copy assignment operator for ModelInstanceBuffer.
  ModelInstanceBuffer &operator=(const ModelInstanceBuffer &buffer);
  /// Move assignment operator for ModelInstanceBuffer.
  ModelInstanceBuffer &operator=(ModelInstanceBuffer &&buffer) noexcept;

  /// Assigns instance data to a mesh.
  /// @param mesh the Mesh to assign the data to.
  /// @param start_index The attribute index to start on, after the mesh has
  /// assigned its vertex attributes.
  /// @param stride Stride object specifying how to split T into floats.
  void assignData(const Mesh &mesh, int start_index, const Stride &stride);

  /// Draws all instances of the mesh to the screen.
  /// @param mesh The mesh to draw.
  /// @param shader Pointer to the shader to be used with this mesh.
  /// @param skipTextures Controls whether to skip textures.
  /// @param drawMode Specifies how to draw the vertices in the buffer. Usually
  /// drawn as triangles by default.
  void draw(const Mesh &mesh, const Shader *shader, bool skipTextures = false,
            unsigned int drawMode = GL_TRIANGLES) const;

private:
  std::shared_ptr<unsigned int> m_instanceBuffer{new unsigned int{0}};
  int m_instanceSize{0};

  void generateBuffer(const std::vector<T> &instanceData);
  static void free(std::shared_ptr<unsigned int> instanceBuffer);
};

#include "ModelInstanceBuffer.tpp"
