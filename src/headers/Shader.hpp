#pragma once

#include <glad/gl.h>
#include <glm/gtc/type_ptr.hpp>
#include <memory>
#include <string_view>

/// Shader class for usage of vertex and fragment shaders.
/// Provides getters and setters for uniforms.
/// A valid shader has an ID representing the shader program.
class Shader {
public:
  /// Sets current shader as active shader.
  /// Automatically called by getters and setters.
  void use() const;

  /// Checks if a variable is defined in a shader
  /// @param name The name of the variable to check
  /// @return Returns false if the variable has not been defined in the shader.
  bool validFetch(std::string_view name) const;

  // Getters and setters for shaders

  /// Returns the value of a boolean.
  /// @param name The name of the boolean variable in the shader.
  bool getBool(std::string_view name) const;
  /// Returns the value of an integer defined in shader.
  /// @param name The name of the integer variable in the shader.
  int getInt(std::string_view name) const;
  /// Returns the value of a float defined in shader.
  /// @param name The name of the float variable in the shader.
  GLfloat getFloat(std::string_view name) const;
  /// Returns the shader ID.
  unsigned int getID() const { return *m_ID; }
  /// Get uniform block by name.
  /// @param block Name of the uniform block
  /// @return index of the named uniform block.
  unsigned int getUniformBlock(std::string_view block) const;

  /// Sets a boolean defined in shader.
  /// @param name The name of the boolean variable in the shader.
  /// @param value The new value of the variable.
  void setBool(std::string_view name, bool value) const;
  /// Sets an integer defined in shader.
  /// @param name The name of the integer variable in the shader.
  /// @param value The new value of the variable.
  void setInt(std::string_view name, int value) const;
  /// Sets a float defined in shader.
  /// @param name The name of the float variable in the shader.
  /// @param value The new value of the variable.
  void setFloat(std::string_view name, float value) const;
  /// Sets a vec2 defined in shader.
  /// @param name The name of the vec2 variable in the shader.
  /// @param value The new value of the variable.
  void setVec2(std::string_view name, const glm::vec2 &value) const;
  /// Sets a vec2 defined in shader.
  /// @param name The name of the vec2 variable in the shader.
  /// @param x The x coordinate of the new vector
  /// @param y The y coordinate of the new vector
  void setVec2(std::string_view name, float x, float y) const;

  /// Sets a vec3 defined in shader.
  /// @param name The name of the vec3 variable in the shader.
  /// @param value The new value of the variable.
  void setVec3(std::string_view name, const glm::vec3 &value) const;
  /// Sets a vec3 defined in shader.
  /// @param name The name of the vec3 variable in the shader.
  /// @param x The x coordinate of the new vector
  /// @param y The y coordinate of the new vector
  /// @param z The z coordinate of the new vector
  void setVec3(std::string_view name, float x, float y, float z) const;
  /// Sets a 4x4 matrix defined in shader. Used for transformations.
  /// @param name The name of the matrix in the shader.
  /// @param matrix The new value of the variable.
  void setMat4(std::string_view name, const glm::mat4 &matrix) const;

  /// Assign a uniform block's index using its number.
  /// @param block The number of the uniform block
  /// @param index The new index to assign to the block
  void setBlockIndex(unsigned int block, int index) const;
  /// Assign a uniform block's index using its name.
  /// @param block The name of the uniform block
  /// @param index The new index to assign to the block
  void setBlockIndex(std::string_view block, int index) const;

  /// Returns true if the shader is in a valid state.
  operator bool() const { return m_isValid; }

  /// Clones this instance of Shader.
  /// @return Pointer to a new Shader copied from this Shader.
  virtual Shader *clone() const = 0;

  /// Copy constructor for Shader.
  Shader(const Shader &shader);
  /// Move constructor for Shader.
  Shader(Shader &&shader) noexcept;
  /// Copy assignment operator for Shader.
  Shader &operator=(const Shader &shader);
  /// Move assignment operator for Shader.
  Shader &operator=(Shader &&shader) noexcept;

  /// Virtual destructor for Shader.
  virtual ~Shader() noexcept;

protected:
  /// Default constructor for derived classes to use.
  Shader() = default;
  /// ID of the shader. Must be initialised by derived classes.
  std::shared_ptr<unsigned int> m_ID{};
  /// Stores whether the shader is in a valid state for debugging.
  bool m_isValid{true};
  /// Frees resources held by the shader.
  void terminate();
};
