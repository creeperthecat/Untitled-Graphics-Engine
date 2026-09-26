#pragma once

#include "TexParams.hpp"
#include <filesystem>
#include <string>
#include <string_view>
#include <vector>

/// Namespace for managing files.
/// Handles opening and reading for files.
/// Used for shaders and textures.
/// All files must be in the correct directory to be found.
namespace FileManager {
/// Enumerator of shader types.
enum class ShaderType {
  VERTEX,
  FRAGMENT,
  GEOMETRY,
};

/// Reads and compile a vertex shader file.
/// Throws runtime exception on read failure or compilation failure.
/// @return The compiled Vertex shader as an unsigned int.
/// @param file The name of the shader file to compile.
/// @param type The type of shader used by the shader file.
unsigned int compileShaderFile(std::string_view file, ShaderType type);

/// Loads a texture file.
/// Used in Texture to load new file.
/// Throws runtime exception if load fails.
/// @param file The name of the texture file to load.
/// @param ID Texture id which is used to bind texture.
/// @param params Optional specifier for texture parameters used when loading
/// the texture.
void loadTexture(std::string_view file, unsigned int ID,
                 const TexParams &params = {});

/// Generates a texture ID and loads texture file.
/// Throws runtime exception if load fails.
/// @param file The name of the texture file to load.
/// @param params Optional specifier for texture parameters used when loading
/// the texture.
unsigned int loadTexture(std::string_view file, const TexParams &params = {});

/// Generates a texture ID and loads texture file using a directory.
/// Throws runtime exception if load fails.
/// @param file The name of the texture file to load.
///@param directory The directory of the texture file.
/// @param params Optional specifier for texture parameters used when loading
/// the texture.
unsigned int loadTexture(std::string_view file, std::string_view directory,
                         const TexParams &params = {});

/// Loads a cubemap.
/// Throws runtime exception if load fails.
/// @param faces Vector of texture files used as the faces of the cubemap
/// @param ID Cubemap id which is used to bind cubemap.
/// @param params Optional specifier for texture parameters used when loading
/// the cubemap.
void loadCubemap(const std::vector<std::string_view> &faces, unsigned int ID,
                 const CubemapParams &params = {});

/// Generates a cubemap ID and loads a cubemap.
/// Throws runtime exception if load fails.
/// @param faces Vector of texture files used as the faces of the cubemap
/// @param params Optional specifier for texture parameters used when loading
/// the cubemap.
unsigned int loadCubemap(const std::vector<std::string_view> &faces,
                         const CubemapParams &params = {});

/// Reads a shader file into an std::string.
/// Throws runtime exception on failure.
///@param file Name of the shader file to read.
std::string readShaderFile(std::string_view file);
/// Returns the absolute path of a shader file.
///@param file Name of the shader file.
std::string getShaderFilePath(std::string_view file);
/// Returns the absolute path of a texture file.
///@param file Name of the texture file.
std::string getTextureFilePath(std::string_view file);
/// Returns the absolute path of a model file.
///@param file Name of the model file.
std::string getModelFilePath(std::string_view file);
/// Returns parent directory of a file.
///@param file Name of file.
std::string getFileDirectory(std::string_view file);
/// Returns the file path of the currently running executable.
std::filesystem::path getExecutablePath();

/// Compiles a shader, specified by shaderType.
/// Returns unsigned int representing shader.
/// @param shaderType integer representing openGL's shader types
/// @param shaderSource Source code of the shader.
unsigned int compile(int shaderType, std::string_view shaderSource);

/// Checks if compilation of a shader has been successful.
/// Throws runtime exception if an error has occurred.
/// @param ID Integer referencing a compiled shader
void checkCompile(unsigned int ID);
/// Checks if linking of shaders has been successful.
/// Throws runtime exception if an error has occurred.
/// @param ID Integer referencing a compiled shader
void checkLink(unsigned int ID);
/// Sets texture parameters for a texture.
/// @param ID The texture ID of the texture to set parameters for.
/// @param params The new parameters of the texture.
void loadTextureParameters(unsigned int ID, const TexParams &params = {});

/// Sets texture parameters for a cubemap.
/// @param ID The cubemap ID of the cubemap to set parameters for.
/// @param params The new parameters of the cubemap.
void loadCubemapParameters(unsigned int ID, const CubemapParams &params = {});

/// Gets the name of a shader type as a string.
///@param type Enum storing the type of a shader.
std::string getShaderTypeName(ShaderType type);
/// Converts a ShaderType into the openGL representation.
///@param type Enum storing the type of a shader.
unsigned int getGLShader(ShaderType type);
} // namespace FileManager
