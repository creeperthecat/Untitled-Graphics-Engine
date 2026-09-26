#include "StandardShader.hpp"
#include "FileManager.hpp"
#include "Shader.hpp"
#include <cassert>
#include <exception>
#include <iostream>
#include <memory>
#include <string_view>

StandardShader::StandardShader(std::string_view vertexShaderFile,
                               std::string_view fragmentShaderFile,
                               bool freeShader)
    : Shader() {
  linkShaders(vertexShaderFile, fragmentShaderFile);
  if (freeShader)
    free();
}

StandardShader::StandardShader(const StandardShader &shader,
                               std::string_view fragmentShaderFile,
                               bool freeShader)
    : m_vertexShader{shader.m_vertexShader} {
  assert(shader && m_vertexShader && "Invalid copy of shader.");
  linkShaders(fragmentShaderFile);
  m_vertexShader.reset();
  if (freeShader)
    free();
}

StandardShader::StandardShader(const StandardShader &shader)
    : Shader{shader}, m_vertexShader{shader.m_vertexShader},
      m_fragmentShader{shader.m_fragmentShader} {}

StandardShader::StandardShader(StandardShader &&shader) noexcept
    : Shader{shader}, m_vertexShader{shader.m_vertexShader},
      m_fragmentShader{shader.m_fragmentShader} {
  shader.m_vertexShader.reset();
  shader.m_fragmentShader.reset();
}

const StandardShader &StandardShader::operator=(const StandardShader &shader) {
  m_vertexShader = shader.m_vertexShader;
  m_fragmentShader = shader.m_fragmentShader;
  Shader::operator=(shader);

  return *this;
}

StandardShader &StandardShader::operator=(StandardShader &&shader) noexcept {
  m_vertexShader = shader.m_vertexShader;
  m_fragmentShader = shader.m_fragmentShader;

  Shader::operator=(shader);

  shader.free();

  return *this;
}

StandardShader::~StandardShader() noexcept { free(); }

void StandardShader::free() {
  if (m_vertexShader && m_vertexShader.use_count() == 1)
    glDeleteShader(*m_vertexShader);
  if (m_fragmentShader && m_fragmentShader.use_count() == 1)
    glDeleteShader(*m_fragmentShader);
  m_vertexShader.reset();
  m_fragmentShader.reset();
}

StandardShader *StandardShader::clone() const {
  return new StandardShader{*this};
}

void StandardShader::linkShaders(std::string_view vertexShaderFile,
                                 std::string_view fragmentShaderFile) {
  try {
    m_vertexShader =
        std::make_shared<unsigned int>(FileManager::compileShaderFile(
            vertexShaderFile, FileManager::ShaderType::VERTEX));
    m_fragmentShader =
        std::make_shared<unsigned int>(FileManager::compileShaderFile(
            fragmentShaderFile, FileManager::ShaderType::FRAGMENT));
    m_ID = std::make_shared<unsigned int>(glCreateProgram());

    glAttachShader(*m_ID, *m_vertexShader);
    glAttachShader(*m_ID, *m_fragmentShader);
    glLinkProgram(*m_ID);
    FileManager::checkLink(*m_ID);

  } catch (const std::exception &e) {
    std::cout << e.what() << "\n";
    m_isValid = false;
  }
}

void StandardShader::linkShaders(std::string_view fragmentShaderFile) {
  try {
    m_fragmentShader =
        std::make_shared<unsigned int>(FileManager::compileShaderFile(
            fragmentShaderFile, FileManager::ShaderType::FRAGMENT));
    m_ID = std::make_shared<unsigned int>(glCreateProgram());

    glAttachShader(*m_ID, *m_vertexShader);
    glAttachShader(*m_ID, *m_fragmentShader);
    glLinkProgram(*m_ID);
    FileManager::checkLink(*m_ID);
  } catch (const std::exception &e) {
    std::cout << e.what() << "\n";
    m_isValid = false;
  }
}
