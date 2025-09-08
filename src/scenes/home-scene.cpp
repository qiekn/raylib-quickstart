#include "scenes/home-scene.h"
#include <raylib.h>
#include "utils/font.h"

void HomeScene::Update() {}

void HomeScene::Draw() {
  ClearBackground(RAYWHITE);
  const char* text = "Home Scene: Press Space or Enter to Start";

  int fs = 32;
  Vector2 size = MeasureTextEx(NotoItlaic(), text, fs, 1);
  Vector2 pos = {(GetScreenWidth() - size.x) / 2.0f, 150};
  DrawTextEx(NotoItlaic(), text, pos, fs, 1, LIGHTGRAY);
}
