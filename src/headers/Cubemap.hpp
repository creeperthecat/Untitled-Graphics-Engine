#include "Buffer.hpp"
#include "Object.hpp"
#include "Shader.hpp"
#include "TexParams.hpp"
#include "Utility.hpp"
#include <string_view>
#include <vector>

/// Object type used for rendering cubemaps.
/// Commonly used for skyboxes.
class Cubemap : public Object {
public:
  /// Constructor for Cubemap instances.
  /// @param buffer Pointer to a constructed Buffer, needed for drawing
  /// vertices.
  /// @param shader Pointer to a constructed Shader for this cubemap to use.
  /// @param faces A vector of texture files used to construct the cubemap.
  /// @param params Optional specifier for texture parameters used when loading
  /// the cubemap.
  Cubemap(Buffer *buffer, Shader *shader,
          const std::vector<std::string_view> &faces,
          const CubemapParams &params = {});

  /// Uses shaders and binds textures before
  /// drawing the cubemap to the screen.
  void draw(unsigned int drawMode = GL_TRIANGLES) const override;
  /// Binds cubemap textures.
  void bind() const;
  /// Returns shader used by cubemap.
  const Shader *getShader() const;

private:
};
