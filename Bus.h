#pragma once
#include "definitions.h"
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
    Cartridge bootrom = {"ROMS/dmg_boot.bin"};
    std::array<uint8_t, 8*1024> wram;
    std::array<uint8_t, 128> hram;
    uint8_t zero = 0x00;

    bool running = true;

    uint8_t bootromreg = 0x00;
    uint8_t sclock = 0x0000;
    uint8_t sb = 0x00;
    uint8_t serialoutbyte = sb;
    uint8_t sc = 0x00;

public:
    void cpuWrite(uint16_t addr, uint8_t data);
    uint8_t cpuRead(uint16_t addr);
    uint8_t* cpuReadPttr(uint16_t addr);
    void shutdown() { running = false; }
    
    void clockSerialTransfer();


};

