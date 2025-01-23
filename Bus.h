#pragma once
#include "SM83.h"
#include "Joypad.h"
#include "PPU.h"
#include "Cartridge.h"
#include <array>
#include <cstdint>

class Bus {

public:
    Bus();
    ~Bus();

public:
    SM83 cpu;
    Joypad joypad;
    PPU ppu;
    Cartridge cart;
    std::array<uint8_t, 8*1024> wram;
    std::array<uint8_t, 128> hram;

public:
    void cpuWrite(uint16_t addr, uint8_t data);
    uint8_t cpuRead(uint16_t addr);
    uint8_t* cpuReadPttr(uint16_t addr);

};
