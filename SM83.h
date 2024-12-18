#pragma once
#include <cstdint>

class Bus;

class SM83 {

public:
    SM83();
    ~SM83();

public:
    Bus *bus = nullptr;

public: 

    // CPU Registers
    uint8_t   a  = 0x00;
    uint8_t   f  = 0x00;
    uint8_t   b  = 0x00;
    uint8_t   c  = 0x00;
    uint8_t   d  = 0x00;
    uint8_t   e  = 0x00;
    uint8_t   h  = 0x00;
    uint8_t   l  = 0x00;
    uint16_t  sp = 0x0000;
    uint16_t  pc = 0x0000;

    enum SM83FLAGS {
        z = 1 << 7;
        n = 1 << 6;
        h = 1 << 5;
        c = 1 << 4;
    }

    void connectBus(Bus *n) { bus = n; }

    void clock();

private:



};
