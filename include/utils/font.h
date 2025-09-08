#pragma once

#include <raylib.h>
#include <string>
#include "managers/font-manager.h"
#include "utils/vector2int.h"

namespace FontSize {
constexpr int kTitle = 48;
constexpr int kSubtitle = 32;
constexpr int kText = 16;
};  // namespace FontSize

namespace TextStyle {
constexpr int kSpacing = 0;
}

inline Font NotoRegular() { return FontManager::Get().Regular(); }
inline Font NotoItlaic() { return FontManager::Get().Italic(); }
inline Font MesloMono() { return FontManager::Get().Mono(); }

namespace ck {
void UpdateFont(const char* text);
void UpdateFont(const std::string& str);
void DrawText(const char* text, int x, int y, int fs, Color color);
void DrawTextPixel(const char* text, int x, int y, int fs, Color color);
void DrawTextPixelCenter(const char* text, int x, int y, int fs, Color color);
Vector2Int MeasurePixelText(const std::string& text, int fs = FontSize::kText);
Vector2Int MeasurePixelText(const char* text, int fs = FontSize::kText);
}  // namespace ck
