#pragma once

#include "scenes/scene.h"

class EndScene : public Scene {
public:
  EndScene() = default;
  virtual ~EndScene() = default;

  void Update() override;
  void Draw() override;
};
