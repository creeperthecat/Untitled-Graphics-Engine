#include "Camera.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/geometric.hpp"
#include "glm/trigonometric.hpp"
#include <iostream>

Camera::Camera(glm::vec3 pos, glm::vec3 up, float yaw, float pitch)
    : m_pos{pos}, m_worldUp{up}, m_yaw{yaw}, m_pitch{pitch} {
  updateVectors();
}

void Camera::move(Direction direction, float deltaTime) {
  float velocity = m_speed * deltaTime;
  switch (direction) {
  case FORWARD:
    m_pos += m_front * velocity;
    break;
  case BACKWARD:
    m_pos -= m_front * velocity;
    break;
  case RIGHT:
    m_pos += m_right * velocity;
    break;
  case LEFT:
    m_pos -= m_right * velocity;
    break;
  default:
    std::cout << "Invalid camera direction.\n";
  }
}

const glm::mat4 Camera::view() {
  return glm::lookAt(m_pos, m_pos + m_front, m_up);
}

void Camera::mouseCallback(double xpos, double ypos) {
  if (m_mouse.firstMouse) {
    m_mouse.lastX = xpos;
    m_mouse.lastY = ypos;
    m_mouse.firstMouse = false;
  }

  float xoffset{static_cast<float>(xpos - m_mouse.lastX)};
  float yoffset{static_cast<float>(m_mouse.lastY -
                                   ypos)}; // reversed, as y starts at bottom
  m_mouse.lastX = xpos;
  m_mouse.lastY = ypos;

  processMouse(xoffset, yoffset);
}

void Camera::processMouse(double xoffset, double yoffset, bool constrainPitch) {
  xoffset *= m_mouse.sensitivity;
  yoffset *= m_mouse.sensitivity;

  m_yaw += xoffset;
  m_pitch += yoffset;

  if (constrainPitch) {
    if (m_pitch > 89.0f)
      m_pitch = 89.0f;
    if (m_pitch < -89.0f)
      m_pitch = -89.0f;
  }

  updateVectors();
}

void Camera::processScroll(double yoffset) {
  m_fov -= static_cast<float>(yoffset);

  if (m_fov < 1.0f)
    m_fov = 1.0f;
  if (m_fov > 45.0f)
    m_fov = 45.0f;
}

void Camera::updateVectors() {
  glm::vec3 direction{};
  direction.x =
      static_cast<float>(cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch)));
  direction.y = static_cast<float>(sin(glm::radians(m_pitch)));
  direction.z =
      static_cast<float>(sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch)));

  m_front = glm::normalize(direction);
  m_right = glm::normalize(glm::cross(m_front, m_worldUp));
  m_up = glm::normalize(glm::cross(m_right, m_front));
}
