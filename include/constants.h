#pragma once

#include <filesystem>

const std::filesystem::path kAssets = ASSETS;
const std::filesystem::path kFontPath = kAssets / "fonts";
const std::filesystem::path kImagePath = kAssets / "images";

const int kScreenWidth = 800;
const int kSreenHeight = 450;

const int kFPS = 120;
