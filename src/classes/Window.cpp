#include <glm/ext/vector_float3.hpp>
#define GLFW_INCLUDE_NONE

#include "Window.hpp"
#include <GLFW/glfw3.h>
#include <cassert>
#include <exception>
#include <iostream>
#include <stdexcept>
#include <string_view>

Window::Window(std::string_view name, int width, int height)
    : m_width{width}, m_height{height} {
  assert(width > 0 && height > 0 && "Invalid dimensions for window.");
  try {
    initialize(name, width, height);
    initializeGLAD();
  } catch (const std::exception &e) {
    m_window = nullptr;
    std::cout << e.what();
  }
}

Window::Window(const Window &window)
    : m_width{window.m_width}, m_height{window.m_height} {
  assert(window && "Invalid copy construct for window.");
}

Window::Window(Window &&window) noexcept
    : m_window{window}, m_width{window.m_width}, m_height{window.m_height} {
  assert(window && "Invalid move construct for window.");
  window.m_window = nullptr;
}

Window::~Window() { close(); }

int Window::getHeight() const { return m_height; }

int Window::getWidth() const { return m_width; }

float Window::getRatio() const { return float(m_width) / float(m_height); }

const Window &Window::operator=(const Window &window) {
  assert(window && "Invalid copy assignment for window.");
  close();
  int width{};
  int height{};
  glfwGetFramebufferSize(window, &width, &height);
  m_width = width;
  m_height = height;

  try {
    initialize(glfwGetWindowTitle(window), width, height);
    initializeGLAD();
  } catch (const std::exception &e) {
    m_window = nullptr;
    std::cout << e.what();
  }
  return *this;
}

const Window &Window::operator=(Window &&window) noexcept {
  assert(window && "Invalid move assignment for window.");
  close();
  m_window = window;
  window.m_window = nullptr;
  return *this;
}

void Window::close() const {
  if (m_window)
    glfwSetWindowShouldClose(m_window, true);
}

void Window::terminate() {
  close();
  m_window = nullptr;
  glfwTerminate();
}

void Window::clear(const glm::vec3 &color) const {
  clear(color.r, color.g, color.b);
}

void Window::clear(float red, float green, float blue) const {
  glClearColor(red, green, blue, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
}

void Window::initialize(std::string_view name, int width, int height) {
  // window initialize
  if (!glfwInit())
    throw std::runtime_error{"Failed to initialise GLFW."};

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

  // window create
  m_window = glfwCreateWindow(width, height, name.data(), NULL, NULL);
  if (m_window == NULL) {
    glfwTerminate();
    std::cout << "Failed to create GLFW window.\n";
  }

  // Current thread
  glfwMakeContextCurrent(m_window);
  // Callback for window resize
  glfwSetFramebufferSizeCallback(m_window, framebufferSizeCallback);
}

void Window::initializeGLAD() {
  // load function pointers
  if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress)) {
    m_window = nullptr;
    glfwTerminate();
    std::cout << "Failed to initialise GLAD.\n";
  }
}

void Window::framebufferSizeCallback([[maybe_unused]] GLFWwindow *window,
                                     int height, int width) {
  glViewport(0, 0, width, height);
}
