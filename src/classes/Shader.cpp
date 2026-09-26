
#include "Shader.hpp"
#include <cassert>
#include <memory>
#include <string>
#include <string_view>

Shader::Shader(const Shader &shader) : m_ID{shader.m_ID} {
  assert(shader && "Invalid copy construct of shader.");
}

Shader::Shader(Shader &&shader) noexcept : m_ID{shader.m_ID} {
  assert(shader && "Invalid move construct of shader.");
  shader.m_isValid = false;
  shader.m_ID.reset();
}

Shader &Shader::operator=(const Shader &shader) {
  assert(shader && "Invalid copy assignment of shader.");
  terminate();
  m_ID = shader.m_ID;

  return *this;
}

Shader &Shader::operator=(Shader &&shader) noexcept {
  assert(shader && "Invalid move assignment of shader.");
  terminate();

  std::swap(m_ID, shader.m_ID);
  shader.m_isValid = false;
  return *this;
}

Shader::~Shader() noexcept { terminate(); }

void Shader::use() const {
  assert(m_isValid && "Trying to use an invalid shader.");
  glUseProgram(*m_ID);
}

void Shader::terminate() {
  m_isValid = false;
  if (m_ID && m_ID.use_count() == 1)
    glDeleteProgram(*m_ID);
  m_ID.reset();
}

bool Shader::validFetch(std::string_view name) const {
  return glGetUniformLocation(*m_ID, name.data()) != -1;
}

void Shader::setBool(std::string_view name, bool value) const {
  use();
  assert(validFetch(name));
  glUniform1i(glGetUniformLocation(*m_ID, name.data()), (int)value);
}

void Shader::setInt(std::string_view name, int value) const {
  use();
  assert(validFetch(name) && "Invalid uniform.");
  glUniform1i(glGetUniformLocation(*m_ID, name.data()), value);
}

void Shader::setFloat(std::string_view name, float value) const {
  use();
  assert(validFetch(name));
  glUniform1f(glGetUniformLocation(*m_ID, name.data()), value);
}

void Shader::setVec3(std::string_view name, const glm::vec3 &value) const {
  use();
  assert(validFetch(name));
  glUniform3fv(glGetUniformLocation(*m_ID, name.data()), 1, &value[0]);
}

void Shader::setVec3(std::string_view name, float x, float y, float z) const {
  use();
  assert(validFetch(name));
  glUniform3f(glGetUniformLocation(*m_ID, name.data()), x, y, z);
}

void Shader::setVec2(std::string_view name, const glm::vec2 &value) const {
  use();
  assert(validFetch(name));
  glUniform2fv(glGetUniformLocation(*m_ID, name.data()), 1, &value[0]);
}

void Shader::setVec2(std::string_view name, float x, float y) const {
  use();
  assert(validFetch(name));
  glUniform2f(glGetUniformLocation(*m_ID, name.data()), x, y);
}

void Shader::setMat4(std::string_view name, const glm::mat4 &matrix) const {
  use();
  assert(validFetch(name));
  glUniformMatrix4fv(glGetUniformLocation(*m_ID, name.data()), 1, GL_FALSE,
                     glm::value_ptr(matrix));
}

void Shader::setBlockIndex(unsigned int block, int index) const {
  glUniformBlockBinding(*m_ID, block, index);
}

void Shader::setBlockIndex(std::string_view block, int index) const {
  setBlockIndex(getUniformBlock(block), index);
}

bool Shader::getBool(std::string_view name) const {
  use();
  assert(validFetch(name));
  bool *value{};
  glGetUniformiv(*m_ID, glGetUniformLocation(*m_ID, name.data()),
                 (GLint *)value);
  return *value;
}

int Shader::getInt(std::string_view name) const {
  use();
  assert(validFetch(name));
  int *value{};
  glGetUniformiv(*m_ID, glGetUniformLocation(*m_ID, name.data()),
                 (GLint *)value);
  return *value;
}

GLfloat Shader::getFloat(std::string_view name) const {
  use();
  assert(validFetch(name));
  GLfloat *value{};
  glGetUniformfv(*m_ID, glGetUniformLocation(*m_ID, name.data()), value);
  return *value;
}

unsigned int Shader::getUniformBlock(std::string_view block) const {
  return glGetUniformBlockIndex(*m_ID, std::string{block}.c_str());
}
