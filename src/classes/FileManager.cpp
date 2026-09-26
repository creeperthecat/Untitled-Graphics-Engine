#include "Globals.hpp"
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION

#include "Config.hpp"
#include "FileManager.hpp"
#include <exception>
#include <fstream>
#include <glad/gl.h>
#include <sstream>
#include <stb_image.h>
#include <stdexcept>
#include <string_view>

std::string FileManager::getShaderTypeName(ShaderType type) {
  switch (type) {
  case ShaderType::VERTEX:
    return "VERTEX";
  case ShaderType::FRAGMENT:
    return "FRAGMENT";
  case ShaderType::GEOMETRY:
    return "GEOMETRY";
  default:
    return "UNKNOWN";
  }
}

unsigned int FileManager::getGLShader(ShaderType type) {
  switch (type) {
  case ShaderType::VERTEX:
    return GL_VERTEX_SHADER;
  case ShaderType::FRAGMENT:
    return GL_FRAGMENT_SHADER;
  case ShaderType::GEOMETRY:
    return GL_GEOMETRY_SHADER;
  default:
    return GL_VERTEX_SHADER;
  }
}

unsigned int FileManager::compileShaderFile(std::string_view file,
                                            ShaderType type) {
  std::string shaderSource{};
  try {
    shaderSource = readShaderFile(file);
  } catch (const std::exception &e) {
    std::stringstream message{};
    std::string name{getShaderTypeName(type)};

    message << "ERROR::FILE::" << name << "::READ_FAILED: " << e.what();
    throw std::runtime_error{message.str()};
  }

  unsigned int shader{compile(getGLShader(type), shaderSource)};

  try {
    checkCompile(shader);
  } catch (const std::exception &e) {
    std::stringstream message{};
    std::string name{getShaderTypeName(type)};
    message << "ERROR::SHADER::" << name
            << "::COMPILATION_FAILED: " << e.what();
    throw std::runtime_error{message.str()};
  }
  return shader;
}

void FileManager::loadTexture(std::string_view file, unsigned int ID,
                              const TexParams &params) {
  loadTextureParameters(ID, params);

  int width{};
  int height{};
  int nrChannels{};
  unsigned char *data{
      stbi_load(std::string{file}.c_str(), &width, &height, &nrChannels, 0)};

  if (!data) {
    stbi_image_free(data);
    std::stringstream error{};
    error << "For file: " << file << "\n";
    error << "Failed to load texture file.";
    throw std::runtime_error{error.str()};
  }

  GLenum format;
  switch (nrChannels) {
  case 1:
    format = GL_RED;
    break;
  case 3:
    format = GL_RGB;
    break;
  case 4:
    format = GL_RGBA;
    break;
  default:
    throw std::runtime_error{"Texture file format not known."};
  }

  glBindTexture(GL_TEXTURE_2D, ID);
  glTexImage2D(GL_TEXTURE_2D, 0, static_cast<GLint>(format), width, height, 0,
               format, GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);
  stbi_image_free(data);
}

void FileManager::loadCubemap(const std::vector<std::string_view> &faces,
                              unsigned int ID, const CubemapParams &params) {
  assert(faces.size() == 6 && "Invalid number of faces.");

  loadCubemapParameters(ID, params);

  for (int i{0}; i < faces.size(); ++i) {
    int width{};
    int height{};
    int nrChannels{};
    unsigned char *data{stbi_load(std::string{faces[i]}.c_str(), &width,
                                  &height, &nrChannels, 0)};

    if (!data) {
      stbi_image_free(data);
      std::stringstream error{};
      error << "For file: " << faces[i] << "\n";
      error << "Failed to load cubemap file.";
      throw std::runtime_error{error.str()};
    }

    GLenum format;
    switch (nrChannels) {
    case 1:
      format = GL_RED;
      break;
    case 3:
      format = GL_RGB;
      break;
    case 4:
      format = GL_RGBA;
      break;
    default:
      throw std::runtime_error{"Texture file format not known."};
    }

    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
                 static_cast<GLint>(format), width, height, 0, format,
                 GL_UNSIGNED_BYTE, data);
    stbi_image_free(data);
  }
}

