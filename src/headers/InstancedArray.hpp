#pragma once

#include "Shape.hpp"
#include "Stride.hpp"
#include "Utility.hpp"
#include "VertexArray.hpp"
#include <memory>
#include <vector>

/// Implementation of Buffer using instancing.
/// Useful for drawing many copies of objects.
template <typename T> class InstancedArray : public VertexArray {
public:
  static_assert(sizeof(T) % sizeof(float) == 0);
  /// Constructs an InstancedArray using a Shape object.
  /// @param shape Constructed Shape object used for processing vertices.
  /// @param instanceData Vector of data to pass to each instance.
  /// @param dataStride Stride object specifying how to split T into floats,
  /// e.g. 4,4,4,4 for a 4x4 matrix
  InstancedArray(const Shape &shape, const std::vector<T> &instanceData,
                 const Stride &dataStride = {sizeof(T) / sizeof(float)});

  /// Override to avoid needing to construct a Shape
  /// Equivalent to constructing and passing a Shape.
  /// @param shape Flattened array of vertices stored as a sequence of floats.
  /// @param stride Stride to define vertex attributes.
  /// @param instanceData Vector of data to pass to each instance.
  /// @param dataStride Stride object specifying how to split T into floats,
  /// e.g. 4,4,4,4 for a 4x4 matrix
  InstancedArray(const std::vector<float> &shape, const Stride &stride,
                 const std::vector<T> &instanceData,
                 const Stride &dataStride = {sizeof(T) / sizeof(float)})
      : InstancedArray{Shape{shape, stride}, instanceData, dataStride} {}

  /// Copy constructor for InstancedArray.
  InstancedArray(const InstancedArray &buffer) = default;
  /// Move constructor for InstancedArray.
  InstancedArray(InstancedArray &&buffer) noexcept = default;
  /// Destructor for InstancedArray, frees memory held, if not being used by
  /// another instance.
  ~InstancedArray() noexcept override { free(m_VAO, m_instanceBuffer); }

  /// Copy assignment operator for InstancedArray.
  InstancedArray &operator=(const InstancedArray &buffer);
  /// Move assignment operator for InstancedArray.
  InstancedArray &operator=(InstancedArray &&buffer) noexcept;

  /// Clones this instance of InstancedArray.
  /// @return Pointer to a new InstancedArray copied from this InstancedArray.
  InstancedArray *clone() const override;

  /// Function which binds vertex array and draws all instances to the screen.
  /// @param drawMode Specifies how to draw the vertices in the buffer. Usually
  /// drawn as triangles by default.
  void draw(unsigned int drawMode = GL_TRIANGLES) const override;

private:
  // Shared pointers for arrays
  unsigned int m_instanceBuffer{};
  int m_instanceSize{0};

  // Functions for initialisation.
  void generateBuffer(const Shape &shape, const std::vector<T> &instanceData);

  void bindShape(const Shape &shape, const std::vector<T> &instanceData,
                 const Stride &dataStride);
  void assignData(int start_index, const Stride &stride);

  /// Frees memory held by internal arrays, if not owned by another Buffer.
  static void free(std::shared_ptr<unsigned int> VAO,
                   unsigned int instanceBuffer);
};

#include "InstancedArray.tpp"
