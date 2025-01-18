#include "Bus.h"
#include <iostream>

Bus::Bus() {
    cpu.connectBus(this);
    joypad.connectBus(this);
    ppu.connectBus(this);

    for (auto &i : hram) i = 0x00;
    
    std::cout << "[DEBUG] BUS CREATED <-------\n";

}


Bus::~Bus() {

}


uint8_t Bus::cpuRead(uint16_t addr) {
    uint8_t data = 0x00;
    if (0xc000 <= addr and addr <= 0xdfff) {
        data = hram[addr-0xc000];
    } else if (addr == 0xff00) {
        data = joypad.read();
    } else {
        std::cout << "[WARNING] " << std::hex << addr << std::dec << " INVALID READ ADDRESS <-------\n";
    }

    return data;
}


uint8_t* Bus::cpuReadPttr(uint16_t addr) {
    uint8_t *data = nullptr;
    if (0xc000 <=  addr and addr <= 0xdfff) {
        data = &hram[addr-0x8000];
    }
    return data;
}


void Bus::cpuWrite(uint16_t addr, uint8_t data) {
    if (0xc000 <= addr and addr <= 0xdfff) {    
        hram[addr-0xc000] = data;
    } else if (addr == 0xff00) {
        joypad.write(data);
    } else {
        std::cout << "[WARNING] INVALID WRITE ADDRESS <-------\n";
    }
}




