#include "scenes/home-scene.h"
#include <raylib.h>

void HomeScene::Update() {}

void HomeScene::Draw() {
  ClearBackground(GREEN);
  DrawText("LOGO SCREEN", 20, 20, 40, LIGHTGRAY);
}
