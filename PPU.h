#pragma once
#include "definitions.h"
#include <cstdint>
#include <array>
#include <SDL2/SDL.h>

class Bus;

class PPU {

public:
    PPU();
    ~PPU();

private:

    uint8_t debug_x = 12;
    uint8_t debug_y = 13;
    uint8_t debug_z = 12;
    
    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;

    #ifdef TILESET_WINDOW
    SDL_Window *tileset_window = nullptr;
    SDL_Renderer *tileset_renderer = nullptr;
    #endif

    #ifdef BG_WINDOW
    SDL_Window *bg_window = nullptr;
    SDL_Renderer *bg_renderer = nullptr;
    #endif

    std::array<uint8_t, 160> oam;
    std::array<uint8_t, 0x2000> vram;
    std::array<SDL_Texture*, 0x180> tileset; 
    SDL_Texture* background_layer;
    SDL_Texture* window_layer;
    
    // ordered in the way the STAT register expects the values to be.
    enum PPUMODE {
        HBLANK,
        VBLANK,
        OAMREAD,
        PIXELTRANSFER,
    };

    PPUMODE mode = OAMREAD;
    uint16_t cycles = 0;    
    uint8_t page_addressing_mode = 0;

    uint8_t lcdc = 0x91;
    uint8_t stat = 0x85;
    uint8_t scy = 0x00;
    uint8_t scx = 0x00;
    uint8_t ly = 0x00;
    uint8_t lyc = 0x00;
    uint8_t dma = 0xff;
    uint8_t bgp = 0xfc;
    uint8_t obp0 = 0x00;
    uint8_t obp1 = 0x00;
    uint8_t wy = 0x00;
    uint8_t wx = 0x00;

    uint8_t dma_prev = dma; // to detect changes we compare old to new

    std::vector<int> color_palette = {
        // 0xe6f8da, 0xb4e1fd, 0xaa9e62, 0x212121
        // 0xf4d8c6, 0xff9a8a, 0x364f4f, 0x332421 // first one with peach
        // 0xfdf0e7, 0xff9a8a, 0x364f4f, 0x332421 // first one with peach but lightvariation
        // 0xb76e79, 0xf4d8c6, 0xb89454, 0x332424 // cake
        // 0xf4d8c6, 0xb76e79, 0xb89454, 0x332424 // cake but lighter 
        // 0xbd2024, 0x6a5acd, 0x36454f, 0x110511 // scarlet 
        // 0xbd2024, 0xfdf0e7, 0x36454f, 0x110511 // scarlet but no purple
        // 0xfdf0e7, 0xbd2024, 0x36454f, 0x110511 // scarlet but no purple
        // 0x6a5acd, 0xbd2024, 0x36454f, 0x110511 // scarlet but switched 
        // 0xfdf0e7, 0xea7073, 0x36454f, 0x110511 // scarlet but switched no purple red lighter
        0xf7e5da, 0xd89454, 0x9085c4, 0x36454f // aro ace but lighter peach
    }; 

    enum LCDCFLAGS {
        BGWindowEnable = 1 << 0,
        OBJEnable = 1 << 1,
        OBJSize = 1 << 2,
        BGTileMapArea = 1 << 3,
        BGAndWindowTileDataArea = 1 << 4,
        WindowEnable = 1 << 5,
        WindowTileMapArea = 1 << 6,
        LCDAndPPUEnable = 1 << 7,
    };


    void setLCDCFlag(LCDCFLAGS flag, bool value) {
        if (value) {
            lcdc |= flag;
        } else {
            lcdc &= ~flag;
        }
    }

    uint8_t getLCDCFlag(LCDCFLAGS flag) {
        return ((lcdc & flag > 0) ? 1 : 0);
    }

private:
    void VBlankInterrupt();
    void LYCInterrupt();
    
    uint16_t last_pttr_addr; 
    uint8_t  last_pttr_value;
    bool vram_accessed = false;
    bool oam_accessed = false;
    void updateTextures();

public:
    Bus *bus = nullptr;
    void connectBus(Bus *n) { bus = n; }

public:
    void cpuWrite(uint16_t addr, uint8_t data);
    uint8_t cpuRead(uint16_t addr);
    uint8_t* cpuReadPttr(uint16_t addr);

public:
    void initLCD();
    void clock();
    // SDL_Texture* getTile(uint16_t addr);
    void getTile(uint16_t addr, SDL_Texture* &texture);
    void createTileset();
    void updateTileset();
    void updateBackgroundLayer();
    void drawBackground();

    #ifdef TILESET_WINDOW
    void drawTilesetWindow();
    #endif
    #ifdef BG_WINDOW
    void drawBGWindow();
    #endif
};