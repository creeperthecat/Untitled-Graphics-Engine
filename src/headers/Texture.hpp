#pragma once

#include "TexParams.hpp"
#include <glad/gl.h>
#include <initializer_list>
#include <string_view>
#include <vector>

/// The Texture class is used for loading and managing textures to be used with
/// shaders. The same instance can store multiple texture files, which can then
/// all be sent to the shader.
class Texture {
public:
  /// Default constructor for Texture.
  /// Initialises an empty list of textures and sets parameters to default.
  Texture() = default;
  /// Constructor for Texture using a list of files.
  /// Automatically finds, reads and loads textures into memory.
  /// All files in the list will use the same parameters, if you wish to specify
  /// different parameters, consider using add() and redefining them.
  /// @param files List of files to load.
  /// @param params Optional specifier for texture parameters used when loading
  Texture(std::initializer_list<std::string_view> files,
          const TexParams &params = {});

  /// Constructor for Texture using a texture file.
  /// @param file Name of file to load.
  /// @param params Optional specifier for texture parameters used when loading
  /// file.
  Texture(std::string_view file, const TexParams &params = {})
      : Texture{{file}, params} {}

  /// Constructor for Texture which sets an initial number of textures to store.
  /// @param length Number of textures to reserve space for.
  explicit Texture(int length);

  /// Loads an additional texture file into memory.
  /// @param file Name of file to add.
  /// @param params Optional specifier for texture parameters used when loading
  /// file.
  void add(std::string_view file, const TexParams &params = {});
  /// Loads a list of texture files into memory.
  /// @param files List of files to add.
  /// @param params Optional specifier for texture parameters used when loading
  /// files.
  void add(const std::vector<std::string_view> &files,
           const TexParams &params = {});
  /// Binds all textures currently held. Used in Object for drawing to the
  /// screen.
  void use() const;

private:
  // Internal members
  std::vector<unsigned int> m_textures{};
  // Binds a texture referenced by index.
  void use(int index) const;
};
