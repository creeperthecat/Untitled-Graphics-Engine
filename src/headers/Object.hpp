#pragma once

#include "Buffer.hpp"
#include "Shader.hpp"
#include "Utility.hpp"
#include <memory>
#include <vector>

/// Abstract class for creating and drawing objects.
/// Used in Renderer.
/// Can only be initialised after initialisation of Renderer.
/// Derived classes must implement the draw function.
class Object {
public:
  /// Returns a pointer to the shader used by this object.
  const Shader *getShader() const;
  /// Sets the Shader used by this object.
  void setShader(Shader *shader);
  /// Sets object ID for use in Renderer.
  void setID(unsigned int ID) { m_ID = ID; }
  /// Gets object ID for use in Renderer.
  unsigned int getID() { return m_ID; }
  /// Gets VAO used by owned Buffer.
  unsigned int getVAO() { return m_buffer->getVAO(); }

  /// Draws object to the screen.
  /// @param drawMode Specifies how to draw the vertices in the buffer. Usually
  virtual void draw(unsigned int drawMode = GL_TRIANGLES) const = 0;

  /// Virtual destructor for Object.
  virtual ~Object() noexcept = default;

protected:
  /// Default constructor for use in derived classes.
  Object() = default;
  // Internal object properties

  /// Object ID used by Renderer to identify objects.
  unsigned int m_ID{};
  /// Buffer instance needed to draw vertices to the screen.
  std::shared_ptr<Buffer> m_buffer{nullptr};
  /// Shader instance to determine how to draw object.
  std::shared_ptr<Shader> m_shader{nullptr};
};
