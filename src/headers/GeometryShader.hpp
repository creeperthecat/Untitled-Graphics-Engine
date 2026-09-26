#pragma once

#include "Shader.hpp"

/// Implementation of geometry shaders for use in Object instances.
class GeometryShader : public Shader {
public:
  /// Initialises the geometry shader.
  /// @param vertexShaderFile Name of the vertex shader file used for this
  /// shader.
  /// @param fragmentShaderFile Name of the fragment shader file used for this
  /// shader.
  /// @param geometryShaderFile Name of the geometry shader file used for this
  /// shader.
  /// @param freeShader Controls whether to hold memory used by shaders for
  /// later. Default behaviour is to always free memory after construction.
  GeometryShader(std::string_view vertexShaderFile,
                 std::string_view fragmentShaderFile,
                 std::string_view geometryShaderFile, bool freeShader = true);

  /// Copy constructor for GeometryShader.
  /// Copies ID and shares ownership of compiled shaders
  GeometryShader(const GeometryShader &shader);
  /// Move constructor for GeometryShader.
  GeometryShader(GeometryShader &&shader) noexcept;

  /// Copy assignment operator for GeometryShader.
  const GeometryShader &operator=(const GeometryShader &shader);
  /// Move assignment operator for GeometryShader.
  GeometryShader &operator=(GeometryShader &&shader) noexcept;

  /// Destructor for GeometryShader. Frees any resources held.
  ~GeometryShader() noexcept override;

  /// Clones this instance of Shader.
  /// @return Pointer to a new Shader copied from this Shader.
  GeometryShader *clone() const override;

private:
  std::shared_ptr<unsigned int> m_vertexShader{};
  std::shared_ptr<unsigned int> m_fragmentShader{};
  std::shared_ptr<unsigned int> m_geometryShader{};

  void free();
  void linkShaders(std::string_view vertexShader,
                   std::string_view fragmentShader,
                   std::string_view geometryShader);
};
