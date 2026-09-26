#pragma once

#include "InstancedArray.hpp"
#include <cassert>
#include <iostream>
#define GLFW_INCLUDE_NONE
#include "Shape.hpp"
#include "Utility.hpp"
#include "VertexArray.hpp"
#include <GLFW/glfw3.h>
#include <algorithm>
#include <glad/gl.h>
#include <memory>
#include <vector>

template <typename T>
InstancedArray<T>::InstancedArray(const Shape &shape,
                                  const std::vector<T> &instanceData,
                                  const Stride &dataStride) {
  assert(gladLoadGL((GLADloadfunc)glfwGetProcAddress) && "GLAD not loaded.");
  m_instanceSize = instanceData.size();

  generateBuffer(shape, instanceData);
  bindShape(shape, instanceData, dataStride);
}

template <typename T>
InstancedArray<T> &InstancedArray<T>::operator=(const InstancedArray &buffer) {
  auto VAO{buffer.m_VAO};
  auto instanceBuffer{buffer.m_instanceBuffer};

  m_instanceBuffer = buffer.m_instanceBuffer;
  m_instanceSize = buffer.m_instanceSize;
  VertexArray::operator=(buffer);

  free(VAO, instanceBuffer);

  return *this;
}

template <typename T>
InstancedArray<T> &
InstancedArray<T>::operator=(InstancedArray &&buffer) noexcept {
  std::swap(m_instanceBuffer, buffer.m_instanceBuffer);
  m_instanceSize = buffer.m_instanceSize;
  VertexArray::operator=(buffer);

  return *this;
}

template <typename T> InstancedArray<T> *InstancedArray<T>::clone() const {
  return new InstancedArray{*this};
}

template <typename T>
void InstancedArray<T>::draw(unsigned int drawMode) const {
  glBindVertexArray(*m_VAO);
  glDrawArraysInstanced(drawMode, 0, m_vertexCount, m_instanceSize);
}

template <typename T>
void InstancedArray<T>::free(std::shared_ptr<unsigned int> VAO,
                             unsigned int instanceBuffer) {
  if (VAO.use_count() == 1) {
    glDeleteBuffers(1, &instanceBuffer);
  }
}

template <typename T>
void InstancedArray<T>::generateBuffer(const Shape &shape,
                                       const std::vector<T> &instanceData) {
  VertexArray::generateBuffer(shape);

  glGenBuffers(1, &m_instanceBuffer);

  glBindBuffer(GL_ARRAY_BUFFER, m_instanceBuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(T) * instanceData.size(),
               instanceData.data(), GL_STATIC_DRAW);
}

template <typename T>
void InstancedArray<T>::bindShape(const Shape &shape,
                                  const std::vector<T> &instanceData,
                                  const Stride &dataStride) {
  VertexArray::bindShape(shape);

  assignData(shape.getStride().count(), dataStride);
}

template <typename T>
void InstancedArray<T>::assignData(int start_index, const Stride &stride) {
  glBindBuffer(GL_ARRAY_BUFFER, m_instanceBuffer);

  std::size_t totalSize{0};
  for (int index{start_index}; index < start_index + stride.count(); ++index) {
    glEnableVertexAttribArray(index);
    glVertexAttribPointer(index, stride.attributeLength(index - start_index),
                          GL_FLOAT, GL_FALSE, sizeof(T), (void *)totalSize);
    glVertexAttribDivisor(index, 1);

    totalSize += stride.attributeSize(index - start_index);
  }
}
