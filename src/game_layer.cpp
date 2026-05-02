#include "game_layer.h"

#include <imgui.h>
#include <imgui_internal.h>

GameLayer::GameLayer() : Layer("GameLayer") {}

GameLayer::~GameLayer() = default;

void GameLayer::OnAttach() {
  // Allocate something non-zero so the first frame has a valid texture even
  // before the Viewport panel reports its real size.
  EnsureTarget(1280, 720);

  scene_manager_ = std::make_unique<SceneManager>();
}

void GameLayer::OnDetach() {
  scene_manager_.reset();
  if (target_valid_) {
    UnloadRenderTexture(target_);
    target_valid_ = false;
  }
}

void GameLayer::OnUpdate(float /*dt*/) {
  if (scene_manager_) scene_manager_->Update();
}

void GameLayer::OnRender() {
  if (!target_valid_) return;
  DrawScene();
}

void GameLayer::OnImGuiRender() {
  if (show_viewport_) DrawViewportPanel();
  if (show_hierarchy_) DrawHierarchyPanel();
  if (show_console_) DrawConsolePanel();
}

void GameLayer::EnsureTarget(int w, int h) {
  if (w < 1 || h < 1) return;
  if (target_valid_ && target_w_ == w && target_h_ == h) return;
  if (target_valid_) UnloadRenderTexture(target_);
  target_ = LoadRenderTexture(w, h);
  // POINT filter avoids the BILINEAR half-pixel smear that turns 1px grid
  // lines into uneven 1/2px streaks when ImGui::Image scales the RT by a
  // non-integer factor. Pixel-art friendly default; switch to BILINEAR if
  // your scene benefits from smoothing.
  SetTextureFilter(target_.texture, TEXTURE_FILTER_POINT);
  target_w_ = w;
  target_h_ = h;
  target_valid_ = true;
}

void GameLayer::DrawScene() {
  BeginTextureMode(target_);
  ClearBackground(background_color_);
  if (scene_manager_) scene_manager_->Draw();
  EndTextureMode();
}

void GameLayer::DrawViewportPanel() {
  if (!show_viewport_) return;

  ImGuiWindowFlags flags =
      ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;
  if (viewport_no_titlebar_) flags |= ImGuiWindowFlags_NoTitleBar;

  // Zero padding so the framebuffer fills the entire panel.
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0, 0});
  bool open = ImGui::Begin("Viewport", nullptr, flags);
  ImGui::PopStyleVar();

  if (!open) {
    ImGui::End();
    return;
  }

  // Auto-hide the dock node's tab bar when only Viewport is in the node.
  // Other windows docked into the same node bring the tab bar back so
  // the user can switch between them.
  if (ImGui::IsWindowDocked()) {
    if (ImGuiDockNode* node = ImGui::GetWindowDockNode()) {
      if (node->Windows.Size == 1) {
        node->LocalFlags |= ImGuiDockNodeFlags_NoTabBar;
      } else {
        node->LocalFlags &= ~ImGuiDockNodeFlags_NoTabBar;
      }
    }
  }

  const ImVec2 avail = ImGui::GetContentRegionAvail();
  EnsureTarget(static_cast<int>(avail.x), static_cast<int>(avail.y));

  if (target_valid_) {
    // raylib renders the FBO upside down relative to ImGui's UV convention,
    // so flip V.
    const ImTextureID tex_id = static_cast<ImTextureID>(target_.texture.id);
    ImGui::Image(tex_id, avail, ImVec2(0, 1), ImVec2(1, 0));
  }

  // Right-click anywhere in the viewport for the toggle — essential when the
  // title bar is hidden, since the menu bar route still works too. The Image
  // fills the panel so we must NOT pass NoOpenOverItems, otherwise the popup
  // never opens.
  if (ImGui::BeginPopupContextWindow("ViewportContext", ImGuiPopupFlags_MouseButtonRight)) {
    ImGui::MenuItem("Hide Title Bar", nullptr, &viewport_no_titlebar_);
    ImGui::EndPopup();
  }

  ImGui::End();
}

void GameLayer::DrawHierarchyPanel() {
  if (!show_hierarchy_) return;
  if (!ImGui::Begin("Hierarchy", nullptr, ImGuiWindowFlags_NoCollapse)) {
    ImGui::End();
    return;
  }
  if (ImGui::TreeNodeEx("Scene", ImGuiTreeNodeFlags_DefaultOpen)) {
    ImGui::BulletText("Camera");
    ImGui::BulletText("Player");
    ImGui::BulletText("World");
    ImGui::TreePop();
  }
  ImGui::End();
}

void GameLayer::DrawConsolePanel() {
  if (!show_console_) return;
  if (!ImGui::Begin("Console", nullptr, ImGuiWindowFlags_NoCollapse)) {
    ImGui::End();
    return;
  }
  ImGui::TextDisabled("[INFO] Game viewport initialized.");
  ImGui::TextDisabled("[INFO] Default dock layout applied.");
  ImGui::TextDisabled("[HINT] Press ` to toggle the editor chrome.");
  ImGui::End();
}
