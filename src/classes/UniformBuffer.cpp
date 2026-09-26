#include "UniformBuffer.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <glad/gl.h>

UniformBuffer::UniformBuffer(std::size_t size) : m_size{size} {
  generateBuffer();

  bind();
  glBufferData(GL_UNIFORM_BUFFER, size, NULL, GL_STATIC_DRAW);
  glBindBufferRange(GL_UNIFORM_BUFFER, 0, *m_ID, 0, size);
}

void UniformBuffer::bind() const { glBindBuffer(GL_UNIFORM_BUFFER, *m_ID); }

void UniformBuffer::generateBuffer() {
  unsigned int ID{};
  glGenBuffers(1, &ID);
  m_ID = std::make_unique<unsigned int>(ID);
}
