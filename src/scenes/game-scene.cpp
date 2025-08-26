#include "scenes/game-scene.h"
#include <raylib.h>
#include "managers/font-manager.h"

void GameScene::Update() {}

void GameScene::Draw() {
  ClearBackground(RAYWHITE);
  const char* text = "Congrats! You created your first window!";

  int font_size = 32;
  Vector2 text_size = MeasureTextEx(NotoItalic(), text, font_size, 1);
  Vector2 text_pos = {(GetScreenWidth() - text_size.x) / 2.0f, 150};
  DrawTextEx(NotoItalic(), text, text_pos, font_size, 1, LIGHTGRAY);
}
