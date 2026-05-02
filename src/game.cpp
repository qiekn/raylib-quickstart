#include "game.h"

#include <raylib.h>
#include <rlgl.h>

#include <cstdio>
#include <memory>

#include <imgui.h>

#include "constants.h"
#include "game_layer.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

namespace {
constexpr const char* kWindowStateFile = "window.state";

struct WindowState {
  int x;
  int y;
  int w;
  int h;
};

WindowState LoadWindowState(int default_w, int default_h) {
  WindowState s{80, 80, default_w, default_h};
  if (FILE* f = std::fopen(kWindowStateFile, "r")) {
    int x, y, w, h;
    int n = std::fscanf(f, "%d %d %d %d", &x, &y, &w, &h);
    if (n >= 4 && w > 0 && h > 0) {
      s.x = x;
      s.y = y;
      s.w = w;
      s.h = h;
    }
    std::fclose(f);
  }
  return s;
}

void SaveWindowState() {
  Vector2 pos = GetWindowPosition();
  if (FILE* f = std::fopen(kWindowStateFile, "w")) {
    std::fprintf(f, "%d %d %d %d\n", (int)pos.x, (int)pos.y, GetScreenWidth(), GetScreenHeight());
    std::fclose(f);
  }
}
}  // namespace

#if defined(PLATFORM_WEB)
namespace {
Game* g_web_loop_game = nullptr;
void WebLoopTrampoline() {
  if (g_web_loop_game) g_web_loop_game->Loop();
}
}  // namespace
#endif

void Game::Run() {
  Init();
  InitWindow(kScreenWidth, kScreenHeight, "game");
  Start();

  // Main loop
#if defined(PLATFORM_WEB)
  g_web_loop_game = this;
  emscripten_set_main_loop(WebLoopTrampoline, 0, 1);
#else
  SetTargetFPS(kTargetFps);
  while (!WindowShouldClose()) {
    Loop();
  }
#endif

  Shutdown();
}

void Game::Init() {
  SetTraceLogLevel(LOG_WARNING);
  SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_HIGHDPI);
}

void Game::Start() {
  const WindowState state = LoadWindowState(kScreenWidth, kScreenHeight);
  SetWindowSize(state.w, state.h);
  SetWindowPosition(state.x, state.y);

  auto imgui_layer = std::make_unique<ImGuiLayer>();
  auto game_layer = std::make_unique<GameLayer>();

  imgui_layer_ = imgui_layer.get();
  game_layer_ = game_layer.get();

  imgui_layer_->BindGamePanelToggles(game_layer->ShowViewportPtr(), game_layer->ShowHierarchyPtr(),
                                     game_layer->ShowConsolePtr(), game_layer->ViewportNoTitleBarPtr());

  // Order matters: ImGuiLayer must submit DockSpaceOverViewport before
  // GameLayer's Viewport window so the panel can dock into the central node
  // on the first frame.
  layers_.push_layer(std::move(imgui_layer));
  layers_.push_layer(std::move(game_layer));
}

void Game::Loop() {
  Update();
  Draw();
}

void Game::Update() {
  const float dt = GetFrameTime();
  if (IsKeyPressed(KEY_F11)) {
    ToggleBorderless();
  }
  for (auto& layer : layers_) {
    layer->OnUpdate(dt);
  }
}

void Game::ToggleBorderless() {
  if (!borderless_) {
    windowed_pos_ = GetWindowPosition();
    windowed_size_ = {(float)GetScreenWidth(), (float)GetScreenHeight()};

    const int monitor = GetCurrentMonitor();
    const Vector2 mpos = GetMonitorPosition(monitor);
    const int mw = GetMonitorWidth(monitor);
    const int mh = GetMonitorHeight(monitor);

    SetWindowState(FLAG_WINDOW_UNDECORATED);
    SetWindowPosition((int)mpos.x, (int)mpos.y);
    // +1 px so Windows doesn't auto-promote this to exclusive fullscreen
    // (WS_POPUP + exact-monitor-size triggers fullscreen optimizations).
    SetWindowSize(mw, mh + 1);
    borderless_ = true;
  } else {
    ClearWindowState(FLAG_WINDOW_UNDECORATED);
    SetWindowSize((int)windowed_size_.x, (int)windowed_size_.y);
    SetWindowPosition((int)windowed_pos_.x, (int)windowed_pos_.y);
    borderless_ = false;
  }
}

void Game::Draw() {
  // Sync the scene background to the active ImGui theme so the RT inside
  // the Viewport panel matches the surrounding chrome.
  game_layer_->SetBackgroundColor(imgui_layer_->BackgroundColor());

  // When ImGui is hidden the Viewport panel doesn't run — size the RT to
  // the framebuffer so the fullscreen blit below is 1:1 in real pixels.
  // GetScreenWidth/Height is logical (DPI-unscaled), GetRenderWidth/Height
  // is the actual framebuffer; using the logical size on a HiDPI display
  // would produce a smaller RT that then gets stretched by the blit.
  const bool imgui_visible = imgui_layer_->IsVisible();
  if (!imgui_visible) {
    game_layer_->EnsureTargetSize(GetRenderWidth(), GetRenderHeight());
  }

  BeginDrawing();
  ClearBackground(imgui_layer_->BackgroundColor());

  for (auto& layer : layers_) {
    layer->OnRender();
  }

  // ImGui hidden: blit the scene RT straight to the backbuffer instead of
  // routing through ImGui::Image. raylib FBOs are y-flipped relative to
  // the backbuffer — negative src height un-flips on the way out.
  if (!imgui_visible && game_layer_->TargetValid()) {
    const RenderTexture2D& rt = game_layer_->Target();
    const Rectangle src{0, 0, (float)rt.texture.width, -(float)rt.texture.height};
    const Rectangle dst{0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()};
    DrawTexturePro(rt.texture, src, dst, Vector2{0, 0}, 0.0f, WHITE);
  }

  rlDrawRenderBatchActive();

  imgui_layer_->Begin();
  if (imgui_visible) {
    for (auto& layer : layers_) {
      layer->OnImGuiRender();
    }
  }
  imgui_layer_->End();

  EndDrawing();
}

void Game::Shutdown() {
  if (borderless_) ToggleBorderless();
  SaveWindowState();
  layers_.clear();  // detach layers before the GL context goes away
  CloseWindow();
}
