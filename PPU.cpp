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
    // i comment this part out sometimes to 
    // see what would happen (curiosity)
    for (auto &i : vram) {
        i = 0b00000000;
    }

    #ifdef LOADMEMDUMP

    std::ifstream file(MEM_DUMP_PATH, std::ios::binary); // open the file
    std::vector<uint8_t> fileBytes((std::istreambuf_iterator<char>(file)),
                                    std::istreambuf_iterator<char>());
    file.close(); // close the file

    for (int i = 0; i < 0x2000; i++) {
        vram[i] = fileBytes[i];
    }

    #endif 

    last_pttr_addr = 0x8000;
    last_pttr_value = 0x00;
    
    std::cout << "[DEBUG] Graphics Chip Created." << std::endl;
}

PPU::~PPU() {

}

void PPU::initLCD() {
    
    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(160*4, 144*4, 0, &window, &renderer);
    SDL_RenderSetScale(renderer, 4, 4);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderDrawPoint(renderer, 150, 140);

    SDL_RenderPresent(renderer);

    tilemap0_layer = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, 256, 256);
    tilemap1_layer = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, 256, 256);
    obj_layer = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 256, 256);
    debug_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 256, 256);
    tileset = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, 8*16, 8*24);
    
    SDL_SetTextureBlendMode(obj_layer, SDL_BLENDMODE_BLEND);

    createTileset(tileset);
    createTilemapLayer(tilemap0_layer, 0x9800);
    createTilemapLayer(tilemap1_layer, 0x9C00);
    
}

void PPU::clock() {

    handleSTAT();

    if (cycles <= 0) 
    switch (mode) {
    case PPUMODE::OAMREAD:

        // next mode...
        mode = PPUMODE::PIXELTRANSFER;
        cycles = 43; // in machine cycles

    break;
    
    case PPUMODE::PIXELTRANSFER: {

        // finally, scanline emulation.
        // ly should have the current scanline number.

        // drawScanLine();
        SDL_Rect viewport = {0, 0, 256, 256};
        SDL_Rect display = {0, 0, 160, 144};
        drawScanLine();
        // SDL_RenderCopy(renderer, tilemap0_layer, &viewport, &display);
        // SDL_RenderPresent(renderer);

        if (vram_accessed) {
            createTileset(tileset);
            createTilemapLayer(tilemap0_layer, 0x9800);
            createTilemapLayer(tilemap1_layer, 0x9c00);
            vram_accessed = false;
        }
        
        // next mode...
        mode = PPUMODE::HBLANK;
        cycles = 51;
    }
    break;

    case PPUMODE::HBLANK:

        // next mode...
        if (ly < 143) {
            mode = PPUMODE::OAMREAD;
            cycles = 20;
        } else {
            mode = PPUMODE::VBLANK;
            cycles = 114;
            // Handle DMA transfer
            // if (dma != dma_prev) {
            if (dma_written) {
                dma_written = false;
                uint16_t dma_addr = dma << 8;
                for (int i = 0; i < 0xa0; i++) {
                    oam[i] = this->bus->cpuRead(dma_addr+i);
                }
                dma_prev = dma;
            }

            renderObjLayer();
            SDL_RenderPresent(renderer);
            VBlankInterrupt();
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
                    std::cout << "quit sensed. attempting shutdown..." << std::endl;
                    bus->shutdown();
                }
            }

            #ifdef DUMP_VRAM
            std::ofstream vramwritefile("ROMS/vram.bin", std::ios::binary);
            vramwritefile.write(reinterpret_cast<char*>(vram.data()), vram.size());

            // Close the file
            vramwritefile.close();
            #endif

            mode = PPUMODE::OAMREAD;
            cycles = 20;
            ly = 0;

        } else {
            cycles = 114;
            ++ly;
        }
    break;
    }

    cycles--;
}

void PPU::VBlankInterrupt() {
    this->bus->cpu.requestInterrupt(SM83::InterruptFlags::VBlank);
}

