#pragma once
#define SDL_MAIN_HANDLED
// #define DEBUGMODE_
#define EMULATION_CLOCK_SPEED 1048576.0 // in m clocks per second
// #define EMULATION_CLOCK_/SPEED 953 // in nanoseconds per machine clock
#define DEBUG_CLOCK_SPEED 1

// #define ROM_PATH "ROMS/Tetris (Japan) (En).gb"
// #define ROM_PATH "ROMS/Castlevania - The Adventure (USA).gb"
#define ROM_PATH "ROMS/Dr. Mario (JU) (V1.1).gb"
// #define ROM_PATH "ROMS/Alleyway (World).gb"
// #define ROM_PATH "ROMS/dk.gb"
// #define ROM_PATH "ROMS/Bomberman GB (U) [S][!].gb"
// #define ROM_PATH "ROMS/Darkwing Duck (U) [!].gb"
// #define ROM_PATH "ROMS/cpu_instrs.gb"
// #define ROM_PATH "ROMS/test9.gb"

#define LOADMEMDUMP
#define MEM_DUMP_PATH "ROMS/mem.dump"
// #define MEM_DUMP_PATH "ROMS/tet.mem.dump"
// #define TILESET_WINDOW // these dont work and i'm not 
// #define BG_WINDOW      // very interested in making them
// #define LOG_PC
#define NO_LOGS
// #define NO_SERIAL_OUT
// #define FULL_VIEWPORT
// #define BREAKPOINT_PC_FILE
// #define NO_CPU
#define SKIP_BOOTROM
// #define HIDE_SPRITES