#include "SceneRenderer.hpp"
#include "GLFW/glfw3.h"
#include "GeometryShader.hpp"
#include "Globals.hpp"
#include "InstancedArray.hpp"
#include "InstancedModel.hpp"
#include "Model.hpp"
#include "Primitive.hpp"
#include "Shader.hpp"
#include "Shape.hpp"
#include "StandardShader.hpp"
#include "Stride.hpp"
#include "VertexArray.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"
#include "glm/trigonometric.hpp"
#include <cmath>
#include <cstdlib>
#include <string>
#include <vector>

void SceneRenderer::start() {
  setClearColor(0.1f, 0.1f, 0.1f);
  getCamera().setSpeed(10.0f);

  srand(glfwGetTime());
  constexpr float radius = 50.f;
  constexpr float offset = 2.5f;

  auto randomDisplacement = [offset]() {
    return (rand() % static_cast<int>(2 * offset * 100)) / 100.f - offset;
  };

  for (unsigned int i{0}; i < Globals::modelMatrices.size(); ++i) {
    glm::mat4 model{1.0f};
    float angle{static_cast<float>(i) /
                static_cast<float>(Globals::modelMatrices.size()) * 360.f};
    float x{static_cast<float>(sin(angle) * radius + randomDisplacement())};
    float y{randomDisplacement() * 0.4f};
    float z{static_cast<float>(cos(angle) * radius + randomDisplacement())};

    model = glm::translate(model, glm::vec3{x, y, z});

    float scale{static_cast<float>((rand() % 20) / 100.f + 0.05)};
    model = glm::scale(model, glm::vec3{scale});

    float rotation{static_cast<float>(rand() % 360)};
    constexpr glm::vec3 rotationAxis{0.4f, 0.6f, 0.8f};
    model = glm::rotate(model, rotation, rotationAxis);

    Globals::modelMatrices[i] = model;
  }

  Globals::asteroidID = addObject(new InstancedModel{
      Globals::asteriodModel,
      new StandardShader{Globals::asteroidVertex, Globals::asteroidFragment},
      Globals::modelMatrices, Stride{4, 4, 4, 4}});

  Globals::planetID = addObject(new Model{
      Globals::planetModel,
      new StandardShader{Globals::planetVertex, Globals::planetFragment}});
}

void SceneRenderer::update() {

  glEnable(GL_DEPTH_TEST);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  Object *asteroid{getObject(Globals::asteroidID)};
  Object *planet{getObject(Globals::planetID)};

  glm::mat4 projection{glm::perspective(glm::radians(45.0f),
                                        getWindow().getRatio(), 0.1f, 1000.f)};
  glm::mat4 view{getCamera().view()};

  asteroid->getShader()->setMat4("projection", projection);
  asteroid->getShader()->setMat4("view", view);
  planet->getShader()->setMat4("projection", projection);
  planet->getShader()->setMat4("view", view);

  glm::mat4 model{1.0f};
  model = glm::translate(model, glm::vec3{0.0f, -3.0f, 0.0f});
  model = glm::scale(model, glm::vec3{4.0f, 4.0f, 4.0f});
  planet->getShader()->setMat4("model", model);
  planet->draw();

  asteroid->draw();
}

void SceneRenderer::onTerminate() {}
