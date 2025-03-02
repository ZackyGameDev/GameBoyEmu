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
    
    #ifndef NO_LOGS
    std::cout << "[DEBUG] BUS CREATED <-------" << std::endl;
    #endif

}


Bus::~Bus() {

}


void Bus::clockSerialTransfer() {
    // if ((sc >> 7) & 0x1) {
    if (sc == 0x81) {
        if (sclock == 8) { // i.e. 8 bits have been transfered.
            #ifndef NO_SERIAL_OUT
            std::cout << "[SERIAL] " << (char)sb << " : " << (int)sb << '\n';
            #endif
            sclock = 0;
            sc = 0x01;
            sb = 0xFF;
            cpu.requestInterrupt(SM83::InterruptFlags::Serial);
        } else {
            if (sclock == 0)
                serialoutbyte = sb;
            ++sclock;
            sb = (serialoutbyte << (sclock)) | ((1 << sclock) - 1);
        }
    }
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
    } else if (0xe000 <= addr and addr <= 0xfdff) {
        data = wram[addr-0xe000]; // echo ram.
    } else if (0xff00 <= addr and addr <= 0xff7f) {
        switch (addr) {
        case 0xff00: data = joypad.read(); break;
        case 0xff01: data = sb; break;
        case 0xff02: data = sc; break;
        case 0xff04: data = cpu.div; break;
        case 0xff05: data = cpu.tima; break;
        case 0xff06: data = cpu.tma; break;
        case 0xff07: data = cpu.tac; break;
        case 0xff0f: data = cpu.if_; break;
        case 0xff10 ... 0xff26: break; //data = apu.readPttr(addr);
        case 0xff30 ... 0xff3f: break; //data = apu.readPttr(addr);
        case 0xff50: data = bootromreg; break;
        default:
            data = 0x00; // default for port mode registers
            #ifndef NO_LOGS
            std::cout << "[WARNING] " << std::hex << addr << std::dec << " PORT NOT IMPLEMENTED <--" << std::endl;
            #endif
        }
    } else if (0xff80 <= addr and addr <= 0xfffe) {
        data = hram[addr - 0xff80];
    } else if (addr == 0xffff) {
        data = cpu.ie;
    } else {
            #ifndef NO_LOGS
            std::cout << "[WARNING] " << std::hex << addr << std::dec << " INVALID READ ADDRESS <------" << std::endl;
            #endif
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
    } else if (0xe000 <=  addr and addr <= 0xfdff) {
        data = &wram[addr-0xe000];
    } else if (0xff00 <= addr and addr <= 0xff7f){
        switch (addr) {
        case 0xff00: data = joypad.readPttr(); break;
        case 0xff01: data = &sb; break;
        case 0xff02: data = &sc; break;
        case 0xff04: data = &cpu.div; break;
        case 0xff05: data = &cpu.tima; break;
        case 0xff06: data = &cpu.tma; break;
        case 0xff07: data = &cpu.tac; break;
        case 0xff0f: data = &cpu.if_; break;
        case 0xff10 ... 0xff26: data = &zero; break; //data = apu.readPttr(addr);
        case 0xff30 ... 0xff3f: data = &zero; break; //data = apu.readPttr(addr);
        case 0xff50: data = &bootromreg; break;
        default:
            data = &zero; // default for port mode registers
            #ifndef NO_LOGS
            std::cout << "[WARNING] " << std::hex << addr << std::dec << " PORT NOT IMPLEMENTED <--" << std::endl;
            #endif
        }
    } else if (0xff80 <= addr and addr <= 0xfffe) {
        data = &hram[addr - 0xff80];
    } else if (addr == 0xffff) {
        data = &cpu.ie;
    } else {
            #ifndef NO_LOGS
            std::cout << "[WARNING] " << std::hex << addr << std::dec << " INVALID READPTTR ADDRESS <-------" << std::endl;
            #endif
            data = &zero;
    }
    return data;
}


void Bus::cpuWrite(uint16_t addr, uint8_t data) {
    if (0x0000 <= addr and addr <= 0x7FFF) {
        cart.write(addr, data);
    } else if (0xc000 <= addr and addr <= 0xdfff) {
        wram[addr-0xc000] = data;
    } else if (0xe000 <= addr and addr <= 0xfdff) {
        wram[addr-0xe000] = data;
    } else if (0xa000 <= addr and addr <= 0xbfff) {
        cart.write(addr, data);
    } else if ((0x8000 <= addr and addr <= 0x9fff) // VRAM
            or (0xfe00 <= addr and addr <= 0xfe9f) // OAM
            or (0xff40 <= addr and addr <= 0xff4b)) { // PPU REG
        ppu.cpuWrite(addr, data);
    } else if (0xff00 <= addr and addr <= 0xff7f) {
        switch (addr) {
        case 0xff00: joypad.write(data);
        case 0xff01: sb = data; break;
        case 0xff02: sc = data; break;
        case 0xff04: cpu.div  = data; break;
        case 0xff05: cpu.tima = data; break;
        case 0xff06: cpu.tma  = data; break;
        case 0xff07: cpu.tac  = data; break;
        case 0xff0f: cpu.if_  = data; break;
        case 0xff10 ... 0xff26: break; //data = apu.readPttr(addr);
        case 0xff30 ... 0xff3f: break; //data = apu.readPttr(addr);
        case 0xff50: bootromreg = data; break;
        default:
            data = 0x00; // default for port mode registers
            #ifndef NO_LOGS
            std::cout << "[WARNING] " << std::hex << addr << std::dec << " PORT NOT IMPLEMENTED <--" << std::endl;
            #endif
        }  
    } else if (0xff80 <= addr and addr <= 0xfffe) {
        hram[addr - 0xff80] = data;
    } else if (addr == 0xffff) {
        cpu.ie = data;
    } else {
            #ifndef NO_LOGS
            std::cout << "[WARNING] " << std::hex << addr << std::dec << " INVALID WRITE ADDRESS <-------" << std::endl;
            #endif
    }
}




