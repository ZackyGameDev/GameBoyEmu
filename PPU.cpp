#include "Bus.h"
#include "PPU.h"
#include "SM83.h"
// #include <SDL2/SDL.h>
#include <fstream>
#include <iostream>

PPU::PPU() {
    for (auto &i : oam) {
        i = 0;
    } 
    for (auto &i : vram) {
        i = 02;
    }

    #ifdef LOADMEMDUMP

    std::ifstream file("ROMS/mem.dump", std::ios::binary); // open the file
    std::vector<uint8_t> fileBytes((std::istreambuf_iterator<char>(file)),
                                    std::istreambuf_iterator<char>());
    file.close(); // close the file

    for (int i = 0; i < 0x2000; i++) {
        vram[i] = fileBytes[i];
    }

    #endif 
    
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
        case 0xff40: data = lcdc; break;
        case 0xff41: data = stat; break;
        case 0xff42: data = scy; break;
        case 0xff43: data = scx; break;
        case 0xff44: data = ly; break;
        case 0xff45: data = lyc; break;
        case 0xff46: data = dma; break;
        case 0xff47: data = bgp; break;
        case 0xff48: data = obp0; break;
        case 0xff49: data = obp1; break;
        case 0xff4a: data = wy; break;
        case 0xff4b: data = wx; break;
        default: throw std::runtime_error("Invalid PPU Read");
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
            case 0xff40: data = &lcdc; break;
            case 0xff41: data = &stat; break;
            case 0xff42: data = &scy; break;
            case 0xff43: data = &scx; break;
            case 0xff44: data = &ly; break;
            case 0xff45: data = &lyc; break;
            case 0xff46: data = &dma; break;
            case 0xff47: data = &bgp; break;
            case 0xff48: data = &obp0; break;
            case 0xff49: data = &obp1; break;
            case 0xff4a: data = &wy; break;
            case 0xff4b: data = &wx; break;
            default: throw std::runtime_error("Invalid PPU PttrRead");
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

    #ifdef LOADMEMDUMP
    updateTileset();
    updateBackgroundLayer();
    #endif
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
        
        // TIME TO REWRITE THE ENTIRE RENDERER

        // uint16_t bx = scx;
        // uint16_t by = (scy+ly)*256;
        // // (bx, by) will be the coordinate of the pixel in the background vram to be drawn
        // for (uint16_t lx = 0; lx < 160; lx++) {
        //     // now (lx, ly) is the coordinate of the currently drawn LCD pixel
        //     uint8_t tile_index = bx/8 + (by/8)*32;
        //     uint8_t tile_pixel_index = (bx%8) + (by%8)*8;
        //     uint8_t tile_addressing_id = this->cpuRead(0x9800+tile_index); // this is the tile id in the tile map
        //     uint16_t tile_address; // this is the actual memory address where the tile is 
        //     page_addressing_mode = getLCDCFlag(LCDCFLAGS::BGAndWindowTileDataArea);
        //     if (page_addressing_mode == 1) {
        //         tile_address = 0x8000 + tile_addressing_id*16;
        //     } else {
        //         tile_address = 0x9000 + (int8_t)tile_addressing_id*16;
        //     }

        //     uint8_t pixel_row_index = tile_pixel_index/8;
        //     uint8_t pixel_color_lsb_row = this->cpuRead(tile_address + pixel_row_index*2);
        //     uint8_t pixel_color_hsb_row = this->cpuRead(tile_address + pixel_row_index*2 + 1);
        //     uint8_t pixel_color_id = 
        //             pixel_color_lsb_row >> (7-tile_pixel_index%8) & 1 |
        //            (pixel_color_hsb_row >> (7-tile_pixel_index%8) & 1) << 1;
            
        //     uint8_t pixel_color = 255*(1-(pixel_color_id/3));
        //     SDL_SetRenderDrawColor(renderer, pixel_color, pixel_color, pixel_color, 255);

        //     SDL_RenderDrawPoint(renderer, lx, ly);
        // }

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
            // for (int i = 0; i < 0x180; i++) {
            //     SDL_Rect tilerect;
            //     tilerect.x = (i % 16)*8;
            //     tilerect.y = (i / 16)*8;
            //     tilerect.w = 8;
            //     tilerect.h = 8;
            //     SDL_RenderCopy(renderer, tileset[i], nullptr, &tilerect);
            //     SDL_RenderPresent(renderer);
            // }
            updateTileset();
            updateBackgroundLayer();
            drawBackground();


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
            
            // Handle events
            SDL_Event event;
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    std::cout << "quit sensed. ignoring." << std::endl;
                }
            }

            std::ofstream vramwritefile("ROMS/vram.bin", std::ios::binary);
            vramwritefile.write(reinterpret_cast<char*>(vram.data()), vram.size());

            // Close the file
            vramwritefile.close();

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

