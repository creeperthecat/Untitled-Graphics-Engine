#include "Cubemap.hpp"
#include "FileManager.hpp"
#include "TexParams.hpp"
#include <algorithm>
#include <iostream>
#include <memory>
#include <string_view>
#include <vector>

Cubemap::Cubemap(Buffer *buffer, Shader *shader,
                 const std::vector<std::string_view> &faces,
                 const CubemapParams &params) {
  m_buffer = std::shared_ptr<Buffer>{buffer};
  m_shader = std::shared_ptr<Shader>{shader};

  std::vector<std::string> paths{};
  for (const auto &f : faces) {
    paths.emplace_back(std::string{FileManager::getTextureFilePath(f)});
  }
  std::vector<std::string_view> view{};
  std::string s{};
  for (const auto &p : paths) {
    view.push_back(std::string_view{p});
  }

  m_ID = FileManager::loadCubemap(view, params);
}

void Cubemap::draw(unsigned int drawMode) const {
  m_shader->use();
  bind();
  m_buffer->draw(drawMode);
}

void Cubemap::bind() const {
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_CUBE_MAP, m_ID);
}

const Shader *Cubemap::getShader() const { return m_shader.get(); }