void PPU::handleSTAT() {

    bool old_stat_interrupt_line_value = stat_interrupt_line_value;

    stat_interrupt_line_value = 0;

    // mode changed in this clock cycle and its not changed to OAM
    if ((stat_readonly_part & 0x3) != mode && mode != 3)
    if ((stat >> (((uint8_t)mode)+3)) & 0x01) {
        stat_interrupt_line_value = 1;
    }

    // if condition is met, if interrupt is enabled, and if the condition was previously false (i.e. it just became true)    
    if (lyc==ly && ((stat >> 6) & 0x01) && !((stat_readonly_part >> 2) & 1)) {
        stat_interrupt_line_value = 1;
    }
    
    stat_readonly_part = ((lyc==ly) << 2) | ((uint8_t)mode);
    stat &= 0xf8;
    stat |= stat_readonly_part;

    if (stat_interrupt_line_value &&  !old_stat_interrupt_line_value ) {
        this->bus->cpu.requestInterrupt(SM83::InterruptFlags::Stat);
    }

}


void PPU::createTileset(SDL_Texture* tileset) {

    for (int tile_index = 0; tile_index < 0x180; tile_index++) {
        uint32_t tile_pixels[8*8];

        for (int row = 0; row < 8; row++) {
            uint8_t lower_row = vram[tile_index*16 + row*2];
            uint8_t upper_row = vram[tile_index*16 + row*2 + 1];

            for (int pixel = 0; pixel < 8; pixel++) {
                uint8_t lower_bit = (lower_row >> (7 - pixel)) & 1;
                uint8_t upper_bit = (upper_row >> (7 - pixel)) & 1;
                uint8_t color_id = (upper_bit << 1) | lower_bit;
                tile_pixels[row * 8 + pixel] = color_palette[color_id];
            }

        }

        // now slapping that tile data onto the tileset texture
        int tiles_per_row = 16;
        int tile_x = (tile_index % tiles_per_row) * 8;
        int tile_y = (tile_index / tiles_per_row) * 8;

        SDL_Rect rect = {tile_x, tile_y, 8, 8};

        // apparently with STREAM access textures we need to lock them first before modifying
        void *pixels;
        int pitch;

        SDL_LockTexture(tileset, &rect, &pixels, &pitch);
        
        uint8_t *dst = (uint8_t *)pixels;

        for (int y = 0; y < 8; y++) 
            memcpy(dst + y * pitch, &tile_pixels[y * 8], 8 * sizeof(uint32_t));

        SDL_UnlockTexture(tileset);
    }
    
}

void PPU::createTilemapLayer(SDL_Texture* layer, uint16_t tilemap_addr) {

    bool addressing_mode = (lcdc >> 4) & 1;
    SDL_Rect src_rect = {0, 0, 8, 8};
    SDL_Rect dst_rect = {0, 0, 8, 8};
    for (int tile_i = 0; tile_i < 32*32; tile_i++) {

        uint8_t tileid = vram[tilemap_addr - 0x8000 + tile_i];
        uint16_t tile_index_in_tileset;
        // std::cout << addressing_mode << std::endl;
        if (addressing_mode) {
            tile_index_in_tileset = tileid;
        } else {
            tile_index_in_tileset = 256 + toSigned(tileid);
        }

        src_rect.x = (tile_index_in_tileset % 16) * 8;
        src_rect.y = (tile_index_in_tileset / 16) * 8;
        dst_rect.x = (tile_i % 32) * 8;
        dst_rect.y = (tile_i / 32) * 8;

        // lock the source
        void *src_pixels;
        int src_pitch;
        SDL_LockTexture(tileset, &src_rect, &src_pixels, &src_pitch);

        // lock destination layer
        void *dst_pixels;
        int dst_pitch;
        SDL_LockTexture(layer, &dst_rect, &dst_pixels, &dst_pitch);
        
        // copy tile to layer
        for (int y = 0; y < 8; y++) {
            uint8_t *src_row = (uint8_t *)src_pixels + y * src_pitch;
            uint8_t *dst_row = (uint8_t *)dst_pixels + y * dst_pitch;
            memcpy(dst_row, src_row, 8 * 4);
        }

        // Unlock both textures
        SDL_UnlockTexture(tileset);
        SDL_UnlockTexture(layer);

    }

}

