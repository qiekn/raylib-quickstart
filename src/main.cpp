#include <raylib.h>

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

const int kScreenWidth = 800;
const int kSreenHeight = 450;

void Loop();
void Update();
void Draw();

int main() {
  SetTraceLogLevel(LOG_WARNING);

  InitWindow(kScreenWidth, kSreenHeight, "game");

  SetTargetFPS(60);

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

void Update() {
  // TODO
}

void Draw() {
  BeginDrawing();

  ClearBackground(RAYWHITE);

  DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);

  EndDrawing();
}
