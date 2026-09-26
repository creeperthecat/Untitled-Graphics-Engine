#include "EntityBuffer.hpp"
#include "VertexArray.hpp"
#include <cstdlib>
#define GLFW_INCLUDE_NONE
#include "Utility.hpp"
#include <GLFW/glfw3.h>
#include <algorithm>
#include <cstddef>
#include <glad/gl.h>
#include <memory>
#include <vector>

EntityBuffer::EntityBuffer(const IndexedShape &shape) {
  assert(gladLoadGL((GLADloadfunc)glfwGetProcAddress) && "GLAD not loaded.");

  generateBuffer(shape);
  bindShape(shape);
}

EntityBuffer &EntityBuffer::operator=(const EntityBuffer &buffer) {
  auto VAO{m_VAO};
  unsigned int EBO{m_EBO};
  m_EBO = buffer.m_EBO;

  VertexArray::operator=(buffer);
  free(VAO, EBO);

  return *this;
}

EntityBuffer &EntityBuffer::operator=(EntityBuffer &&buffer) noexcept {
  std::swap(m_EBO, buffer.m_EBO);
  VertexArray::operator=(buffer);

  return *this;
}

EntityBuffer *EntityBuffer::clone() const { return new EntityBuffer{*this}; }

void EntityBuffer::free(std::shared_ptr<unsigned int> VAO, unsigned int EBO) {
  if (VAO.use_count() == 1) {
    glDeleteBuffers(1, &EBO);
  }
}

void EntityBuffer::generateBuffer(const IndexedShape &shape) {
  VertexArray::generateBuffer(shape);

  glGenBuffers(1, &m_EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, toInt(shape.indicesSize()),
               shape.getIndices(), GL_STATIC_DRAW);
}
