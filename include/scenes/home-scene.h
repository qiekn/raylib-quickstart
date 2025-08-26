#pragma once

#include "scenes/scene.h"

class HomeScene : public Scene {
public:
  HomeScene() = default;
  virtual ~HomeScene() = default;

  void Update() override;
  void Draw() override;
};
