#pragma once
#include <cstdint>
#include <windows.h>
#include <vector>
#include <iostream>

class Bus;

class Joypad {

public:
    Joypad() {
        std::cout << "[DEBUG] Joypad created <-----\n" << std::endl;
    }

private:
    uint8_t p1register = 0x00;

    // select start b a down up left right
    std::vector<int> keyboardmapping = {16, 13, 90, 88, 40, 38, 37, 39};


public:
    Bus *bus = nullptr;
    void connectBus(Bus *n) { bus = n; }


    uint8_t read() {
        return p1register;
    }
    
    void write(uint8_t value) {
        p1register = (value & 0xf0) | (p1register & 0x0f) ;
    }

    void update() {
        if (~p1register & 0x30) for (int i = 0; i < 4; i++) {
            bool dpad = p1register & 0x20;
            if (GetAsyncKeyState(keyboardmapping[i + (4*dpad)]) & 0x8000) {
                p1register &= ~(1 << (3-i));
            } else {
                p1register |= (1 << (3-i));
            }
        }
    }

};