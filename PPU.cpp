#include "Bus.h"
#include "PPU.h"

PPU::PPU() {
    oam.resize(160, 0x00);

}

PPU::~PPU() {
    oam.clear();
}


void PPU::cpuWrite(uint16_t addr, uint8_t data) {
    
    if (0xfe00 <= addr and addr <= 0xfe9f) {    
        oam[addr-0xfe00] = data;
    } else switch(addr) {
        case 0xff40: lcdc = data;
        case 0xff41: stat = data;
        case 0xff42: scy = data;
        case 0xff43: scx = data;
        case 0xff44: ly = data;
        case 0xff45: lyc = data;
        case 0xff46: dma = data;
        case 0xff47: bgp = data;
        case 0xff48: obp0 = data;
        case 0xff49: obp1 = data;
        case 0xff4a: wy = data;
        case 0xff4b: wx = data;
    }
    
}

uint8_t PPU::cpuRead(uint16_t addr) {
    uint8_t data = 0;

    if (0xfe00 <= addr and addr <= 0xfe9f) {    
        data = oam[addr-0xfe00];
    } else switch (addr) {
        case 0xff40: data = lcdc;
        case 0xff41: data = stat;
        case 0xff42: data = scy;
        case 0xff43: data = scx;
        case 0xff44: data = ly;
        case 0xff45: data = lyc;
        case 0xff46: data = dma;
        case 0xff47: data = bgp;
        case 0xff48: data = obp0;
        case 0xff49: data = obp1;
        case 0xff4a: data = wy;
        case 0xff4b: data = wx;
    }

    return data;

}

uint8_t* PPU::cpuReadPttr(uint16_t addr) {
    uint8_t* data = nullptr;

    if (0xfe00 <= addr and addr <= 0xfe9f) {    
        data = &oam[addr-0xfe00];
    } else {
        switch (addr) {
            case 0xff40: data = &lcdc;
            case 0xff41: data = &stat;
            case 0xff42: data = &scy;
            case 0xff43: data = &scx;
            case 0xff44: data = &ly;
            case 0xff45: data = &lyc;
            case 0xff46: data = &dma;
            case 0xff47: data = &bgp;
            case 0xff48: data = &obp0;
            case 0xff49: data = &obp1;
            case 0xff4a: data = &wy;
            case 0xff4b: data = &wx;
        }
    }
    
    return data;
}