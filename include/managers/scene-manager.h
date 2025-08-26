#pragma once

#include <memory>
#include <unordered_map>
#include "scenes/scene.h"

class SceneManager {
public:
  SceneManager();
  virtual ~SceneManager();

  void Update();
  void Draw();

  void ChangeScene(SceneId id);
  void ChangeScene(int index);

private:
  SceneId current_scene_id_{SceneId::kGame};
  size_t frame_counter_{0};
  std::unordered_map<SceneId, std::unique_ptr<Scene>> scenes_;
};
