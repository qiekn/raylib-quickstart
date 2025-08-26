#pragma once

#include <raylib.h>
#include <cstddef>
#include "utils/singleton.h"

namespace FontSize {
constexpr float kTitle = 32.0f;
constexpr float kSubtitle = 24.0f;
constexpr float kText = 16.0f;
};  // namespace FontSize

class FontManager {
  MAKE_SINGLETON(FontManager)
public:
  const Font& Regular() const { return normal_font_; }
  const Font& Italic() const { return itlaic_font_; }
  const Font& Mono() const { return mono_; }

private:
  FontManager()
      : normal_font_(LoadFontEx("assets/fonts/noto-regular.ttf", 128, NULL, 0)),
        itlaic_font_(LoadFontEx("assets/fonts/noto-italic.ttf", 128, NULL, 0)),
        mono_(LoadFontEx("assets/fonts/meslo-regular.ttf", 128, NULL, 0)) {}

  ~FontManager() {
    UnloadFont(normal_font_);
    UnloadFont(itlaic_font_);
    UnloadFont(mono_);
  }

  Font normal_font_;
  Font itlaic_font_;
  Font mono_;
};

inline Font NotoRegular() { return FontManager::Get().Regular(); }
inline Font NotoItalic() { return FontManager::Get().Italic(); }
inline Font Mono() { return FontManager::Get().Mono(); }
