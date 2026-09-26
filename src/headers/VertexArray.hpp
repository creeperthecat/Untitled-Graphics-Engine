#pragma once

#include "Buffer.hpp"
#include "ModelData.hpp"
#include "Shape.hpp"
#include "Stride.hpp"
#include "Utility.hpp"
#include <memory>
#include <vector>

/// Implementation of Buffer using a VAO and VBO.
/// Most basic and common implementation of vertex buffering.
/// Can be inherited from to modify behaviour (see EntityBuffer)
/// Used by Primitive instances.
class VertexArray : public Buffer {
public:
  /// Construct a VertexArray using a Shape object.
  /// @param shape Constructed Shape object used for processing vertices.
  VertexArray(const Shape &shape);

  /// Construct a VertexArray using a vector of floats and a Stride.
  /// Equivalent to constructing and passing a Shape.
  /// @param shape Flattened array of vertices stored as a sequence of floats.
  /// @param stride Stride to define vertex attributes.
  VertexArray(const std::vector<float> &shape, const Stride &stride)
      : VertexArray{Shape{shape, stride}} {}

  /// Copy constructor for VertexArray.
  VertexArray(const VertexArray &buffer) = default;
  /// Move constructor for VertexArray.
  VertexArray(VertexArray &&buffer) noexcept = default;
  /// Destructor for VertexArray, frees memory held by VAO and VBO, if not used
  /// by another instance.
  virtual ~VertexArray() noexcept override { free(m_VAO, m_VBO); }

  /// Copy assignment operator for VertexArray.
  VertexArray &operator=(const VertexArray &buffer);
  /// Move assignment operator for VertexArray.
  VertexArray &operator=(VertexArray &&buffer) noexcept;

  /// Clones this instance of VertexArray.
  /// @return Pointer to a new VertexArray copied from this VertexArray.
  virtual VertexArray *clone() const override;

  /// Draws vertices stored by instance to the screen.
  /// @param drawMode Specifies how to draw the vertices in the buffer. Usually
  /// drawn as triangles by default.
  virtual void draw(unsigned int drawMode = GL_TRIANGLES) const override;

protected:
  /// Default constructor for derived classes to use.
  VertexArray() = default;
  /// Vertex Buffer Object for storing vertex data.
  unsigned int m_VBO{};

  /// Total number of vertices in shape, needed for draw call.
  int m_vertexCount{0};

  /// Assign vertex attributes using the supplied Stride.
  /// @param stride Stride object supplied in constructor.
  void assignAttributes(const Stride &stride);
  /// Generate the VAO and VBO and set buffer data.
  /// @param shape Shape object supplied in constructor.
  void generateBuffer(const Shape &shape);
  /// Initialise buffers using a Shape.
  /// @param shape Shape object supplied in constructor.
  void bindShape(const Shape &shape);

private:
  /// Free memory held by VAO and VBO, if not held by another Buffer.
  static void free(std::shared_ptr<unsigned int> VAO, unsigned int VBO);
};
