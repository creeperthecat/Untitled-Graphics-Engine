#include <cassert>
#include <iostream>
#include <utility>
#define GLFW_INCLUDE_NONE
#include "Shape.hpp"
#include "Utility.hpp"
#include "VertexArray.hpp"
#include <GLFW/glfw3.h>
#include <algorithm>
#include <cstddef>
#include <glad/gl.h>
#include <memory>

VertexArray::VertexArray(const Shape &shape) {
  assert(gladLoadGL((GLADloadfunc)glfwGetProcAddress) && "GLAD not loaded.");

  generateBuffer(shape);
  bindShape(shape);
}

VertexArray &VertexArray::operator=(const VertexArray &buffer) {
  auto VAO{m_VAO};
  unsigned int VBO{m_VBO};

  m_VAO = buffer.m_VAO;
  m_VBO = buffer.m_VBO;
  m_vertexCount = buffer.m_vertexCount;

  free(VAO, VBO);
  return *this;
}

VertexArray &VertexArray::operator=(VertexArray &&buffer) noexcept {
  m_VAO.swap(buffer.m_VAO);
  std::swap(m_VBO, buffer.m_VBO);

  m_vertexCount = buffer.m_vertexCount;

  return *this;
}

VertexArray *VertexArray::clone() const { return new VertexArray{*this}; }

void VertexArray::draw(unsigned int drawMode) const {
  glBindVertexArray(*m_VAO);
  glDrawArrays(drawMode, 0, m_vertexCount);
}

void VertexArray::free(std::shared_ptr<unsigned int> VAO, unsigned int VBO) {
  if (VAO.use_count() == 1) {
    glDeleteVertexArrays(1, VAO.get());
    glDeleteBuffers(1, &VBO);
  }
}

void VertexArray::generateBuffer(const Shape &shape) {
  glGenVertexArrays(1, m_VAO.get());
  glGenBuffers(1, &m_VBO);

  glBindVertexArray(*m_VAO);
  glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  glBufferData(GL_ARRAY_BUFFER, shape.size(), shape.getVertices(),
               GL_STATIC_DRAW);
}

void VertexArray::bindShape(const Shape &shape) {
  glBindVertexArray(*m_VAO);
  glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

  assignAttributes(shape.getStride());
  m_vertexCount = shape.vertexCount();
}

void VertexArray::assignAttributes(const Stride &stride) {
  std::size_t totalSize{0};
  for (auto attribute : stride.getAttributes()) {
    glEnableVertexAttribArray(static_cast<GLuint>(attribute));
    glVertexAttribPointer(static_cast<GLuint>(attribute),
                          stride.attributeLength(attribute), GL_FLOAT, GL_FALSE,
                          toInt(stride.size()), (void *)(totalSize));

    totalSize += stride.attributeSize(attribute);
  }
}
