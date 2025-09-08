#include "utils/font.h"
#include <raylib.h>
#include "managers/font-manager.h"
#include "utils/font-cache.h"

void DrawTextUtil(const char* text, int x, int y, int fs, Color color);

void ck::DrawText(const char* text, int x, int y, int fs, Color color) {
  const auto& fd = FontManager::Get().GetNotoSCData();
  DrawTextUtil(text, x, y, fs, color);
}

void ck::DrawTextPixel(const char* text, int x, int y, int fs, Color color) {
  const auto& fd = FontManager::Get().GetPixelSCData();
  DrawTextUtil(text, x, y, fs, color);
}

Vector2Int ck::MeasurePixelText(const char* text, int fs) {
  Vector2 size = MeasureTextEx(FontCache::Get().GetFont(fs, text), text, (float)fs, (float)TextStyle::kSpacing);
  int w = fs / 16;
  size.x -= w;
  size.y += w;
  return {size.x, size.y};
}

Vector2Int ck::MeasurePixelText(const std::string& text, int fs) { return MeasurePixelText(text.c_str(), fs); }

void DrawTextUtil(const char* text, int x, int y, int fs, Color color) {
  const Font& font = FontCache::Get().GetFont(fs, text);
  DrawTextEx(font, text, Vector2{(float)x, (float)y}, (float)fs, (float)TextStyle::kSpacing, color);
}

void ck::UpdateFont(const char* text) { FontCache::Get().GetFont(FontSize::kText, text); }

void ck::UpdateFont(const std::string& str) { UpdateFont(str.c_str()); }

void ck::DrawTextPixelCenter(const char* text, int x, int y, int fs, Color color) {
  Vector2Int size = MeasurePixelText(text, fs);
  DrawTextPixel(text, x - size.x / 2, y - size.y / 2, fs, color);
}
