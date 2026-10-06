# Simple Platform Game

This is a simple platform game using Godot game engine with C++ GDExtension.
This project was built based on [godot-cpp-template](https://github.com/godotengine/godot-cpp-template).

## How to use

### Requirements

- Scons
- Python 3
- Pkg-Config
- C++ compiler

If you use NixOS, then just run `nix develop`.

### Compile

Run `scons` to compile to project. If you need `compile_commands.json` file for code editor, run `scons compiledb=yes` instead.

After compiling, open `project` folder in Godot, you can run the game.
