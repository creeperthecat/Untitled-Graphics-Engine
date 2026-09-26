#include "FrameBuffer.hpp"
#include "TexParams.hpp"
#include "Window.hpp"
#include <cassert>
#include <iostream>

FrameBuffer::FrameBuffer(int width, int height) {
  generateFramebuffer();
  bind();
  generateTextureBuffer(width, height);
  generateBufferObject(width, height);
  attachBuffer();
  bindDefault();
}

void FrameBuffer::bind() {
  assert(m_isValid && "Attempt to bind an invalid framebuffer.");
  glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
}

void FrameBuffer::bindTexture() {
  glBindTexture(GL_TEXTURE_2D, m_textureColorbuffer);
}

void FrameBuffer::bindDefault() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

void FrameBuffer::generateFramebuffer() {
  glGenFramebuffers(1, &m_framebuffer);
}

void FrameBuffer::generateTextureBuffer(int width, int height) {
  glGenTextures(1, &m_textureColorbuffer);
  glBindTexture(GL_TEXTURE_2D, m_textureColorbuffer);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
               GL_UNSIGNED_BYTE, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                         m_textureColorbuffer, 0);
}

void FrameBuffer::generateBufferObject(int width, int height) {
  glGenRenderbuffers(1, m_rbo.get());
  glBindRenderbuffer(GL_RENDERBUFFER, *m_rbo);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
}

void FrameBuffer::attachBuffer() {
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
                            GL_RENDERBUFFER, *m_rbo);
  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    m_isValid = false;
    std::cerr << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!\n";
  }
}

void FrameBuffer::free(std::shared_ptr<unsigned int> rbo,
                       unsigned int framebuffer) {
  if (rbo.use_count() == 1) {
    glDeleteRenderbuffers(1, rbo.get());
    glDeleteFramebuffers(1, &framebuffer);
  }
}

FrameBuffer &FrameBuffer::operator=(const FrameBuffer &framebuffer) {
  auto rbo{m_rbo};
  auto buffer{m_framebuffer};

  m_rbo = framebuffer.m_rbo;
  m_framebuffer = framebuffer.m_framebuffer;
  free(rbo, buffer);

  return *this;
}

FrameBuffer &FrameBuffer::operator=(FrameBuffer &&framebuffer) {
  m_rbo.swap(framebuffer.m_rbo);
  std::swap(m_framebuffer, framebuffer.m_framebuffer);
  m_isValid = framebuffer.m_isValid;

  return *this;
}
