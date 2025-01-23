#pragma once
#include <cstdint>
#include <array>
#define SDL_MAIN_HANDLED
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

    std::array<uint8_t, 160> oam;
    std::array<uint8_t, 0x1fff> vram;
    
    // ordered in the way the STAT register expects the values to be.
    enum PPUMODE {
        HBLANK,
        VBLANK,
        OAMREAD,
        PIXELTRANSFER,
    };

    PPUMODE mode = OAMREAD;
    uint8_t cycles = 0;
    uint8_t page_addressing_mode = 0;

    uint8_t lcdc = 0x00;
    uint8_t stat = 0x00;
    uint8_t scy = 0x00;
    uint8_t scx = 0x00;
    uint8_t ly = 0x00;
    uint8_t lyc = 0x00;
    uint8_t dma = 0x00;
    uint8_t bgp = 0x00;
    uint8_t obp0 = 0x00;
    uint8_t obp1 = 0x00;
    uint8_t wy = 0x00;
    uint8_t wx = 0x00;

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
        return (lcdc & flag > 0) ? 1 : 0;
    }

private:
    void handleHBlankInterrupt();
    void handleVBlankInterrupt();
    void handleLYCInterrupt();

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

};