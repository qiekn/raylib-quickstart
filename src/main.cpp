#include <raylib.h>
#include "constants.h"
#include "managers/scene-manager.h"

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

// Constants Here
const int kScreenWidth = 800;
const int kSreenHeight = 450;

// Global Variables
SceneManager scene_manager;

// Function Declarations
void Loop();
void Update();
void Draw();

int main() {
  SetTraceLogLevel(LOG_WARNING);

  InitWindow(kScreenWidth, kSreenHeight, "game");

  SetTargetFPS(kFPS);

#if defined(PLATFORM_WEB)
  emscripten_set_main_loop(Loop, 0, 1);
#else
  SetTargetFPS(120);
  while (!WindowShouldClose()) {
    Loop();
  }
#endif

  CloseWindow();
  return 0;
}

void Loop() {
  Update();
  Draw();
}

void Update() { scene_manager.Update(); }

void Draw() {
  BeginDrawing();
  scene_manager.Draw();
  EndDrawing();
}
