#pragma once

#include "Shape.hpp"

/// Helper class inheriting from Shape for use with EntityBuffer objects to
/// store and process vertices.
/// Functionally equivalent to Shape, but now stores indices used for indexed
/// drawing.
class IndexedShape : public Shape {
public:
  /// Type alias for indices.
  using Indices = std::vector<unsigned int>;

  /// Move constructor using a 2D array of vertices
  /// @param shape List of vertices to be moved into this class for processing.
  /// @param stride Stride to define vertex attributes.
  /// @param indices Vector defining use and order of vertices.
  IndexedShape(std::vector<Vertex> &&shape, const Stride &stride,
               const Indices &indices)
      : Shape{std::move(shape), stride}, m_indices{indices} {}
  /// Copy constructor using a 2D array of vertices
  /// @param shape List of vertices to be moved into this class for processing.
  /// @param stride Stride to define vertex attributes.
  /// @param indices Vector defining use and order of vertices.
  IndexedShape(const std::vector<Vertex> &shape, const Stride &stride,
               const Indices &indices)
      : Shape{shape, stride}, m_indices{indices} {}

  /// Move constructor using a flattened array of floats
  /// @param shape Flattened array of vertices stored as a sequence of floats.
  /// @param stride Stride to define vertex attributes.
  /// @param indices Vector defining use and order of vertices.
  IndexedShape(std::vector<float> &&shape, const Stride &stride,
               const Indices &indices);
  /// Copy construct using a flattened array of floats
  /// @param shape Flattened array of vertices stored as a sequence of floats.
  /// @param stride Stride to define vertex attributes.
  /// @param indices Vector defining use and order of vertices.
  IndexedShape(const std::vector<float> &shape, const Stride &stride,
               const Indices &indices = {});

  /// Return internal indices array.
  const unsigned int *getIndices() const { return m_indices.data(); }
  /// Get total size in bytes of indices.
  std::size_t indicesSize() const {
    return m_indices.size() * sizeof(unsigned int);
  }

private:
  /// Internal indices used in indexed drawing.
  Indices m_indices{};
};
