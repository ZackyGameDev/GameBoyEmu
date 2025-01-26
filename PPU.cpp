#include "Bus.h"
#include "PPU.h"
#include "SM83.h"
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
        cycles = 43; // in machine cycles

        // JUST ENTERED PIXEL TRANSFER MODE
        // std::cout << "[DEBUG] PIXELTRANSFER" << std::endl;


    break;
    
    case PPUMODE::PIXELTRANSFER: {

        // at the end of PIXEL TRANSFER i'll draw
        // an entire scan line.
        
        // BACKGROUND IN VRAM IS STORED IN  9800h to 9BFFh
        // WINDOW 9C00h to 9FFFh
        
        uint16_t bx = scx;
        uint16_t by = (scy+ly)*256;
        // (bx, by) will be the coordinate of the pixel in the background vram to be drawn
        for (uint16_t lx = 0; lx < 160; lx++) {
            // now (lx, ly) is the coordinate of the currently drawn LCD pixel
            uint8_t tile_index = bx/8 + (by/8)*32;
            uint8_t tile_pixel_index = (bx%8) + (by%8)*8;
            uint8_t tile_addressing_id = this->cpuRead(0x9800+tile_index); // this is the tile id in the tile map
            uint16_t tile_address; // this is the actual memory address where the tile is 
            page_addressing_mode = getLCDCFlag(LCDCFLAGS::BGAndWindowTileDataArea);
            if (page_addressing_mode == 1) {
                tile_address = 0x8000 + tile_addressing_id*16;
            } else {
                tile_address = 0x9000 + (int8_t)tile_addressing_id*16;
            }

            uint8_t pixel_row_index = tile_pixel_index/8;
            uint8_t pixel_color_lsb_row = this->cpuRead(tile_address + pixel_row_index*2);
            uint8_t pixel_color_hsb_row = this->cpuRead(tile_address + pixel_row_index*2 + 1);
            uint8_t pixel_color_id = 
                    pixel_color_lsb_row >> (7-tile_pixel_index%8) & 1 |
                   (pixel_color_hsb_row >> (7-tile_pixel_index%8) & 1) << 1;
            
            uint8_t pixel_color = 255*(1-(pixel_color_id/3));
            SDL_SetRenderDrawColor(renderer, pixel_color, pixel_color, pixel_color, 255);

            SDL_RenderDrawPoint(renderer, lx, ly);
        }

        // SDL_RenderPresent(renderer);


        // next mode...
        mode = PPUMODE::HBLANK;
        cycles = 51;

        // JUST ENTERED HBLANK MODE
        // std::cout << "[DEBUG] HBLANK" << std::endl;

        if (ly == lyc) {
            stat |= 1 << 2;
            if (stat & 1 << 6) {
                LYCInterrupt();
            }
        }
    }
    break;

    case PPUMODE::HBLANK:

        // next mode...
        if (ly < 143) {
            mode = PPUMODE::OAMREAD;
            cycles = 20;

            // JUST ENTERED OAM READ MODE FROM HBLANK
            // std::cout << "[DEBUG] OAMREAD" << std::endl;

        } else {
            mode = PPUMODE::VBLANK;
            cycles = 114;

            // JUST ENTERED VBLANK MODE
            // std::cout << "[DEBUG] VBLANK" << std::endl;

            VBlankInterrupt();
            // SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            // SDL_RenderDrawPoint(renderer, debug_x, debug_y);
            // debug_x++; debug_y++;
        }
        ly++;
    break;

    case PPUMODE::VBLANK:
        // printf("Exited VBLANK\n");

        // next mode
        if (ly >= 153) {
            SDL_RenderPresent(renderer);
            mode = PPUMODE::OAMREAD;
            cycles = 20;
            ly = 0;

            // JUST ENTERED OAM READ MODE FROM VBLANK
            // std::cout << "[DEBUG] OAMREAD" << std::endl;
        } else {
            cycles = 114;
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

void PPU::VBlankInterrupt() {
    this->bus->cpu.requestInterrupt(SM83::InterruptFlags::VBlank);
}

void PPU::LYCInterrupt() {
    this->bus->cpu.requestInterrupt(SM83::InterruptFlags::Stat);
}