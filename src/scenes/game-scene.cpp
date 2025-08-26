#include "scenes/game-scene.h"
#include <raylib.h>

void GameScene::Update() {}

void GameScene::Draw() {
  ClearBackground(RAYWHITE);
  DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);
}
