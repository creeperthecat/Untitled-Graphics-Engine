#include "GeometryShader.hpp"
#include "FileManager.hpp"
#include "Shader.hpp"
#include <iostream>

GeometryShader::GeometryShader(std::string_view vertexShaderFile,
                               std::string_view fragmentShaderFile,
                               std::string_view geometryShaderFile,
                               bool freeShader) {
  linkShaders(vertexShaderFile, fragmentShaderFile, geometryShaderFile);
  if (freeShader)
    free();
}

GeometryShader::GeometryShader(const GeometryShader &shader)
    : Shader{shader}, m_vertexShader{shader.m_vertexShader},
      m_fragmentShader{shader.m_fragmentShader},
      m_geometryShader{shader.m_geometryShader} {}

GeometryShader::GeometryShader(GeometryShader &&shader) noexcept
    : Shader{shader}, m_vertexShader{shader.m_vertexShader},
      m_fragmentShader{shader.m_fragmentShader},
      m_geometryShader{shader.m_geometryShader} {
  shader.m_vertexShader.reset();
  shader.m_fragmentShader.reset();
  shader.m_geometryShader.reset();
}

const GeometryShader &GeometryShader::operator=(const GeometryShader &shader) {
  m_vertexShader = shader.m_vertexShader;
  m_fragmentShader = shader.m_fragmentShader;
  m_geometryShader = shader.m_geometryShader;
  Shader::operator=(shader);

  return *this;
}

GeometryShader &GeometryShader::operator=(GeometryShader &&shader) noexcept {
  m_vertexShader = shader.m_vertexShader;
  m_fragmentShader = shader.m_fragmentShader;
  m_geometryShader = shader.m_geometryShader;

  Shader::operator=(shader);

  shader.free();
  return *this;
}

GeometryShader::~GeometryShader() noexcept { free(); }

void GeometryShader::free() {
  if (m_vertexShader && m_vertexShader.use_count() == 1)
    glDeleteShader(*m_vertexShader);
  if (m_fragmentShader && m_fragmentShader.use_count() == 1)
    glDeleteShader(*m_fragmentShader);
  if (m_geometryShader && m_geometryShader.use_count() == 1)
    glDeleteShader(*m_geometryShader);

  m_vertexShader.reset();
  m_fragmentShader.reset();
  m_geometryShader.reset();
}

GeometryShader *GeometryShader::clone() const {
  return new GeometryShader{*this};
}

void GeometryShader::linkShaders(std::string_view vertexShaderFile,
                                 std::string_view fragmentShaderFile,
                                 std::string_view geometryShaderFile) {
  try {
    m_vertexShader =
        std::make_shared<unsigned int>(FileManager::compileShaderFile(
            vertexShaderFile, FileManager::ShaderType::VERTEX));
    m_fragmentShader =
        std::make_shared<unsigned int>(FileManager::compileShaderFile(
            fragmentShaderFile, FileManager::ShaderType::FRAGMENT));
    m_geometryShader =
        std::make_shared<unsigned int>(FileManager::compileShaderFile(
            geometryShaderFile, FileManager::ShaderType::GEOMETRY));

    m_ID = std::make_shared<unsigned int>(glCreateProgram());

    glAttachShader(*m_ID, *m_vertexShader);
    glAttachShader(*m_ID, *m_fragmentShader);
    glAttachShader(*m_ID, *m_geometryShader);

    glLinkProgram(*m_ID);
    FileManager::checkLink(*m_ID);

  } catch (const std::exception &e) {
    std::cout << e.what() << "\n";
    m_isValid = false;
  }
}
