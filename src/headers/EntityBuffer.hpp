#pragma once

#include "IndexedShape.hpp"
#include "Shape.hpp"
#include "Stride.hpp"
#include "Utility.hpp"
#include "VertexArray.hpp"
#include <memory>
#include <vector>

/// Implementation of Buffer using a Entity Buffering.
/// Used for indexed drawing.
class EntityBuffer : public VertexArray {
public:
  /// Constructs an EntityBuffer using an IndexedShape object.
  /// @param shape Constructed Shape object used for processing vertices.
  EntityBuffer(const IndexedShape &shape);

  /// Override to avoid needing to construct an IndexedShape
  /// Equivalent to constructing and passing an IndexedShape.
  /// @param shape Flattened array of vertices stored as a sequence of floats.
  /// @param stride Stride to define vertex attributes.
  /// @param indices List of indices which defines the order of vertices to be
  /// drawn.
  EntityBuffer(const std::vector<float> &shape, const Stride &stride,
               const IndexedShape::Indices &indices)
      : EntityBuffer{IndexedShape{shape, stride, indices}} {}

  /// Copy constructor for EntityBuffer.
  EntityBuffer(const EntityBuffer &buffer) = default;
  /// Move constructor for EntityBuffer.
  EntityBuffer(EntityBuffer &&buffer) noexcept = default;
  /// Destructor for Entity, frees memory held by EBO, if not held by another
  /// instance.
  ~EntityBuffer() noexcept override { free(m_VAO, m_EBO); }

  /// Copy assignment operator for EntityBuffer.
  EntityBuffer &operator=(const EntityBuffer &buffer);
  /// Move assignment operator for EntityBuffer.
  EntityBuffer &operator=(EntityBuffer &&buffer) noexcept;

  /// Clones this instance of EntityBuffer.
  /// @return Pointer to a new EntityBuffer copied from this EntityBuffer.
  EntityBuffer *clone() const override;

private:
  /// Element Buffer Object for storing element data.
  unsigned int m_EBO{};

  /// Generates the EBO and sets buffer data.
  void generateBuffer(const IndexedShape &shape);
  /// Frees memory held by EBO, if not held by another Buffer.
  static void free(std::shared_ptr<unsigned int> VAO, unsigned int EBO);
};
