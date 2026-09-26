#pragma once

#include "Buffer.hpp"
#include "Object.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "Utility.hpp"
#include "VertexArray.hpp"
#include <algorithm>
#include <memory>
#include <vector>

/// The Primitive class provides functionality to render basic shapes to the
/// screen.
class Primitive : public Object {
public:
  /// Construct a Primitive and prepare for drawing to the screen.
  /// @param buffer Pointer to a VertexArray to draw vertices to the screen.
  ///@param shader Pointer to the shader for this primitive to use.
  /// @param texture Optional textures to use for this primitive.
  Primitive(VertexArray *buffer, Shader *shader, const Texture &texture = {})
      : m_texture{texture} {
    m_buffer = std::shared_ptr<Buffer>(buffer);
    m_shader = std::shared_ptr<Shader>(shader);
  }

  /// Returns Texture held by object.
  const Texture &getTexture() const;

  /// Draws the primitive to the screen.
  /// @param drawMode Specifies how to draw the vertices in the buffer. Usually
  /// drawn as triangles by default.
  void draw(unsigned int drawMode = GL_TRIANGLES) const override;

  /// Destructor for Primitive.
  ~Primitive() noexcept override = default;

private:
  // Internal object properties
  Texture m_texture{};
};
