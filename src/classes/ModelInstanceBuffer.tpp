#pragma once

#include "ModelInstanceBuffer.hpp"
#include "TexParams.hpp"
#include <iostream>

template <typename T>
ModelInstanceBuffer<T>::ModelInstanceBuffer(const std::vector<T> &instanceData,
                                            const Stride &dataStride) {
  m_instanceSize = instanceData.size();
  generateBuffer(instanceData);
}

template <typename T>
ModelInstanceBuffer<T> &
ModelInstanceBuffer<T>::operator=(const ModelInstanceBuffer &buffer) {
  auto instanceBuffer{buffer.m_instanceBuffer};
  m_instanceBuffer = buffer.m_instanceBuffer;
  m_instanceSize = buffer.m_instanceSize;
  free(instanceBuffer);

  return *this;
}

template <typename T>
ModelInstanceBuffer<T> &
ModelInstanceBuffer<T>::operator=(ModelInstanceBuffer &&buffer) noexcept {
  std::swap(m_instanceBuffer, buffer.m_instanceBuffer);
  m_instanceSize = buffer.m_instanceSize;

  return *this;
}

template <typename T>
void ModelInstanceBuffer<T>::draw(const Mesh &mesh, const Shader *shader,
                                  bool skipTextures,
                                  unsigned int drawMode) const {
  shader->use();
  mesh.bindTextures(shader);
  glBindVertexArray(mesh.getVAO());
  glDrawElementsInstanced(GL_TRIANGLES,
                          static_cast<unsigned int>(mesh.indicesSize()),
                          GL_UNSIGNED_INT, 0, m_instanceSize);
}

template <typename T>
void ModelInstanceBuffer<T>::generateBuffer(
    const std::vector<T> &instanceData) {
  glGenBuffers(1, m_instanceBuffer.get());
  glBindBuffer(GL_ARRAY_BUFFER, *m_instanceBuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(T) * instanceData.size(),
               instanceData.data(), GL_STATIC_DRAW);
}

template <typename T>
void ModelInstanceBuffer<T>::assignData(const Mesh &mesh, int start_index,
                                        const Stride &stride) {
  glBindVertexArray(mesh.getVAO());

  std::size_t totalSize{0};
  for (int index{start_index}; index < start_index + stride.count(); ++index) {
    glEnableVertexAttribArray(index);
    glVertexAttribPointer(index, stride.attributeLength(index - start_index),
                          GL_FLOAT, GL_FALSE, sizeof(T), (void *)totalSize);
    glVertexAttribDivisor(index, 1);
    totalSize += stride.attributeSize(index - start_index);
  }
}

template <typename T>
void ModelInstanceBuffer<T>::free(
    std::shared_ptr<unsigned int> instanceBuffer) {
  if (instanceBuffer.use_count() == 1) {
    glDeleteBuffers(1, instanceBuffer.get());
  }
}
