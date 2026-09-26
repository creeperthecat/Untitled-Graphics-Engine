#pragma once

#include "InstancedModel.hpp"
#include <iostream>

template <typename T>
InstancedModel<T>::InstancedModel(std::string_view path, Shader *shader,
                                  std::vector<T> &instanceData,
                                  const Stride &dataStride)
    : Model{path, shader}, m_instanceBuffer{instanceData, dataStride} {
  for (const auto &mesh : m_meshes) {
    m_instanceBuffer.assignData(mesh, 3, dataStride);
  }
}

template <typename T>
void InstancedModel<T>::draw(bool skipTextures, unsigned int drawMode) const {
  for (const auto &mesh : m_meshes) {
    m_instanceBuffer.draw(mesh, m_shader.get(), skipTextures, drawMode);
  }
}