unsigned int FileManager::loadTexture(std::string_view file,
                                      const TexParams &params) {
  unsigned int ID{};
  glGenTextures(1, &ID);
  loadTexture(file, ID, params);
  return ID;
}

unsigned int FileManager::loadTexture(std::string_view path,
                                      std::string_view directory,
                                      const TexParams &params) {
  return loadTexture(std::string{directory}.append("/").append(path));
}

unsigned int
FileManager::loadCubemap(const std::vector<std::string_view> &faces,
                         const CubemapParams &params) {
  unsigned int ID{};
  glGenTextures(1, &ID);
  loadCubemap(faces, ID, params);
  return ID;
}

std::string FileManager::readShaderFile(std::string_view file) {
  std::ifstream shaderFile{getShaderFilePath(file)};
  shaderFile.exceptions(std::ifstream::badbit | std::ifstream::failbit);

  std::stringstream fileStream{};
  fileStream << shaderFile.rdbuf();
  return fileStream.str();
}

std::string FileManager::getShaderFilePath(std::string_view file) {
  std::filesystem::path filePath{getExecutablePath()};
  filePath = filePath.parent_path().parent_path() / SharedFileDirectory /
             ShaderDirectory / file;
  return filePath.lexically_normal().string();
}

std::string FileManager::getTextureFilePath(std::string_view file) {
  std::filesystem::path filePath{getExecutablePath()};
  filePath = filePath.parent_path().parent_path() / SharedFileDirectory /
             TextureDirectory / file;
  return filePath.lexically_normal().string();
}

std::string FileManager::getModelFilePath(std::string_view file) {
  std::filesystem::path filePath{getExecutablePath()};
  filePath = filePath.parent_path().parent_path() / SharedFileDirectory / file;
  return filePath.lexically_normal().string();
}

std::filesystem::path FileManager::getExecutablePath() {
  std::filesystem::path filepath{std::filesystem::current_path()};
  filepath /= Globals::programPath;
  if (!std::filesystem::exists(filepath)) {
    throw std::runtime_error{"File does not exist."};
  }
  return filepath;
}

std::string FileManager::getFileDirectory(std::string_view file) {
  std::filesystem::path filePath{file};
  return filePath.parent_path().lexically_normal().string();
}

unsigned int FileManager::compile(int shaderType,
                                  std::string_view shaderSource) {
  const char *const contents{shaderSource.data()};
  unsigned int shader{glCreateShader(static_cast<GLenum>(shaderType))};
  glShaderSource(shader, 1, &contents, NULL);
  glCompileShader(shader);
  return shader;
}

void FileManager::checkCompile(unsigned int shader) {
  constexpr int bufferSize{1024};
  int success{};
  char infoLog[bufferSize]{};
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

  if (!success) {
    glGetShaderInfoLog(shader, bufferSize, NULL, infoLog);
    throw std::runtime_error{infoLog};
  }
}

void FileManager::checkLink(unsigned int ID) {
  constexpr int bufferSize{1024};
  int success{};
  char infoLog[bufferSize]{};
  glGetProgramiv(ID, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(ID, bufferSize, NULL, infoLog);
    std::stringstream message{};
    message << "ERROR::SHADER::PROGRAM::LINKING_FAILED: " << infoLog;
    throw std::runtime_error{message.str()};
  }
}

void FileManager::loadTextureParameters(unsigned int ID,
                                        const TexParams &params) {
  glBindTexture(GL_TEXTURE_2D, ID);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                  static_cast<GLint>(params.wrap_S));
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
                  static_cast<GLint>(params.wrap_T));
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  static_cast<GLint>(params.filterMin));
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                  static_cast<GLint>(params.filterMag));

  stbi_set_flip_vertically_on_load_thread(true);
}

void FileManager::loadCubemapParameters(unsigned int ID,
                                        const CubemapParams &params) {
  glBindTexture(GL_TEXTURE_CUBE_MAP, ID);

  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S,
                  static_cast<GLint>(params.wrap_S));

  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T,
                  static_cast<GLint>(params.wrap_T));
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R,
                  static_cast<GLint>(params.wrap_R));
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER,
                  static_cast<GLint>(params.filterMin));
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER,
                  static_cast<GLint>(params.filterMag));
  stbi_set_flip_vertically_on_load_thread(false);
}
