# Quick start

## macOS Users

```bash
brew install raylib
git clone https://github.com/qiekn/raylib-quickstart.git
cmake -B build && make -j$(nproc) -C build
./build/game
```

`compile_commands.json` for neovim lsp config:

```bash
ls -s build/compile_commands.json ./compile_commands.json
```
