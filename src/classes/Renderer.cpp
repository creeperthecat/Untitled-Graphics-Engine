#include "Renderer.hpp"
#include "Primitive.hpp"
#include <GLFW/glfw3.h>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <functional>
#include <glm/detail/type_mat2x2.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/fwd.hpp>
#include <glm/trigonometric.hpp>
#include <iostream>
#include <math.h>
#include <vector>

Object *Renderer::getObject(int ID) {
  assert((0 <= ID && ID < m_objects.size()) && "Invalid object ID.");
  return m_objects[ID];
}

void Renderer::setKeybind(unsigned int keycode, std::function<void()> onPress) {
  keybinds[keycode] = onPress;
}

void Renderer::setMouseCallback(
    std::function<void(GLFWwindow *window, double xPos, double yPos)>
        mouseCallback) {
  glfwSetInputMode(getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSetCursorPosCallback(
      m_window,
      *mouseCallback.target<void (*)(GLFWwindow *, double, double)>());
}

void Renderer::setScrollCallback(
    std::function<void(GLFWwindow *window, double xOffset, double yOffset)>
        scrollCallback) {
  glfwSetScrollCallback(
      m_window,
      *scrollCallback.target<void (*)(GLFWwindow *, double, double)>());
}

int Renderer::addObject(Object *object) {
  m_objects.push_back(object);
  m_objects.back()->setID(getObjectID());
  return m_objects.back()->getID();
}

std::vector<int> Renderer::addObjects(const std::vector<Object *> &objects) {
  std::vector<int> objectIDs{};
  for (const auto &o : objects) {
    objectIDs.push_back(addObject(o));
  }
  return objectIDs;
}

void Renderer::setClearColor(const glm::vec3 &color) { m_clearColor = color; }

void Renderer::setClearColor(float red, float green, float blue) {
  m_clearColor = {red, green, blue};
}

void Renderer::closeWindow() { m_window.close(); }

void Renderer::renderLoop() {
  awake();

  while (!glfwWindowShouldClose(m_window)) {
    updateTime();
    processInput();
    m_window.clear(m_clearColor);

    update();

    glfwSwapBuffers(m_window);
    glfwPollEvents();
  }
  terminate();
}

int Renderer::getObjectID() { return m_objectID++; }

void Renderer::updateTime() {
  float currentFrame = static_cast<float>(glfwGetTime());
  m_deltaTime = currentFrame - m_lastFrame;
  m_lastFrame = currentFrame;
}

void Renderer::processInput() {
  for (const auto &key : keybinds) {
    if (glfwGetKey(m_window, static_cast<int>(key.first)) == GLFW_PRESS) {
      key.second();
    }
  }
}

void Renderer::awake() {
  if (!m_window)
    return;

  start();
}

void Renderer::terminate() {
  onTerminate();
  m_objects.clear();
  m_window.terminate();
}
