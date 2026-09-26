#pragma once

#include "Window.hpp"
#include <memory>

/// The FrameBuffer class is used for the management and use of framebuffers
/// during rendering, which allows rendering the screen to a texture.
class FrameBuffer {
public:
  /// Constructs framebuffer and prepares for binding.
  /// @param width width in pixels of the framebuffer.
  /// @param height height in pixels of the framebuffer.
  FrameBuffer(int width, int height);
  /// Constructs framebuffer and prepares for binding.
  /// @param window The window currently in use by the Renderer, used to get
  /// window dimensions.
  FrameBuffer(const Window &window)
      : FrameBuffer{window.getWidth(), window.getHeight()} {}

  /// Copy constructor for FrameBuffer.
  FrameBuffer(const FrameBuffer &framebuffer) = default;
  /// Move constructor for FrameBuffer.
  FrameBuffer(FrameBuffer &&framebuffer) = default;

  /// Destructor for FrameBuffer. Frees data held by framebuffer, if not being
  /// held by another instance.
  ~FrameBuffer() { free(m_rbo, m_framebuffer); }
  /// Binds framebuffer to be the output of rendering calls.
  void bind();
  /// Binds texture for framebuffer to draw to.
  void bindTexture();
  /// Unbinds the framebuffer.
  static void bindDefault();

  /// Copy assignment operator for FrameBuffer.
  FrameBuffer &operator=(const FrameBuffer &framebuffer);
  /// Move assignment operator for FrameBuffer.
  FrameBuffer &operator=(FrameBuffer &&framebuffer);

private:
  // Internal buffers used for framebuffering
  std::shared_ptr<unsigned int> m_rbo{new unsigned int{0}};
  unsigned int m_framebuffer{};
  unsigned int m_textureColorbuffer{};

  // Is false when framebuffer fails
  bool m_isValid{true};

  // Helper functions for creating the framebuffer.
  void generateFramebuffer();
  void generateTextureBuffer(int width, int height);
  void generateBufferObject(int width, int height);
  void attachBuffer();

  // Frees data held
  static void free(std::shared_ptr<unsigned int> rbo, unsigned int framebuffer);
};
