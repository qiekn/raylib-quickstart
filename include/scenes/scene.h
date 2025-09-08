#pragma once

enum class SceneId {
  kLogo,
  kHome,
  kGame,
  kEnd,
  kTest,
  kCount,  // Used for ChangeScene() index check
};

class Scene {
public:
  Scene() = default;
  virtual ~Scene() = default;

  void Init() { is_initialized_ = true; }

  virtual void Start() {}
  virtual void Update() = 0;
  virtual void Draw() = 0;

  bool IsStarted() const { return is_initialized_; }

private:
  bool is_initialized_{false};
};
