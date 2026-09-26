#include "Primitive.hpp"
#include "VertexArray.hpp"
#include <algorithm>
#include <cassert>
#include <memory>

const Texture &Primitive::getTexture() const { return m_texture; }

void Primitive::draw(unsigned int drawMode) const {
  m_texture.use();
  m_shader->use();
  m_buffer->draw(drawMode);
}
