#pragma once

enum class SceneId {
  kLogo,
  kHome,
  kGame,
  kPause,
  KEnd,
  kCount,  // Used for ChangeScene() index check
};

class Scene {
public:
  virtual void Update() = 0;
  virtual void Draw() = 0;

  Scene() = default;
  virtual ~Scene() = default;
};