/*
  All this function does is render a scanline
  to the display window (wihtout renderpresent)
  which scanline is determined through ly register
*/
void PPU::drawScanLine() {
    
    // std::cout << "begining draw LY=" << (int)ly << '\n';
    SDL_Texture** background_layer;
    SDL_Texture** window_layer;

    if (getLCDCFlag(LCDCFLAGS::BGTileMapArea))
        background_layer = &tilemap1_layer;
    else 
        background_layer = &tilemap0_layer;

    if (getLCDCFlag(LCDCFLAGS::WindowTileMapArea))
        window_layer = &tilemap1_layer;
    else 
        window_layer = &tilemap0_layer;

    bool test = getLCDCFlag(LCDCFLAGS::BGAndWindowTileDataArea);
    
    // background first
    int row = (scy + ly) % 256;

    if ((int)scx + 160 > 256) { // wrap around condition
        // std::cout<< "warp around\n";
        int w = 256 - (int)scx;
        SDL_Rect src_row = {scx, row, w, 1};
        SDL_Rect dst_row = {0, ly, w, 1};
        
        SDL_RenderCopy(renderer, *background_layer, &src_row, &dst_row);
        
        dst_row.x = w;
        dst_row.w = 160 - dst_row.w;
        src_row.x = 0;
        src_row.w = dst_row.w;
        
        SDL_RenderCopy(renderer, *background_layer, &src_row, &dst_row);
    } else {
        // std::cout<< "NOT\n";
        SDL_Rect src_row = {scx, row, 160, 1};
        SDL_Rect dst_row = {0, ly, 160, 1};
        SDL_RenderCopy(renderer, *background_layer, &src_row, &dst_row);
    }

    
    // SDL_SetRenderTarget(renderer, debug_texture);
    // SDL_Rect viewport = {0, 0, 256, 256};
    // SDL_Rect display = {0, 0, 160, 144};
    // SDL_RenderCopy(renderer, *background_layer, &viewport, &viewport);
    // SDL_Rect viewport2 = {scx, scy, 160, 144};
    // SDL_SetRenderDrawColor(renderer, 0xff, 0, 0, 0xff);
    // SDL_RenderDrawRect(renderer, &viewport2);
    // SDL_RenderDrawPoint(renderer, scx, scy+ly);
    // SDL_RenderDrawPoint(renderer, scx+1, scy+ly);
    // SDL_RenderDrawPoint(renderer, scx+2, scy+ly);
    // SDL_RenderDrawPoint(renderer, scx+3, scy+ly);
    // SDL_SetRenderTarget(renderer, NULL);
    // SDL_RenderCopy(renderer, debug_texture, &viewport, &display);
    // SDL_RenderPresent(renderer);
    // std::cout << "finish draw LY=" << (int)ly << '\n';
    // return;

    // now window layer
    if ((getLCDCFlag(LCDCFLAGS::WindowEnable) && ly >= wy) && !(wx < 7 || wx > 166 || wy > 143)) {
        if (ly == wy) {
            window_line_counter = 0;
        } else {
            window_line_counter++;
        }
        row = window_line_counter;

        if (row < 256) {
            SDL_Rect src_row = {0, row, 160 - (wx - 7), 1};
            SDL_Rect dst_row = {wx - 7, ly, 160 - (wx - 7), 1};
            SDL_RenderCopy(renderer, *window_layer, &src_row, &dst_row);
        } 
    }
    


    // not doing scanline basis object drawing right now
    // // now for sprites
    // int sprite_height = getLCDCFlag(PPU::LCDCFLAGS::OBJSize) ? 16 : 8;
    // int drawn = 0;
    // for (uint8_t i = 0; i < 40; i++) {
    //     short j = i*4;
        
    //     if (ly >= oam[j]-16 && ly < oam[j]-16 + sprite_height) {
    //         drawObjectScanLine(oam[j], oam[j+1], oam[j+2], oam[j+3]);
    //         drawn++;
    //     }
    //     if (drawn == 10) break;
    // }

    // lazier approach (more efficient for effort)
    int drawn = 0;
    for (int i = 0; i < 40; i++) {
        int j = i*4;
        if ((oam[j]-16) == ly) {
            drawObjectToTexture(oam[j], oam[j+1], oam[j+2], oam[j+3]);
            drawn++;
            if (drawn == 10) {
                break;
            }
        } 
    }

}

