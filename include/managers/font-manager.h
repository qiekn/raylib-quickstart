#pragma once

#include <raylib.h>
#include "constants.h"
#include "utils/singleton.h"

struct FontData {
  unsigned char* data;  // read from file
  int size;             // data size
};

class FontManager {
  MAKE_SINGLETON(FontManager)
public:
  const Font& Regular() const { return normal_font_; }
  const Font& Italic() const { return italic_font_; }
  const Font& Mono() const { return mono_font_; }

  FontData GetNotoSCData() const { return {noto_font_data_, noto_font_size_}; }
  FontData GetPixelSCData() const { return {fusion_font_data_, fusion_font_size_}; }

private:
  // clang-format off
  FontManager()
    : normal_font_(LoadFontEx((kFontPath / "noto-regular.ttf").string().c_str(), 128, 0, 0)), // for HiDPI 64px
      italic_font_(LoadFontEx((kFontPath / "noto-italic.ttf").string().c_str(), 128, 0, 0)),
      mono_font_(LoadFontEx((kFontPath / "meslo-regular.ttf").string().c_str(), 128, 0, 0)),
      noto_font_data_(LoadFileData((kFontPath / "noto-sc.ttf").string().c_str(), &noto_font_size_)),
      fusion_font_data_(LoadFileData((kFontPath / "fusion.ttf").string().c_str(), &fusion_font_size_))
  {}
  // clang-format on

  ~FontManager() {
    UnloadFont(normal_font_);
    UnloadFont(italic_font_);
    UnloadFont(mono_font_);
    UnloadFileData(noto_font_data_);
    UnloadFileData(fusion_font_data_);
  }

  /* English */
  Font normal_font_;
  Font italic_font_;
  Font mono_font_;

  /* Chinese */
  unsigned char* noto_font_data_;    // noto sans
  unsigned char* fusion_font_data_;  // fusion pixel

  int noto_font_size_;
  int fusion_font_size_;

  void LoadFontData();
};
