#pragma once
#include "SM83.h"
#include <array>
#include <cstdint>

class Bus {

public:
    Bus();
    ~Bus();

public:
    SM83 cpu;
    std::array<uint8_t, 4*1024> HRAM;

public:
    void write(uint16_t addr, uint8_t data);
    uint8_t read(uint16_t addr);

};
