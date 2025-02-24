#include "Bus.h"
#include <iostream>
// 
// "ROMS/Tetris (Japan) (En).gb"
// "ROMS/mytestrom.gb"
// "ROMS/cpu_instrs.gb"
Bus::Bus() : cart(ROM_PATH) {
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

    if (0x0000 <= addr and addr <= 0x00ff) {
        data = bootrom.read(addr);
    } else if (0x0100 <= addr and addr <= 0x7fff) {
        data = cart.read(addr);
    } else if ((0x8000 <= addr and addr <= 0x9fff) // VRAM
            or (0xfe00 <= addr and addr <= 0xfe9f) // OAM
            or (0xff40 <= addr and addr <= 0xff4b)) { // PPU REG
        data = ppu.cpuRead(addr);
    } else if (0xa000 <= addr and addr <= 0xbfff) {
        data = cart.sram[addr - 0xa000];
    } else if (0xc000 <= addr and addr <= 0xdfff) {
        data = wram[addr-0xc000];
    } else if (0xff00 <= addr and addr <= 0xff7f) {
        switch (addr) {
        case 0xff00: data = joypad.read(); break;
        // case 0xff01: data = sb; break;
        // case 0xff02: data = sc; break;
        case 0xff04: data = cpu.div; break;
        case 0xff05: data = cpu.tima; break;
        case 0xff06: data = cpu.tma; break;
        case 0xff07: data = cpu.tac; break;
        case 0xff0f: data = cpu.if_; break;
        default:
            data = 0x00; // default for port mode registers
            std::cout << "[WARNING] " << std::hex << addr << std::dec << " PORT NOT IMPLEMENTED <--" << std::endl;
        }
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
    
    if (0x0000 <= addr and addr <= 0x00ff) {
        data = bootrom.readPttr(addr);
    } else if (0x0000 <= addr and addr <= 0x7fff) {
        data = cart.readPttr(addr);
    } else if ((0x8000 <= addr and addr <= 0x9fff) // VRAM
            or (0xfe00 <= addr and addr <= 0xfe9f) // OAM
            or (0xff40 <= addr and addr <= 0xff4b)) { // PPU REG
        data = ppu.cpuReadPttr(addr);
    } else if (0xa000 <= addr and addr <= 0xbfff) {
        data = cart.readPttr(addr);
    } else if (0xc000 <=  addr and addr <= 0xdfff) {
        data = &wram[addr-0xc000];
    } else if (0xff00 <= addr and addr <= 0xff7f){
        switch (addr) {
        case 0xff00: data = joypad.readPttr(); break;
        // case 0xff01: data = sb; break;
        // case 0xff02: data = sc; break;
        case 0xff04: data = &cpu.div; break;
        case 0xff05: data = &cpu.tima; break;
        case 0xff06: data = &cpu.tma; break;
        case 0xff07: data = &cpu.tac; break;
        case 0xff0f: data = &cpu.if_; break;
        default:
            data = &zero; // default for port mode registers
            std::cout << "[WARNING] " << std::hex << addr << std::dec << " PORT NOT IMPLEMENTED <--" << std::endl;
        }
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
    } else if ((0x8000 <= addr and addr <= 0x9fff) // VRAM
            or (0xfe00 <= addr and addr <= 0xfe9f) // OAM
            or (0xff40 <= addr and addr <= 0xff4b)) { // PPU REG
        ppu.cpuWrite(addr, data);
    } else if (0xff00 <= addr and addr <= 0xff7f) {
        switch (addr) {
        case 0xff00: joypad.write(data);
        case 0xff04: cpu.div  = data; break;
        case 0xff05: cpu.tima = data; break;
        case 0xff06: cpu.tma  = data; break;
        case 0xff07: cpu.tac  = data; break;
        case 0xff0f: cpu.if_  = data; break;
        default:
            data = 0x00; // default for port mode registers
            std::cout << "[WARNING] " << std::hex << addr << std::dec << " PORT NOT IMPLEMENTED <--" << std::endl;
        }  
    } else if (0xff80 <= addr and addr <= 0xfffe) {
        hram[addr - 0xff80] = data;
    } else if (addr == 0xffff) {
        cpu.ie = data;
    } else {
        std::cout << "[WARNING] " << std::hex << addr << std::dec << " INVALID WRITE ADDRESS <-------" << std::endl;
    }
}




