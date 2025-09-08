#include "scenes/logo-scene.h"
#include <raylib.h>
#include "utils/font.h"

void LogoScene::Update() {}

void LogoScene::Draw() {
  ClearBackground(RAYWHITE);
  const char* text = "Congrats! You created your first window!";

  int font_size = 32;
  Vector2 text_size = MeasureTextEx(NotoItlaic(), text, font_size, 1);
  Vector2 text_pos = {(GetScreenWidth() - text_size.x) / 2.0f, 150};
  DrawTextEx(NotoItlaic(), text, text_pos, font_size, 1, LIGHTGRAY);
}
