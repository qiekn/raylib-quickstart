#include "scenes/end-scene.h"
#include <raylib.h>
#include "utils/font.h"

void EndScene::Update() {}

void EndScene::Draw() {
  ClearBackground(RAYWHITE);
  const char* text = "Thanks for playing my game!";

  int fs = 32;
  Vector2 size = MeasureTextEx(NotoItlaic(), text, fs, 1);
  Vector2 pos = {(GetScreenWidth() - size.x) / 2.0f, 150};
  DrawTextEx(NotoItlaic(), text, pos, fs, 1, LIGHTGRAY);
}
