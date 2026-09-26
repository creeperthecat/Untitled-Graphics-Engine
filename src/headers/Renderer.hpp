#pragma once
#define GLFW_INCLUDE_NONE
#include "Camera.hpp"
#include "GLFW/glfw3.h"
#include "Object.hpp"
#include "Window.hpp"
#include <functional>
#include <glm/ext/vector_float3.hpp>
#include <map>
#include <vector>

/// Renderer class which handles rendering of objects.
/// Internally stores a Camera, Window and a list of Object instances.
/// User code can inherit Renderer and provide implementations of the start()
/// and update() functions.
class Renderer {
public:
  /// Type alias for defining keybinds
  using KeyBind = std::function<void()>;

  /// Initialises the renderer.
  /// @param window Window object for the renderer to use. Must be in a valid
  /// state for rendering to succeed.
  /// @param camera Camera object used for viewing the scene.
  Renderer(Window &&window, const Camera &camera)
      : m_window{std::move(window)}, m_camera{camera} {}
  /// Constructs a new Window and initialises the renderer.
  /// @param windowName Name of the new window.
  /// @param camera Camera object used for viewing the scene.
  Renderer(std::string_view windowName, const Camera &camera)
      : m_window{windowName}, m_camera{camera} {}

  /// Custom start function to be executed before render loop.
  virtual void start() {}
  /// Custom update function called at each iteration of render loop
  virtual void update() {}
  /// Custom terminate function to be called after the window is closed.
  virtual void onTerminate() {}

  /// Returns the time passed since the last frame.
  float deltaTime() const { return m_deltaTime; }
  /// Returns Window for use with opengl functions, such as closing window.
  const Window &getWindow() const { return m_window; }
  /// Gets an object referenced by index.
  /// @param index The ID of the object   .
  Object *getObject(int index);
  /// Returns Camera to alter view of scene.
  Camera &getCamera() { return m_camera; }

  /// Sets keybind.
  /// The keybind function is called when the specified key is
  /// pressed.
  void setKeybind(unsigned int keycode, KeyBind keybind);

  /// Sets callback for mouse position.
  /// Can be used to rotate camera with mouse movement.
  ///@param mouseCallback The callback function to use.
  void setMouseCallback(
      std::function<void(GLFWwindow *window, double xPos, double yPos)>
          mouseCallback);
  /// Sets callback for when the mouse is scrolled.
  /// Can be used to zoom in scene when scrolled.
  ///@param scrollCallback The callback function to use.
  void setScrollCallback(
      std::function<void(GLFWwindow *window, double xOffset, double yOffset)>
          scrollCallback);

  /// Sets clear colour for window background.
  ///@param color A vec3 of the rgb colour to use.
  /// Values must be between 0 and 1.
  void setClearColor(const glm::vec3 &color);
  /// Sets clear colour for window background.
  ///@param red floating point red value between 0 and 1.
  ///@param green floating point green value between 0 and 1.
  ///@param blue floating point blue value between 0 and 1.
  void setClearColor(float red, float green, float blue);

  /// Adds an object to the scene.
  /// @return ID assigned to object.
  /// @param object Object to add to the scene.
  int addObject(Object *object);
  /// Adds a list of objects to the scene.
  /// @return A vector of IDs of the objects.
  /// @param objects Vector of objects to add to the scene.
  std::vector<int> addObjects(const std::vector<Object *> &objects);

  /// Executes render loop. Will loop indefinitely until Window is closed.
  void renderLoop();

  /// Closes window, which ends the render loop.
  void closeWindow();

private:
  // Internal members
  Window m_window;
  Camera m_camera;

  std::vector<Object *> m_objects{};

  std::map<unsigned int, KeyBind> keybinds{};
  glm::vec3 m_clearColor{0.1f, 0.1f, 0.1f};
  // Object ID for assigning new objects.
  int m_objectID{0};

  // Returns new ID for creating object.
  int getObjectID();
  // Called before first iteration of render loop.
  // Calls userStart.
  void awake();

  // Internal timing variables for frames
  float m_deltaTime{};
  float m_lastFrame{};
  // Helper functions for use in render loop.
  void processInput();
  void updateTime();
  // Called at end of render loop to clean up memory.
  void terminate();
};
