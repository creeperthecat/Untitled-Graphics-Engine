#include "Texture.hpp"
#include "FileManager.hpp"
#include "Utility.hpp"
#include <cassert>
#include <cstddef>
#include <exception>
#include <glad/gl.h>
#include <initializer_list>
#include <iostream>
#include <string_view>
#include <vector>

Texture::Texture(int length) {
  assert(length > 0 && "Length cannot be 0.");
  m_textures.resize(toUZ(length));
  glGenTextures(toInt(m_textures.size()), m_textures.data());
}

Texture::Texture(std::initializer_list<std::string_view> files,
                 const TexParams &params)
    : Texture(toInt(files.size())) {
  assert(files.size() > 0 && "Files not provided to texture.");
  try {
    for (std::size_t i{0}; i < files.size(); ++i) {
      FileManager::loadTexture(
          FileManager::getTextureFilePath(files.begin()[i]), m_textures[i],
          params);
    }
  } catch (const std::exception &e) {
    std::cout << e.what() << "\n";
    m_textures.resize(m_textures.size() - files.size());
  }
}

void Texture::add(const std::string_view file, const TexParams &params) {
  m_textures.emplace_back();
  glGenTextures(1, &m_textures.back());
  try {
    FileManager::loadTexture(FileManager::getTextureFilePath(file),
                             m_textures.back(), params);
  } catch (const std::exception &e) {
    std::cout << e.what() << "\n";
    m_textures.pop_back();
  }
}

void Texture::add(const std::vector<std::string_view> &files,
                  const TexParams &params) {
  assert(files.size() > 0 && "Files not provided to texture.");
  m_textures.resize(m_textures.size() + files.size());
  glGenTextures(toInt(files.size()),
                &m_textures.at(m_textures.size() - files.size()));
  for (std::size_t i{m_textures.size() - files.size()}; i < m_textures.size();
       ++i) {
    FileManager::loadTexture(FileManager::getTextureFilePath(files[i]),
                             m_textures[i], params);
  }
}

void Texture::use() const {
  for (int i{0}; i < m_textures.size(); ++i) {
    use(i);
  }
}

void Texture::use(int index) const {
  assert((0 <= index && index < m_textures.size()) && "Index out of bounds.");
  glActiveTexture(GL_TEXTURE0 + static_cast<GLenum>(index));
  glBindTexture(GL_TEXTURE_2D, m_textures[toUZ(index)]);
}
