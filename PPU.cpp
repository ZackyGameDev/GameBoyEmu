#include "Bus.h"
#include "PPU.h"
// #include <SDL2/SDL.h>
#include <iostream>

PPU::PPU() {
    for (auto &i : oam) {
        i = 0;
    } 
    for (auto &i : vram) {
        i = 0;
    }
    
    std::cout << "[DEBUG] Graphics Chip Created." << std::endl;
}

PPU::~PPU() {

}


void PPU::cpuWrite(uint16_t addr, uint8_t data) {
    
    if (0x8000 <= addr and addr <= 0x9fff) {
        vram[addr-0x8000] = data;
    } else if (0xfe00 <= addr and addr <= 0xfe9f) {    
        oam[addr-0xfe00] = data;
    } else switch(addr) {
        case 0xff40: lcdc = data; break;
        case 0xff41: stat = data; break;
        case 0xff42: scy = data; break;
        case 0xff43: scx = data; break;
        // case 0xff44: ly = data; // this is read only
        case 0xff45: lyc = data; break;
        case 0xff46: dma = data; break;
        case 0xff47: bgp = data; break;
        case 0xff48: obp0 = data; break;
        case 0xff49: obp1 = data; break;
        case 0xff4a: wy = data; break;
        case 0xff4b: wx = data; break;
        default: throw std::runtime_error("Invalid PPU Write");
    }
    
}

uint8_t PPU::cpuRead(uint16_t addr) {
    uint8_t data = 0;

    if (0x8000 <= addr and addr <= 0x9fff) {
        data = vram[addr-0x8000];
    } else if (0xfe00 <= addr and addr <= 0xfe9f) {    
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

    if (0x8000 <= addr and addr <= 0x9fff) {
        data = &vram[addr-0x8000];
    } else if (0xfe00 <= addr and addr <= 0xfe9f) {    
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


void PPU::initLCD() {

    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(160*4, 144*4, 0, &window, &renderer);
    SDL_RenderSetScale(renderer, 4, 4);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawPoint(renderer, 150, 140);

    SDL_RenderPresent(renderer);
}

void PPU::clock() {

    if (cycles <= 0) switch (mode) {
    case PPUMODE::OAMREAD:

        // next mode...
        mode = PPUMODE::PIXELTRANSFER;
        cycles = 43*4; // the *4 is to convert from machine cycles to clock cycles

        // JUST ENTERED PIXEL TRANSFER MODE
        std::cout << "[DEBUG] PIXELTRANSFER" << std::endl;


    break;
    
    case PPUMODE::PIXELTRANSFER:

        // at the end of PIXEL TRANSFER i'll draw
        // an entire scan line.
        
        // BACKGROUND IN VRAM IS STORED IN  9800h to 9BFFh
        // WINDOW 9C00h to 9FFFh
        uint8_t  scanline_start_index; 
        uint16_t scanline_start_address;
        for (int hpos = 0; hpos < 256; hpos++) {
            scanline_start_index = scx + (scy + ly)*256 + hpos;
            if (scanline_start_index > 255) scanline_start_index -= 256;
            scanline_start_address = 0x8000 + scanline_start_index;
        }

        uint8_t tile_index = this->cpuRead(scanline_start_address);

        uint16_t tile_address;
        page_addressing_mode = getLCDCFlag(LCDCFLAGS::BGAndWindowTileDataArea);
        if (page_addressing_mode == 1) {
            tile_address = 0x8000 + tile_index;
        } else {
            tile_address = 0x9000 + (int8_t)tile_index;
        }

        


        // next mode...
        mode = PPUMODE::HBLANK;
        cycles = 51*4;

        // JUST ENTERED HBLANK MODE
        std::cout << "[DEBUG] HBLANK" << std::endl;
    break;

    case PPUMODE::HBLANK:

        // next mode...
        if (ly < 143) {
            mode = PPUMODE::OAMREAD;
            cycles = 20*4;

            // JUST ENTERED OAM READ MODE FROM HBLANK
            std::cout << "[DEBUG] OAMREAD" << std::endl;

        } else {
            mode = PPUMODE::VBLANK;
            cycles = 114*4;

            // JUST ENTERED VBLANK MODE
            std::cout << "[DEBUG] VBLANK" << std::endl;
    
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);

            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderDrawPoint(renderer, debug_x, debug_y);
            debug_x++; debug_y++;
        }
        ly++;
    break;

    case PPUMODE::VBLANK:
        SDL_RenderPresent(renderer);

        // next mode
        if (ly >= 153) {
            mode = PPUMODE::OAMREAD;
            cycles = 20;
            ly = 0;

            // JUST ENTERED OAM READ MODE FROM VBLANK
            std::cout << "[DEBUG] OAMREAD" << std::endl;
        } else {
            cycles = 114*4;
            ly++;
        }
    break;
    }

    cycles--;
    // setting the STAT register flags.
    stat &= 0xf8;
    stat |= ly==lyc << 2;
    stat |= mode;
}
