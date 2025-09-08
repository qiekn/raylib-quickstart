#pragma once

#include <memory>
#include "managers/scene-manager.h"

class Game {
public:
  void Run();

private:
  void Init();   // configs need before init window
  void Start();  // configs need opengl content
  void Loop();   // used by webassembly
  void Update();
  void Draw();

  std::unique_ptr<SceneManager> scene_manager_;
};
