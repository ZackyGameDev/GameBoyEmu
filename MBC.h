#pragma once
#include "definitions.h"
#include <cstdint>
#include <array>

class Cartridge;

class MBC { // this is a base class.
public:
    MBC();
    ~MBC();

public:
    Cartridge *cart = nullptr;
    void connectCart(Cartridge *n) { cart = n; }

public:
    virtual void cpuWrite(uint16_t addr, uint8_t data);
    virtual uint8_t cpuRead(uint16_t addr);
    virtual uint8_t* cpuReadPttr(uint16_t addr);

};

class MBC1 : public MBC {

public:
    MBC1();
    ~MBC1();

private:
    uint8_t sram_enabled = 0x00;
    uint8_t rom_bank_number = 0x01;
    uint8_t ram_bank_number = 0x00;
    uint8_t banking_mode = 0x00;

public:
    void cpuWrite(uint16_t addr, uint8_t data) override;
    uint8_t cpuRead(uint16_t addr) override;
    uint8_t* cpuReadPttr(uint16_t addr) override;
};