# GameBoyEmu

## Demo

<video src="https://github.com/ZackyGameDev/GameBoyEmu/raw/refs/heads/main/gbemudemo.mp4" controls width="720"></video>

> All ROMs shown in the demo are legally obtained and are **not** distributed with this repository.

---

## Overview

This is a Nintendo **DMG (Original Game Boy)** emulator written in **C++17** using **SDL2**.

This project was built purely for educational purposes to deeply understand:

* The SM83 CPU
* Memory-mapped hardware
* The Game Boy PPU rendering pipeline
* Cartridge bank switching (MBC1)
* Hardware execution flow and timing behavior

This is not a production emulator.
It is a hardware learning project.

---

## Current Feature Set

* Full **SM83 CPU instruction implementation**
* Memory bus abstraction
* PPU rendering
* Basic timing model
* Support for:

  * **MBC free cartridges**
  * **Most MBC1 cartridges**
* Windows keyboard input via `windows.h`

Not supported:

* MBC3 / MBC5 / RTC
* Game Boy Color

---

## Platform

Currently built and tested on **Windows (MSYS2 / MinGW)**.

Input handling uses `windows.h`, so this build is Windows-specific.

---

## ROM Loading

ROMs are not passed as command-line arguments.

Instead, you must hardcode the ROM path inside:

```
definitions.h
```

Example:

```cpp
// #define ROM_PATH "ROMS/Super Mario Land (World).gb"
#define ROM_PATH "ROMS/Kirby's Dream Land (USA, Europe).gb"
// #define ROM_PATH "ROMS/Crystal Quest (USA).gb"
```

ROMs are not included in this repository.
You must provide your own legally obtained `.gb` files.

---

## Boot ROM Behavior

Inside `definitions.h`:

```cpp
#define SKIP_BOOTROM
```

If you **comment this out**, the emulator will execute the boot ROM and display the Nintendo logo animation before transferring control to the cartridge.

If enabled, the emulator skips directly to post-boot state.

---

## Keyboard Mapping

Input uses Windows virtual key codes:

```cpp
// start select b a down up left right
std::array<int, 8> keyboardmapping = {13, 16, 90, 88, 40, 38, 37, 39};
```

Mapping:

| Key              | Action |
| ---------------- | ------ |
| Enter (13)       | Start  |
| Shift (16)       | Select |
| Z (90)           | B      |
| X (88)           | A      |
| Arrow Down (40)  | Down   |
| Arrow Up (38)    | Up     |
| Arrow Left (37)  | Left   |
| Arrow Right (39) | Right  |

Modify `Joypad.h` if you want different bindings.

---

## Color Palettes

Inside `PPU.h`, the `PPU` class contains:

```cpp
std::vector<uint32_t> color_palette
```

Multiple preset palettes are provided (original DMG, greyscale, experimental palettes, etc.).

Uncomment the palette you prefer.

Format:

```
0xRRGGBBAA
```

---

## Build Instructions (Windows / MSYS2)

### Requirements

* MSYS2 (UCRT64 toolchain)
* SDL2
* SDL2_ttf

### Build Command

Equivalent to:

```bash
g++ -fdiagnostics-color=always ^
-Iinclude ^
-Llib ^
-g ^
main.cpp SM83.cpp Bus.cpp Joypad.cpp Cartridge.cpp PPU.cpp MBC.cpp ^
-lwinmm -lSDL2main -lSDL2 -lSDL2_ttf ^
-o build\a.exe ^
-std=c++17
```

Output binary:

```
build/a.exe
```

---

## Project Structure

Top-level files:

```
Bus.cpp / Bus.h
SM83.cpp / SM83.h
PPU.cpp / PPU.h
Cartridge.cpp / Cartridge.h
MBC.cpp / MBC.h
Joypad.cpp / Joypad.h
definitions.h
main.cpp
opcodes.json
```

Other directories:

* `ROMS/` – Local ROM storage (not included)
* `include/`, `lib/` – SDL dependencies
* `build/` – Compiled output
* `fonts/` – Font resources
* `experiments/` – Scratch work
* `.vscode/` – Editor configuration

---

## Helper Scripts

The Python files (`codewriter.py`, `codewriter2.py`, `romwriter.py`) are personal helper scripts used to generate repetitive C++ code (e.g., opcode wrappers). They are not part of the emulator runtime.

---

## Opcode Tracking

`track.png` and `prefixedtrack.png` are personal opcode tracking sheets used during development to manually mark implemented instructions. They are not used by the emulator.

---

## Limitations

* Windows-only input handling
* Not fully cycle-perfect
* No advanced MBC types
* No Game Boy Color support
* Compatibility not exhaustively tested

The emulator runs most **No MBC** and **MBC1** titles, but edge cases may still exist.

---

## Purpose

This project exists to understand the Game Boy hardware by implementing:

* Instruction decoding
* Flag behavior
* Memory-mapped I/O
* PPU state transitions
* Bank switching logic
* Hardware-driven execution flow

It was built to learn, not to compete.
