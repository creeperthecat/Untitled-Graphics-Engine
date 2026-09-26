#pragma once

#include "glm/ext/vector_float3.hpp"
#include "glm/fwd.hpp"

/// The Mouse struct stores data about mouse movement.
/// Used in Camera class to determine viewing angle.
struct Mouse {
  /// Boolean value to determine first mouse entry into the window
  bool firstMouse{true};
  /// X coordinate of the last known mouse position.
  double lastX{400};
  /// Y coordinate of the last known mouse position.
  double lastY{300};
  /// Mouse sensitivity (controls speed of mouse).
  float sensitivity{0.1f};
};

/// The Camera class enables control of how the scene is viewed.
/// Used in Renderer for viewing scene.
class Camera {
public:
  /// Enumerable for camera direction.
  /// Forward by default.
  enum Direction { FORWARD, BACKWARD, LEFT, RIGHT };

  /// Constructor for camera.
  /// Default constructs to a camera at the origin facing forwards.
  /// @param pos Initial position of the camera.
  /// @param up The up vector used to define the 3d axes of the camera.
  /// @param yaw Initial yaw angle in degrees.
  /// @param pitch Initial pitch angle in degrees.
  Camera(glm::vec3 pos = {0.0f, 0.0f, 0.0f}, glm::vec3 up = {0.0f, 1.0f, 0.0f},
         float yaw = -90.0f, float pitch = 0.0f);

  /// Returns current FOV of camera.
  float getFOV() { return m_fov; }
  /// Calculates and returns view matrix.
  const glm::mat4 view();
  /// Gets current camera position.
  const glm::vec3 &getPosition() { return m_pos; };
  /// Gets the direction which the camera is facing.
  const glm::vec3 &front() { return m_front; }

  /// Gets the current pitch of the camera in degrees.
  float getPitch() { return m_pitch; };
  /// Gets the current yaw of the camera in degrees.
  float getYaw() { return m_yaw; };

  /// Sets the speed of camera.
  /// @param speed New speed of the camera.
  void setSpeed(float speed) { m_speed = speed; }
  /// Sets the mouse sensitivity.
  /// @param sensitivity New sensitivity of the camera.
  void setSensitivity(float sensitivity) { m_mouse.sensitivity = sensitivity; }
  /// Sets the pitch of the camera.
  /// @param pitch New pitch of the camera (in degrees).
  void setPitch(float pitch) { m_pitch = pitch; }
  /// Set the yaw of the camera.
  /// @param yaw New yaw of the camera (in degrees).
  void setYaw(float yaw) { m_yaw = yaw; }

  /// Move the camera for one frame of length deltaTime.
  /// @param direction The Direction to move the camera in.
  /// @param deltaTime The time elapsed between the last frame and the current
  /// frame.
  void move(Direction direction, float deltaTime);
  /// Callback for mouse entry into window.
  /// @param xoffset The difference in x of the mouse position between the last
  /// frame and the current frame.
  /// @param yoffset The difference in x of the mouse position between the last
  /// frame and the current frame.
  void mouseCallback(double xoffset, double yoffset);
  /// Rotates camera based on mouse movement.
  /// @param xoffset The difference in x of the mouse position between the last
  /// frame and the current frame.
  /// @param yoffset The difference in x of the mouse position between the last
  /// frame and the current frame.
  /// @param constrainPitch Controls whether to constrain the pitch to avoid the
  /// camera facing 90 degrees up or down.
  void processMouse(double xoffset, double yoffset, bool constrainPitch = true);
  /// Zooms in camera based on mouse scroll.
  /// @param yoffset Amount scrolled between the last frame and current frame.
  void processScroll(double yoffset);

private:
  // Constant word up vector for construction of basis vectors.
  const glm::vec3 m_worldUp{0, 1, 0};
  // Mouse data used to calculate viewing angle.
  Mouse m_mouse{};

  // Euler angles used to determine view vectors.
  float m_yaw{-90.0f};
  float m_pitch{0.0f};

  // Internal view vectors and position used in calculation of view matrix.
  glm::vec3 m_front{0, 0, -1};
  glm::vec3 m_right{1, 0, 0};
  glm::vec3 m_up{0, 1, 0};
  glm::vec3 m_pos{0};

  // Configurable parameters to determine camera behaviour.
  float m_speed{1.0f};
  float m_fov{45.0f};

  // Private member function called at each frame.
  // Updates the 3 axes of the camera.
  void updateVectors();
};
