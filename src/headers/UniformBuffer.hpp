#pragma once

#include "Window.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <cstddef>
#include <memory>

/// The UniformBuffer class is used to assist shaders in managing uniform
/// buffers. It is used to declare a set of global uniform variables, useful
/// for when variables are often repeated.
class UniformBuffer {
public:
  /// Creates a new uniform buffer.
  /// @param size The size of the uniform buffer.
  UniformBuffer(std::size_t size);

  /// Bind uniform buffer for shaders to use.
  void bind() const;

  /// Stores data in the uniform buffer.
  /// @param offset Offset of the new data to be stored.
  /// @param value New data to be stored.
  template <typename T> void storeData(int offset, const T &value) const;

  /// Adds data to the uniform buffer.
  /// @param value New data to be stored.
  template <typename T> void addData(const T &value);

  /// Returns the id of the uniform buffer.
  unsigned int getID() const { return *m_ID; }
  /// Returns how much of the buffer has been used i.e. where to place new data.
  int getOffset() const { return m_offset; }

private:
  std::unique_ptr<unsigned int> m_ID{nullptr};
  std::size_t m_size{};
  int m_offset{0};

  void generateBuffer();
};

template <typename T>
void UniformBuffer::storeData(int offset, const T &value) const {
  assert(offset + sizeof(T) <= m_size && "Memory overflow.");
  glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(T), glm::value_ptr(value));
}

template <typename T> void UniformBuffer::addData(const T &value) {
  storeData(m_offset, value);
  m_offset += sizeof(value);
}
