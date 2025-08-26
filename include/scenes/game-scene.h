#pragma once

#include "scenes/scene.h"

class GameScene : public Scene {
public:
  GameScene() = default;
  virtual ~GameScene() = default;

  void Update() override;
  void Draw() override;
};
