#include "Bus.h"
#include <iostream>
// 
// "ROMS/Tetris (Japan) (En).gb"
// "ROMS/mytestrom.gb"
Bus::Bus() : cart("ROMS/Tetris (Japan) (En).gb") {
    cpu.connectBus(this);
    joypad.connectBus(this);
    ppu.connectBus(this);
    cart.connectBus(this);

    for (auto &i : wram) i = 0x00;
    for (auto &i : hram) i = 0x00;
    
    std::cout << "[DEBUG] BUS CREATED <-------" << std::endl;

}


Bus::~Bus() {

}


uint8_t Bus::cpuRead(uint16_t addr) {
    uint8_t data = 0x00;

    if (0x0000 <= addr and addr <= 0x7fff) {
        data = cart.read(addr);
    } else if (0xa000 <= addr and addr <= 0xbfff) {
        data = cart.sram[addr - 0xa000];
    } else if (0xc000 <= addr and addr <= 0xdfff) {
        data = wram[addr-0xc000];
    } else if ((0x8000 <= addr and addr <= 0x9fff) // VRAM
            or (0xfe00 <= addr and addr <= 0xfe9f) // OAM
            or (0xff40 <= addr and addr <= 0xff4b)) { // PPU REG
        data = ppu.cpuRead(addr);
    } else if (addr == 0xff00) {
        data = joypad.read();
    } else if (addr == 0xff0f) {
        data = cpu.if_;
    } else if (0xff00 <= addr and addr <= 0xff7f){
        data = 0x00; // default for port mode registers
    } else if (0xff80 <= addr and addr <= 0xfffe) {
        data = hram[addr - 0xff80];
    } else if (addr == 0xffff) {
        data = cpu.ie;
    } else {
        std::cout << "[WARNING] " << std::hex << addr << std::dec << " INVALID READ ADDRESS <------" << std::endl;
    }

    return data;
}


uint8_t* Bus::cpuReadPttr(uint16_t addr) {
    uint8_t *data = nullptr;
    if (0x0000 <= addr and addr <= 0x7fff) {
        data = cart.readPttr(addr);
    } else if (0xa000 <= addr and addr <= 0xbfff) {
        data = &cart.sram[addr - 0xa000];
    } else if (0xc000 <=  addr and addr <= 0xdfff) {
        data = &wram[addr-0xc000];
    } else if ((0x8000 <= addr and addr <= 0x9fff) // VRAM
            or (0xfe00 <= addr and addr <= 0xfe9f) // OAM
            or (0xff40 <= addr and addr <= 0xff4b)) { // PPU REG
        data = ppu.cpuReadPttr(addr);
    } else if (addr == 0xff00) {
        data = joypad.readPttr();
    } else if (addr == 0xff0f) {
        data = &cpu.if_;
    } else if (0xff00 <= addr and addr <= 0xff7f){
        data = &zero; // default for port mode registers
    } else if (0xff80 <= addr and addr <= 0xfffe) {
        data = &hram[addr - 0xff80];
    } else if (addr == 0xffff) {
        data = &cpu.ie;
    } else {
        std::cout << "[WARNING] " << std::hex << addr << std::dec << " INVALID READPTTR ADDRESS <-------" << std::endl;
    }
    return data;
}


void Bus::cpuWrite(uint16_t addr, uint8_t data) {
    if (0xc000 <= addr and addr <= 0xdfff) {    
        wram[addr-0xc000] = data;
    } else if (0xa000 <= addr and addr <= 0xbfff) {
        cart.sram[addr - 0xa000] = data;
    } else if (addr == 0xff00) {
        joypad.write(data);
    } else if (addr == 0xff0f) {
        cpu.if_ = data;
    } else if ((0x8000 <= addr and addr <= 0x9fff) // VRAM
            or (0xfe00 <= addr and addr <= 0xfe9f) // OAM
            or (0xff40 <= addr and addr <= 0xff4b)) { // PPU REG
        ppu.cpuWrite(addr, data);
    } else if (0xff80 <= addr and addr <= 0xfffe) {
        hram[addr - 0xff80] = data;
    } else if (addr == 0xffff) {
        cpu.ie = data;
    } else {
        std::cout << "[WARNING] " << std::hex << addr << std::dec << " INVALID WRITE ADDRESS <-------" << std::endl;
    }
}