SDL_Texture* PPU::getTile(uint16_t addr) {
    uint16_t vram_addr = addr - 0x8000;
    
    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_STATIC, 8, 8);
    if (!texture) {
        std::cerr << "Failed to create tile texture: " << SDL_GetError() << std::endl;
        return nullptr;
    }

    std::vector<uint8_t> rgbValues;

    for (int row = 0; row < 8; row++) {
        uint8_t lower_dot_row_data = vram[vram_addr + row*2];
        uint8_t upper_dot_row_data = vram[vram_addr + row*2 + 1];

        for (int pixel = 0; pixel < 8; pixel++) {
            uint8_t lower_dot_data = (lower_dot_row_data >> (7-pixel)) & 0x1;
            uint8_t upper_dot_data = (upper_dot_row_data >> (7-pixel)) & 0x1;
            uint8_t color_id = (upper_dot_data << 1) | lower_dot_data;

            uint8_t color = 255*(1 - (color_id/(float)3));
            
            rgbValues.push_back(color);
            rgbValues.push_back(color);
            rgbValues.push_back(color);
        }
    }

    SDL_UpdateTexture(texture, nullptr, rgbValues.data(), 8 * 3);
    return texture;
}

void PPU::updateTileset() {
    uint16_t addr;
    // looping through every tile address
    uint16_t tileindex;
    for (tileindex = 0; tileindex < 0x180; tileindex++) {
        addr = 0x8000 + (tileindex)*0x10;
        tileset[tileindex] = getTile(addr);
    }
}

void PPU::updateBackgroundLayer() {
    SDL_Texture* background_layer = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_STATIC, 256, 256);
    if (!background_layer) {
        std::cerr << "Failed to create background texture: " << SDL_GetError() << std::endl;
        return;
    }

    uint16_t bg_base_address;
    int tileset_index_offset = 0;
    bool tile_addressing_mode = (lcdc & (1 << 4));
    bool bg_location_mode = (lcdc & (1 << 3));
    if (bg_location_mode) 
        bg_base_address = 0x9c00;
    else 
        bg_base_address = 0x9800;

    if (!tile_addressing_mode) 
        tileset_index_offset = 256;
    
    uint16_t background_tile_index;
    SDL_SetRenderTarget(renderer, background_layer);
    for (background_tile_index = 0x00; background_tile_index < 0x400; background_tile_index++) {
        uint8_t tile_id = vram[bg_base_address+background_tile_index - 0x8000];
        int tile_index = tile_id;

        // for breakpoint
        // if (background_tile_index == 0x104) {
        //     std::cout << "here" << std::endl;
        // }

        if (!tile_addressing_mode && tile_index > 127)
            tile_index -= 256;
        
        SDL_Texture* tile = tileset[tile_index+tileset_index_offset];

        SDL_Rect tile_rect;
        tile_rect.x = (background_tile_index % 32) * 8;
        tile_rect.y = (background_tile_index / 32) * 8;
        tile_rect.w = 8;
        tile_rect.h = 8;
        std::cout<<tile_rect.x<<std::endl;
        SDL_RenderCopy(renderer, tile, nullptr, &tile_rect);
    }
    SDL_SetRenderTarget(renderer, nullptr);
}

void PPU::drawBackground() {
    SDL_Rect viewport;
    viewport.x = scx;
    viewport.y = scy;
    viewport.w = 160;
    viewport.h = 144;

    SDL_RenderCopy(renderer, background_layer, &viewport, nullptr);
    SDL_RenderPresent(renderer);
}