#include "programFunctions.hpp"
#include "Camera.hpp"
#include "Globals.hpp"
#include "Renderer.hpp"
#include "Shader.hpp"
#include "UniformBuffer.hpp"
#include "Window.hpp"
#include <GLFW/glfw3.h>
#include <cmath>
#include <cstdlib>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/quaternion_geometric.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/fwd.hpp>
#include <glm/trigonometric.hpp>

using Globals::renderer;

void setKeybinds() {
  renderer->setKeybind(GLFW_KEY_ESCAPE, onEscape);
  renderer->setKeybind(GLFW_KEY_W, onW);
  renderer->setKeybind(GLFW_KEY_A, onA);
  renderer->setKeybind(GLFW_KEY_S, onS);
  renderer->setKeybind(GLFW_KEY_D, onD);

  renderer->setMouseCallback(mouseCallback);
  renderer->setScrollCallback(scrollCallback);
}

void onEscape() { renderer->closeWindow(); }

void onW() {
  renderer->getCamera().move(Camera::FORWARD, renderer->deltaTime());
}

void onS() {
  renderer->getCamera().move(Camera::BACKWARD, renderer->deltaTime());
}

void onA() { renderer->getCamera().move(Camera::LEFT, renderer->deltaTime()); }

void onD() { renderer->getCamera().move(Camera::RIGHT, renderer->deltaTime()); }

void mouseCallback([[maybe_unused]] GLFWwindow *window, double xpos,
                   double ypos) {
  renderer->getCamera().mouseCallback(xpos, ypos);
}

void scrollCallback([[maybe_unused]] GLFWwindow *window,
                    [[maybe_unused]] double xoffset, double yoffset) {
  renderer->getCamera().processScroll(yoffset);
}

void runProgram() {
  setKeybinds();
  renderer->renderLoop();
}
