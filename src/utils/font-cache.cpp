#include "utils/font-cache.h"
#include <raylib.h>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "constants.h"
#include "managers/font-manager.h"
#include "raygui.h"
#include "utils/font.h"

FontCache::FontCache() {
  const FontData& fd = FontManager::Get().GetPixelSCData();
  Init(fd);
}

FontCache::~FontCache() { Clear(); }

void FontCache::Init(const FontData& font_data) {
  TraceLog(LOG_WARNING, "FontCache Start.");
  font_data_ = font_data;

  // process en
  std::string text =
      "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789,.:!@#$%^&"
      "*()[]{}<>?/\\|`~\"'_-+= \n\r\t";

  // process zh-ch
  // 1. open file
  std::filesystem::path chinese_txt = kAssets / "zh-sc-3500.txt";
  std::ifstream file(chinese_txt.string());
  if (!file.is_open()) {
    TraceLog(LOG_ERROR, "File to open: %s", chinese_txt.string().c_str());
    return;
  }
  // 2. read file to a string
  std::stringstream buffer;
  buffer << file.rdbuf();
  std::string hanzi = buffer.str();
  // 3. concat
  text += hanzi;

  const Font& font = GetFont(text.c_str());
  GuiSetFont(font);
  GuiSetStyle(DEFAULT, TEXT_SIZE, FontSize::kText);
  TraceLog(LOG_WARNING, "FontCache initialized.");
}

void FontCache::Clear() {
  for (auto& pair : cache_map_) {
    if (pair.second.is_valid) {
      UnloadFont(pair.second.font);
    }
  }
  cache_map_.clear();
  lru_list_.clear();
}

const Font& FontCache::GetFont() { return cache_map_[lru_list_.front()].font; }

const Font& FontCache::GetFont(const char* text) { return GetFont(FontSize::kText, text); }

const Font& FontCache::GetFont(int font_size, const char* text) {
  // Convert text to codepoints array
  int count;
  int* codepoints = LoadCodepoints(text, &count);

  // Convert codepoints array to std::set
  std::set<int> codepoints_set;
  for (int i = 0; i < count; i++) {
    codepoints_set.insert(codepoints[i]);
  }
  UnloadCodepoints(codepoints);

  return UpdateFont(font_size, codepoints_set);
}

bool FontCache::NeedUpdate(int font_size, const std::set<int>& new_codepoints) {
  // 该字号字体没有被缓存，所有的 Codepoints 都是缺失的
  auto it = cache_map_.find(font_size);
  if (it == cache_map_.end() || !it->second.is_valid) {
    return true;
  }

  const auto& cached_codepoints = it->second.codepoints;
  for (int cp : new_codepoints) {
    if (cached_codepoints.find(cp) == cached_codepoints.end()) {
      return true;
    }
  }
  return false;
}

const Font& FontCache::UpdateFont(int font_size, const std::set<int>& new_codepoints) {
  if (!NeedUpdate(font_size, new_codepoints)) {
    UpdateLRU(font_size);
    return cache_map_[font_size].font;
  }

  // 如果缓存已满，并且请求的一个新的字号, 弹出最近未使用的字号
  if (cache_map_.size() >= kMaxCachedFonts && cache_map_.find(font_size) == cache_map_.end()) {
    EvictLRU();
  }

  // 更新字体
  CacheEntry& entry = cache_map_[font_size];
  lru_list_.push_front(font_size);

  // 合并 codepoints
  entry.codepoints.insert(new_codepoints.begin(), new_codepoints.end());
  std::vector<int> codepoint_array(entry.codepoints.begin(), entry.codepoints.end());

  // 释放旧字体
  if (entry.is_valid) {
    UnloadFont(entry.font);
  }

  // 加载新字体
  Font new_font = LoadFontFromMemory(".ttf", font_data_.data, font_data_.size, font_size, codepoint_array.data(),
                                     codepoint_array.size());
  GuiSetFont(new_font);
  entry.is_valid = true;

  // 更新缓存
  entry.font = new_font;
  entry.lru_iterator = lru_list_.begin();
  entry.is_valid = true;

  UpdateLRU(font_size);

  return entry.font;
}

void FontCache::UpdateLRU(int font_size) {
  auto& entry = cache_map_[font_size];
  if (entry.is_valid) {
    // 从当前位置移除
    lru_list_.erase(entry.lru_iterator);
  }
  // 插入到链表头部
  lru_list_.push_front(font_size);
  entry.lru_iterator = lru_list_.begin();
}

void FontCache::EvictLRU() {
  if (lru_list_.empty()) return;

  int lru_font_size = lru_list_.back();
  lru_list_.pop_back();

  auto it = cache_map_.find(lru_font_size);
  if (it != cache_map_.end() && it->second.is_valid) {
    UnloadFont(it->second.font);
    cache_map_.erase(it);
  }
}
