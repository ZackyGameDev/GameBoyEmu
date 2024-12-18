#include "Bus.h"

Bus::Bus() {
    cpu.connectBus(this);

    for (auto &i : hram) i = 0x00;
}


Bus::~Bus() {

}


void Bus::write(uint16_t addr, uint8_t data) {
    if (0xc000 <= addr and addr <= 0xdfff) {    
        hram[addr-0xc000] = data;
    }
}

uint8_t Bus::read(uint16_t addr) {
    uint8_t data = 0x00;
    if (0xc000 <= addr and addr <= 0xdfff) {
        data = hram[addr-0xc000];
    }
    return data;
}



