#pragma once
#include "definitions.h"
#include <cstdint>
#include <windows.h>
#include <vector>
#include <iostream>

class Bus;

class Joypad {

public:
    Joypad();
    ~Joypad();

private:
    uint8_t p1register = 0xcf;
    uint8_t p1registerprevious = 0xcf;

    void JoypadInterrupt();

    // select start b a down up left right
    std::vector<int> keyboardmapping = {16, 13, 90, 88, 40, 38, 37, 39};

    Bus *bus = nullptr;

public:
    void connectBus(Bus *n) { bus = n; }

    uint8_t read() {
        return p1register;
    }
    
    uint8_t* readPttr() {
        return &p1register;
    }
    
    void write(uint8_t value) {
        p1register = (value & 0xf0) | (p1register & 0x0f) ;
    }

    void update();

};