#pragma once
#include <glad/gl.h>

/// Struct to define a list of parameters for textures.
/// Used in Texture and FileManager when loading an image.
struct TexParams {
  unsigned int wrap_S{GL_REPEAT};
  unsigned int wrap_T{GL_REPEAT};
  unsigned int filterMin{GL_LINEAR};
  unsigned int filterMag{GL_LINEAR};
};

/// Struct to define a list of parameters for cubemaps
/// Used in Cubemap and FileManager when loading images.
struct CubemapParams {
  unsigned int wrap_S{GL_CLAMP_TO_EDGE};
  unsigned int wrap_T{GL_CLAMP_TO_EDGE};
  unsigned int wrap_R{GL_CLAMP_TO_EDGE};
  unsigned int filterMin{GL_LINEAR};
  unsigned int filterMag{GL_LINEAR};
};
