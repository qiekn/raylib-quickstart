#pragma once

#include <memory>

#include <raylib.h>

#include "layer.h"
#include "managers/scene-manager.h"

// Owns the offscreen RenderTexture2D the game scene draws into, then displays
// it in a "Viewport" ImGui window. Also hosts placeholder Hierarchy and Console
// panels so the default dock layout has something in every region.
//
// The actual game-side update / render is delegated to SceneManager — this
// layer is the bridge between the scene system and the ImGui editor chrome.
class GameLayer : public Layer {
public:
  GameLayer();
  ~GameLayer() override;

  void OnAttach() override;
  void OnDetach() override;
  void OnUpdate(float dt) override;
  void OnRender() override;
  void OnImGuiRender() override;

  bool* ShowViewportPtr() { return &show_viewport_; }
  bool* ShowHierarchyPtr() { return &show_hierarchy_; }
  bool* ShowConsolePtr() { return &show_console_; }
  bool* ViewportNoTitleBarPtr() { return &viewport_no_titlebar_; }

  // Push the active scene background color (typically the ImGui theme's
  // background, sourced by Game::Render). Cheap to call every frame.
  void SetBackgroundColor(Color c) { background_color_ = c; }

  // Exposed for Game::Render's "ImGui hidden = scene fullscreen" path.
  // When the chrome is hidden we resize the RT to window size (so the
  // blit is 1:1) and blit it directly to the backbuffer instead of
  // routing through ImGui::Image.
  void EnsureTargetSize(int w, int h) { EnsureTarget(w, h); }
  const RenderTexture2D& Target() const { return target_; }
  bool TargetValid() const { return target_valid_; }

private:
  void EnsureTarget(int w, int h);
  void DrawScene();
  void DrawViewportPanel();
  void DrawHierarchyPanel();
  void DrawConsolePanel();

  RenderTexture2D target_{};
  int target_w_ = 0;
  int target_h_ = 0;
  bool target_valid_ = false;

  bool show_viewport_ = true;
  bool show_hierarchy_ = true;
  bool show_console_ = true;
  bool viewport_no_titlebar_ = false;

  Color background_color_{30, 30, 46, 255};  // overridden per-frame by Game

  std::unique_ptr<SceneManager> scene_manager_;
};
