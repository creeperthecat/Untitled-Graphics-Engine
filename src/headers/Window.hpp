#pragma once
#include <glm/ext/vector_float3.hpp>
#define GLFW_INCLUDE_NONE

#include <GLFW/glfw3.h>
#include <glad/gl.h>
#include <string_view>

/// The Window class is used to create an opengl context and a view of the
/// scene. Used in Renderer during render loop. Must be initialised before
/// initialisation of Object.
class Window {
public:
  /// Constructor for Window.
  /// Initialises context for opengl functions and creates a window.
  /// If an error occurs, sets to invalid state and deletes any held memory.
  ///@param name Name of the new window.
  ///@param width Width of the new window.
  ///@param height Height of the new window.
  Window(std::string_view name = {"Window"}, int width = 800, int height = 600);

  /// Copy constructor for Window.
  /// Creates a new window using the same parameters.
  Window(const Window &window);
  /// Move constructor for Window.
  /// Transfers ownership of window to this class instance.
  Window(Window &&window) noexcept;
  /// Destructor for Window. Calls close().
  ~Window();

  /// Copy assignment operator for Window.
  /// Calls close(), then creates new window using same parameters.
  const Window &operator=(const Window &window);
  /// Move assignment operator for Window.
  /// Calls close(), then transfers ownership.
  const Window &operator=(Window &&window) noexcept;

  /// Implicit typecasting to GLFWwindow for use with opengl functions.
  operator GLFWwindow *() const { return get(); }
  /// Returns true if in a valid state
  operator bool() const { return m_window; }

  /// Returns internal GLFWwindow. Consider using implicit typecasting instead.
  GLFWwindow *get() const { return m_window; }
  /// Fills the window screen using the specified colour.
  /// Uses a light grey by default.
  ///@param red floating point red value between 0 and 1.
  ///@param green floating point green value between 0 and 1.
  ///@param blue floating point blue value between 0 and 1.
  void clear(float red = 0.1f, float green = 0.1f, float blue = 0.1f) const;
  /// Fills the window screen using the specified colour.
  /// @param color Color to fill the window with.
  void clear(const glm::vec3 &color) const;
  /// Closes window, if one exists.
  void close() const;
  /// Gets aspect ratio of window. Used in Renderer when constructing
  /// perspective matrix.
  float getRatio() const;
  /// Returns height of window in pixels.
  int getHeight() const;
  /// Returns width of window in pixels.
  int getWidth() const;
  /// Terminates glfw functions. Cannot
  /// be used in callback functions. Called automatically by Renderer after
  /// render loop.
  void terminate();

private:
  // Internal data
  GLFWwindow *m_window{};
  int m_width{};
  int m_height{};
  // Helper functions for initialisation
  void initialize(std::string_view name, int width, int height);
  void initializeGLAD();
  // Callback functions for window resize.
  static void framebufferSizeCallback(GLFWwindow *window, int height,
                                      int width);
};
