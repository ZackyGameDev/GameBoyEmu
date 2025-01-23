#pragma once
#include <vector>
#include <iostream>
#include <cstdint>
#include <array>
#include <string>


class Bus;

class Cartridge {
public:
    Cartridge(const std::string filename);
    ~Cartridge();

    std::array<uint8_t, 0x2000> sram;
private:
    struct RomHeader {
        char name[16];
    } header;

    std::vector<uint8_t> rom;

    Bus *bus = nullptr;

public:
    void connectBus(Bus *n) { bus = n; }

    uint8_t read(uint16_t address);
    uint8_t* readPttr(uint16_t address);
    void write(uint16_t address, uint8_t data);

};
