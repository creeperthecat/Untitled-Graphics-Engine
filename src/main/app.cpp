#include "Camera.hpp"
#include "Globals.hpp"
#include "InstancedModel.hpp"
#include "Window.hpp"
#include "glm/fwd.hpp"
#include "programFunctions.hpp"
#include <GLFW/glfw3.h>
#include <memory>

const Camera createCamera() {
  glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
  glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
  Camera camera{cameraPos, cameraUp};
  camera.setSpeed(2.5f);
  camera.setSensitivity(0.1f);

  return camera;
}

using Globals::renderer;

int main([[maybe_unused]] int argc, const char *argv[]) {
  Globals::programPath = std::string{argv[0]};
  renderer = std::make_unique<SceneRenderer>(
      Window{Globals::windowName, 1200, 700}, createCamera());

  runProgram();
  return 0;
};
