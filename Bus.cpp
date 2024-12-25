#include "Bus.h"
#include <iostream>

Bus::Bus() {
    cpu.connectBus(this);

    for (auto &i : hram) i = 0x00;
    
    std::cout << "[DEBUG] BUS CREATED <-------\n";

}


Bus::~Bus() {

}


uint8_t Bus::cpuRead(uint16_t addr) {
    uint8_t data = 0x00;
    if (0xc000 <= addr and addr <= 0xdfff) {
        data = hram[addr-0xc000];
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
    }
}




