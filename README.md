# Quickstart

## Build for Desktop

```bash
cmake -S . -B build && cmake --build build
./build/game
```

## Build for WebAssembly

Firstly read [Raylib Docs](https://github.com/raysan5/raylib/wiki/Working-for-Web-(HTML5))

Install [emscripten toolchain](https://emscripten.org/docs/getting_started/downloads.html).
If you are on macOS, you can install Emscripten using Homebrew: `brew install emscripten`


```bash
mkdir build
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=/Users/leoua7/Apps/emsdk/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake -DPLATFORM=Web
make