void PPU::renderObjLayer() {
    SDL_Rect src_rect = {8, 16, 160, 144};
    SDL_Rect dst_rect = {0, 0, 160, 144};

    SDL_RenderCopy(renderer, obj_layer, &src_rect, &dst_rect);

    SDL_SetRenderTarget(renderer, obj_layer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0); // transparent
    SDL_RenderClear(renderer); // clear the layer
    SDL_SetRenderTarget(renderer, NULL);

}

void PPU::drawObjectToTexture(uint8_t y, uint8_t x, uint8_t tile_index, uint8_t attributes) {

    SDL_SetRenderTarget(renderer, obj_layer);

    if (getLCDCFlag(LCDCFLAGS::OBJSize)) {

        SDL_SetRenderTarget(renderer, debug_texture);

        // drawing upper tile
        tile_index -= tile_index % 2; // ignoring odd addresses or something.
        uint16_t tile_x = (tile_index % 16) * 8;
        uint16_t tile_y = (tile_index / 16) * 8;
    
        SDL_Rect src_rect = {tile_x, tile_y, 8, 8};
        SDL_Rect dst_rect = {0, 0, 8, 8};
        SDL_RenderCopy(renderer, tileset, &src_rect, &dst_rect);
        
        // drawing lower tile
        tile_index++;
        uint16_t tile_x2 = (tile_index % 16) * 8;
        uint16_t tile_y2 = (tile_index / 16) * 8;
    
        SDL_Rect src_rect2 = {tile_x2, tile_y2, 8, 8};
        SDL_Rect dst_rect2 = {0, 8, 8, 8};
        SDL_RenderCopy(renderer, tileset, &src_rect2, &dst_rect2);

        // now pasting them with correct flip to display buffer
        SDL_SetRenderTarget(renderer, obj_layer);
        SDL_Rect src_rect3 = {0, 0, 8, 16};
        SDL_Rect dst_rect3 = {x, y, 8, 16};
        
        SDL_RendererFlip flip = SDL_FLIP_NONE;
        if (attributes & 0x20) {
            flip = (SDL_RendererFlip)(flip | SDL_FLIP_HORIZONTAL);
        }
        if (attributes & 0x40) {
            flip = (SDL_RendererFlip)(flip | SDL_FLIP_VERTICAL);
        }
        
        // Render the texture with the appropriate flip state
        SDL_RenderCopyEx(renderer, debug_texture, &src_rect3, &dst_rect3, 0, nullptr, flip);

    } else {
        uint16_t tile_x = (tile_index % 16) * 8;
        uint16_t tile_y = (tile_index / 16) * 8;
    
        SDL_Rect src_rect = {tile_x, tile_y, 8, 8};
        SDL_Rect dst_rect = {x, y, 8, 8};
        
        // Determine the flip state based on sprite_flags
        SDL_RendererFlip flip = SDL_FLIP_NONE;
        if (attributes & 0x20) {
            flip = (SDL_RendererFlip)(flip | SDL_FLIP_HORIZONTAL);
        }
        if (attributes & 0x40) {
            flip = (SDL_RendererFlip)(flip | SDL_FLIP_VERTICAL);
        }
        
        // Render the texture with the appropriate flip state
        SDL_RenderCopyEx(renderer, tileset, &src_rect, &dst_rect, 0, nullptr, flip);
    }

    SDL_SetRenderTarget(renderer, NULL);
}


/*
    This function straight up draws the scanline. it does NOT check if the object
    follows the 10 object limit, or if ly is currently on the objects visible area.
    it. just. draws.
*/
void PPU::drawObjectScanLine(uint8_t y, uint8_t x, uint8_t tile_index, uint8_t attributes) {
    
    uint8_t row;
    row = ly - (y-16);
    
    // basically if row to draw is beyond the object area
    if (row > 15) return;
    if (row > 7 && !getLCDCFlag(PPU::LCDCFLAGS::OBJSize)) return;

    if (row > 7) row -= 8; tile_index++;

    uint16_t tile_x = (tile_index % 16) * 8;
    uint16_t tile_y = (tile_index / 16) * 8;

    SDL_Rect src_row = {tile_x, tile_y+row, 8, 1};
    SDL_Rect dst_row = {x-8, ly, 8, 1};
    SDL_RenderCopy(renderer, tileset, &src_row, &dst_row);

}


