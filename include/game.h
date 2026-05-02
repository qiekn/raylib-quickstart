#pragma once

#include <raylib.h>

#include "imgui_layer.h"
#include "layer_stack.h"

class GameLayer;

class Game {
public:
  void Run();
  void Loop();   // public so the emscripten trampoline can reach it

private:
  void Init();   // configs need before init window
  void Start();  // configs need opengl content
  void Update();
  void Draw();
  void Shutdown();

  void ToggleBorderless();

private:
  static constexpr int kScreenWidth = 1280;
  static constexpr int kScreenHeight = 720;
  static constexpr int kTargetFps = 144;

  LayerStack layers_;
  ImGuiLayer* imgui_layer_ = nullptr;  // non-owning; layers_ owns the unique_ptr.
  GameLayer* game_layer_ = nullptr;    // non-owning; layers_ owns the unique_ptr.

  bool borderless_ = false;
  Vector2 windowed_pos_{};
  Vector2 windowed_size_{};
};
