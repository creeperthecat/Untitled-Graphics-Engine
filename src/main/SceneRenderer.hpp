#pragma once
#include "Renderer.hpp"

class SceneRenderer : public Renderer {
  using Renderer::Renderer;

public:
  void start() override;
  void update() override;
  void onTerminate() override;

private:
};