void PPU::drawObjects() {
    // drawing sprites.
    uint8_t oc = 0;
    // if (oam[0] != 0x90)
        // std::cout << "breakpoint";
    for (int i = 0; i < 40; i++) {
        uint8_t sprite_y = oam[oc++] - 16;
        uint8_t sprite_x = oam[oc++] - 8;
        uint8_t sprite_tile_index = oam[oc++];
        uint8_t sprite_flags = oam[oc++];

        uint16_t tile_x = (sprite_tile_index % 16) * 8;
        uint16_t tile_y = (sprite_tile_index / 16) * 8;

        SDL_Rect src_rect = {tile_x, tile_y, 8, 8};
        SDL_Rect dst_rect = {sprite_x, sprite_y, 8, 8};
        
        // Determine the flip state based on sprite_flags
        SDL_RendererFlip flip = SDL_FLIP_NONE;
        if (sprite_flags & 0x20) {
            flip = (SDL_RendererFlip)(flip | SDL_FLIP_HORIZONTAL);
        }
        if (sprite_flags & 0x40) {
            flip = (SDL_RendererFlip)(flip | SDL_FLIP_VERTICAL);
        }
        
        // Render the texture with the appropriate flip state
        SDL_RenderCopyEx(renderer, tileset, &src_rect, &dst_rect, 0, nullptr, flip);
    }
}




///////////////////// THESE ARE JUST PLACEHOLDERS TO MAKE IT COMPILE
void PPU::cpuWrite(uint16_t addr, uint8_t data) {
    
    if (0x8000 <= addr and addr <= 0x9fff) {
        // if (mode != PPUMODE::PIXELTRANSFER) {
            vram[addr-0x8000] = data;
            vram_accessed = true;
        // }
    } else if (0xfe00 <= addr and addr <= 0xfe9f) {    
        // if (mode != PPUMODE::OAMREAD and mode != PPUMODE::PIXELTRANSFER) {
            oam[addr-0xfe00] = data;
            oam_accessed = true;
        // }
    } else switch(addr) {
        case 0xff40: lcdc = data; break;
        case 0xff41: stat = data; break;
        case 0xff42: scy = data; break;
        case 0xff43: scx = data; break;
        case 0xff44: break; //ly = data; // this is read only
        case 0xff45: lyc = data; break;
        case 0xff46: dma = data; dma_written = true; break;
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
        // if (mode != PPUMODE::PIXELTRANSFER) {
            data = vram[addr-0x8000];
        // } else {
        //     data = garbage_byte;
        // }
    } else if (0xfe00 <= addr and addr <= 0xfe9f) {    
        // if (mode != PPUMODE::OAMREAD and mode != PPUMODE::PIXELTRANSFER) {
            data = oam[addr-0xfe00];
        // } else {
            // data = garbage_byte;
        // }
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

    if (0x8000 <= addr && addr <= 0x9fff) {
        // if (mode != PPUMODE::PIXELTRANSFER) {
            data = &vram[addr-0x8000];
            last_pttr_addr = addr;
            last_pttr_value = *data;
            vram_accessed = true;
        // } else {
            // data = &garbage_byte;
        // }
    } else if (0xfe00 <= addr && addr <= 0xfe9f) {
        // if (mode != PPUMODE::OAMREAD && mode != PPUMODE::PIXELTRANSFER) {
            data = &oam[addr-0xfe00];
            last_pttr_addr = addr;
            last_pttr_value = *data;
            oam_accessed = true;
        // } else {
        //     data = &garbage_byte;
        // }
    } else {
        switch (addr) {
            case 0xff40: data = &lcdc; break;
            case 0xff41: data = &stat; break;
            case 0xff42: data = &scy; break;
            case 0xff43: data = &scx; break;
            case 0xff44: data = &ly; break;
            case 0xff45: data = &lyc; break;
            case 0xff46: data = &dma; dma_written = true; break;
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
