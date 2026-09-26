#pragma once

#include "Stride.hpp"
#include <cassert>
#include <cstddef>
#include <vector>

/// Helper class for use with Buffer objects to store and process vertices.
/// Invokes move semantics where possible (using std::vector).
class Shape {
public:
  /// Type alias for a single vertex in a shape.
  using Vertex = std::vector<float>;

  /// Move constructor using a 2D array of vertices
  /// @param shape List of vertices to be moved into this class for processing.
  /// @param stride Stride to define vertex attributes.
  Shape(std::vector<Vertex> &&shape, const Stride &stride)
      : Shape{generateShape(std::move(shape)), stride} {}
  /// Copy constructor using a 2D array of vertices
  /// @param shape List of vertices to be moved into this class for processing.
  /// @param stride Stride to define vertex attributes.
  Shape(const std::vector<Vertex> &shape, const Stride &stride)
      : Shape{generateShape(shape), stride} {}

  /// Move constructor using a flattened array of floats
  /// @param shape Flattened array of vertices stored as a sequence of floats.
  /// @param stride Stride to define vertex attributes.
  Shape(std::vector<float> &&shape, const Stride &stride);
  /// Copy construct using a flattened array of floats
  /// @param shape Flattened array of vertices stored as a sequence of floats.
  /// @param stride Stride to define vertex attributes.
  Shape(const std::vector<float> &shape, const Stride &stride);

  /// Specialised copy constructor to change definition of stride and
  /// attributes.
  /// @param shape Shape to copy from.
  /// @param stride Stride to define vertex attributes.
  Shape(const Shape &shape, const Stride &stride);

  // Helper functions to work with Buffer instances.

  /// Get number of vertices in shape. Needed for Buffer to draw the shape.
  int vertexCount() const;
  /// Get total size in bytes of the shape.
  std::size_t size() const;

  /// Return internal vertex array of shape.
  const float *getVertices() const;
  /// Return Stride, which stores attributes of shape.
  const Stride &getStride() const;

private:
  // Internal data

  // Flattened array of vertices to define shape.
  std::vector<float> m_shape{};
  // Stride object to define vertex attributes.
  Stride m_stride;

  // Constructs the shape using copy of vertices
  std::vector<float> generateShape(const std::vector<Vertex> &shape) const;
  // Constructs the shape by moving vertices.
  std::vector<float> generateShape(std::vector<Vertex> &&shape) const;
};
