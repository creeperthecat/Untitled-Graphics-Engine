#pragma once

#include "Shader.hpp"
#include <glad/gl.h>
#include <glm/gtc/type_ptr.hpp>
#include <memory>
#include <string_view>

/// Implementation of vertex and fragment shaders for use in Object instances.
class StandardShader : public Shader {
public:
  /// Initialises the shader.
  /// @param vertexShaderFile Name of the vertex shader file used for this
  /// shader.
  /// @param fragmentShaderFile Name of the fragment shader file used for this
  /// shader.
  /// @param freeShader Controls whether to hold memory used by shaders for
  /// later. Default behaviour is to always free memory after construction.
  StandardShader(std::string_view vertexShaderFile,
                 std::string_view fragmentShaderFile, bool freeShader = true);

  /// Copy constructor for Shader.
  /// Copies ID and shares ownership of compiled shaders
  StandardShader(const StandardShader &shader);
  /// Copy constructor for Shader, specifying a new fragment shader to use.
  /// The shader must still own memory held in vertex shader for this operation
  /// to be valid. If shader memory has been freed, construction will fail.
  ///@param shader %Shader to copy from
  ///@param fragmentShaderFile New fragment shader to use for this shader.
  /// @param freeShader Controls whether to hold fragment shader memory. Default
  /// behaviour is to always free memory after construction.
  StandardShader(const StandardShader &shader,
                 std::string_view fragmentShaderFile, bool freeShader = true);
  /// Move constructor for Shader.
  /// Passes ownership of any memory held by shader object.
  StandardShader(StandardShader &&shader) noexcept;

  /// Clones this instance of StandardShader.
  /// @return Pointer to a new StandardShader copied from this StandardShader.
  StandardShader *clone() const override;

  /// Copy assignment operator for Shader.
  /// Frees any memory currently held.
  /// Copies ID and shares ownership of compiled shaders
  const StandardShader &operator=(const StandardShader &shader);
  /// Move assignment operator for shader.
  /// Frees any memory held by shader, as it is no longer being used.
  /// Passes ownership of any memory held by shader object.
  StandardShader &operator=(StandardShader &&shader) noexcept;

  /// Destructor for StandardShader. Frees any memory held by shader.
  ~StandardShader() noexcept override;

private:
  // Internal state
  std::shared_ptr<unsigned int> m_vertexShader{};
  std::shared_ptr<unsigned int> m_fragmentShader{};

  void free();
  // Helper functions to compile and link shaders.
  void linkShaders(std::string_view vertexShaderFile,
                   std::string_view fragmentShaderFile);
  void linkShaders(std::string_view fragmentShaderFile);
};
