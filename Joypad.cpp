#include "Joypad.h"
#include "Bus.h"
#include "SM83.h"


Joypad::Joypad() {
    std::cout << "[DEBUG] Joypad created <-----" << std::endl;
    std::cout << "keyboard maps: \n";
    for (int i : keyboardmapping) std::cout << i << " "; 
    std::cout << std::endl;
    
}

Joypad::~Joypad() {
    std::cout << "[DEBUG] Joypad destroyed? <-----\n" << std::endl;
}

void Joypad::JoypadInterrupt() {
    bus->cpu.requestInterrupt(SM83::InterruptFlags::Joypad);
}

void Joypad::update() {
    p1registerprevious = p1register;
    if (~p1register & 0x30) for (int i = 0; i < 4; i++) {
        bool dpad = p1register & 0x20;
        // for (int i : keyboardmapping) std::cout << i << " "; 
        if (GetAsyncKeyState(keyboardmapping[i + (4*dpad)]) & 0x8000) {
            p1register &= ~(1 << (3-i));
        } else {
            p1register |= (1 << (3-i));
        }
        // std::cout << "GETIT\n";
    }

    // for JOYPAD interrupt
    if (p1register & 0x30 < 0x30) // if one of the two modes are selected (buttons/directions)
    for (int i = 0; i < 4; i++) {
        // if any bit went from high to low
        if (p1register & (1 << i) < p1registerprevious & (1 << i)) {
            JoypadInterrupt();
            break;
        }
    }

}
