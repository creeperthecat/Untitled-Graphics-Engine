#pragma once

#include "ModelData.hpp"
#include "Shape.hpp"
#include "Stride.hpp"
#include "Utility.hpp"
#include <memory>
#include <vector>

/// @brief Abstract class for processing and drawing vertices to the screen.
/// @details Stores a VAO as a shared pointer for use within draw calls.
/// Used by all Object instances.
class Buffer {
public:
  /// Draws vertices stored by instance to the screen.
  /// @param drawMode Specifies how to draw the vertices in the buffer. Usually
  /// drawn as triangles by default.
  virtual void draw(unsigned int drawMode = GL_TRIANGLES) const = 0;

  /// Clones this instance of Buffer.
  /// @return Pointer to a new Buffer copied from this Buffer.
  virtual Buffer *clone() const = 0;
  /// Virtual destructor for Buffer.
  virtual ~Buffer() noexcept = default;

  /// Gets the Vertex Array Object held by this instance.
  unsigned int getVAO() const { return *m_VAO; }

protected:
  /// Default constructor for derived classes to use.
  Buffer() = default;
  /// Shared pointer to Vertex Array %Object. Must be initialised by derived
  /// classes.
  std::shared_ptr<unsigned int> m_VAO{std::make_shared<unsigned int>(0)};
};
