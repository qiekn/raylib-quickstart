#pragma once

#include <raylib.h>
#include <list>
#include <set>
#include "managers/font-manager.h"
#include "utils/singleton.h"

/**
 * @class FontCache
 * @brief 用来处理中文字体
 *
 * 每次绘制中文时，都会调用LoadFontFromMemory加载字体，这个过程比较耗时。
 * FontCache类实现了一个简单的字体缓存机制，缓存最近使用的几种字号和字符集。
 * 当请求一个字号和字符集时，先检查缓存中是否已有对应的字体。
 * 这个类只对一个字体进行缓存，FontData 通过 FontManager 获取
 *
 */
class FontCache {
public:
  MAKE_SINGLETON(FontCache)

  const Font& GetFont();
  const Font& GetFont(const char* text);
  const Font& GetFont(int font_size, const char* text);

  void Init(const FontData& font_data);
  void Clear();

private:
  FontCache();
  ~FontCache();

  struct CacheEntry {
    Font font;
    std::set<int> codepoints;
    std::list<int>::iterator lru_iterator;
    bool is_valid{false};
  };

  /* fields */
  FontData font_data_;  // 从文件中读取的字体数据

  std::unordered_map<int, CacheEntry> cache_map_;
  std::list<int> lru_list_;

  static const int kMaxCachedFonts = 10;  // 最多缓存的字号数量
  static const int kMaxCodepointsPerFont = 7000;

  /* methods */
  bool NeedUpdate(int font_size, const std::set<int>& new_codepoints);         // Missing codepoints
  const Font& UpdateFont(int font_size, const std::set<int>& new_codepoints);  // Update font with new codepoints

  void UpdateLRU(int font_size);  // 移动至队头
  void EvictLRU();                // 移除最少使用的字体字号
};
