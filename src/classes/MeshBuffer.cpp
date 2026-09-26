#include "MeshBuffer.hpp"
#define GLFW_INCLUDE_NONE
#include "Buffer.hpp"
#include "Utility.hpp"
#include <GLFW/glfw3.h>
#include <cstddef>
#include <glad/gl.h>
#include <memory>
#include <vector>

MeshBuffer::MeshBuffer(const std::vector<ModelData::Vertex> &vertices,
                       const std::vector<unsigned int> &indices) {
  assert(gladLoadGL((GLADloadfunc)glfwGetProcAddress) && "GLAD not loaded.");
  assert(!vertices.empty() && "No vertices provided to buffer.");
  m_vertexCount = indices.size();

  generateBuffer();
  bindMesh(vertices, indices);
}

const MeshBuffer &MeshBuffer::operator=(const MeshBuffer &buffer) {
  auto VAO{m_VAO};
  auto VBO{m_VBO};
  auto EBO{m_EBO};

  m_VAO = buffer.m_VAO;
  m_VBO = buffer.m_VBO;
  m_EBO = buffer.m_EBO;
  m_vertexCount = buffer.m_vertexCount;

  free(VAO, VBO, EBO);

  return *this;
}

const MeshBuffer &MeshBuffer::operator=(MeshBuffer &&buffer) noexcept {
  m_VAO.swap(buffer.m_VAO);
  std::swap(m_VBO, buffer.m_VBO);
  std::swap(m_EBO, buffer.m_EBO);

  m_vertexCount = buffer.m_vertexCount;

  return *this;
}

MeshBuffer *MeshBuffer::clone() const { return new MeshBuffer{*this}; }

void MeshBuffer::free(std::shared_ptr<unsigned int> VAO, unsigned int VBO,
                      unsigned int EBO) {
  if (VAO.use_count() == 1) {
    glDeleteVertexArrays(1, VAO.get());
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
  }
}

void MeshBuffer::generateBuffer() {
  glGenVertexArrays(1, m_VAO.get());
  glGenBuffers(1, &m_VBO);
  glGenBuffers(1, &m_EBO);
}

void MeshBuffer::bindMesh(const std::vector<ModelData::Vertex> &vertices,
                          const std::vector<unsigned int> &indices) {
  glBindVertexArray(*m_VAO);
  glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(ModelData::Vertex),
               &vertices[0], GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
               &indices[0], GL_STATIC_DRAW);

  assignAttributes();
}

void MeshBuffer::assignAttributes() {
  // vertex positions
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ModelData::Vertex),
                        (void *)0);
  // vertex normals
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(ModelData::Vertex),
                        (void *)offsetof(ModelData::Vertex, normal));
  // vertex texture coordinates
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(ModelData::Vertex),
                        (void *)offsetof(ModelData::Vertex, texCoords));
}

void MeshBuffer::draw(unsigned int drawMode) const {
  glBindVertexArray(*m_VAO);
  glDrawElements(drawMode, m_vertexCount, GL_UNSIGNED_INT, 0);
}
