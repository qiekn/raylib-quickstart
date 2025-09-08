#include "scenes/game-scene.h"
#include <raylib.h>
#include "utils/font.h"

void GameScene::Update() {}

void GameScene::Draw() {
  ClearBackground(RAYWHITE);
  const char* text = "Game Scene";

  int font_size = 32;
  Vector2 text_size = MeasureTextEx(NotoItlaic(), text, font_size, 1);
  Vector2 text_pos = {(GetScreenWidth() - text_size.x) / 2.0f, 150};
  DrawTextEx(NotoItlaic(), text, text_pos, font_size, 1, LIGHTGRAY);
}
