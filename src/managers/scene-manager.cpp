#include "managers/scene-manager.h"
#include <raylib.h>
#include <memory>
#include "constants.h"
#include "scenes/end-scene.h"
#include "scenes/game-scene.h"
#include "scenes/home-scene.h"
#include "scenes/logo-scene.h"
#include "scenes/scene.h"

SceneManager::SceneManager() {
  scenes_.emplace(SceneId::kLogo, std::make_unique<LogoScene>());
  scenes_.emplace(SceneId::kHome, std::make_unique<HomeScene>());
  scenes_.emplace(SceneId::kGame, std::make_unique<GameScene>());
  scenes_.emplace(SceneId::kEnd, std::make_unique<EndScene>());
}

SceneManager::~SceneManager() {}

void SceneManager::Update() {
  // switch scene base on current_scene_id_
  switch (current_scene_id_) {
    case SceneId::kLogo:
      frame_counter_++;
      if (frame_counter_ > 2 * kFPS) {
        current_scene_id_ = SceneId::kHome;
      }
      break;
    case SceneId::kHome:
      if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER)) {
        current_scene_id_ = SceneId::kGame;
      }
      break;
    case SceneId::kGame:
      break;
    case SceneId::kEnd:
      break;
    default:
      break;
  }
  // scene update
  auto it = scenes_.find(current_scene_id_);
  if (it != scenes_.end() && it->second) {
    it->second->Update();
  }
}

void SceneManager::Draw() {
  auto it = scenes_.find(current_scene_id_);
  if (it != scenes_.end() && it->second) {
    it->second->Draw();
  }
}

void SceneManager::ChangeScene(SceneId id) { current_scene_id_ = id; }

void SceneManager::ChangeScene(int index) {
  if (index < 0 || index >= static_cast<int>(SceneId::kCount)) {
    return;
  }
  current_scene_id_ = static_cast<SceneId>(index);
  frame_counter_ = 0;
}
