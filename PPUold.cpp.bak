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


void PPU::initLCD() {
    
    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(160*4, 144*4, 0, &window, &renderer);
    SDL_RenderSetScale(renderer, 4, 4);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawPoint(renderer, 150, 140);

    SDL_RenderPresent(renderer);

    background_layer = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_TARGET, 256, 256);
    window_layer = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_TARGET, 256, 256);

    if (!background_layer) {
        std::cerr << "Failed to create background texture: " << SDL_GetError() << std::endl;
        return;
    }
    
    createTileset();

    #ifdef LOADMEMDUMP
    updateTileset();
    updateBackgroundLayer();
    drawBackground();
    #endif

    #ifdef TILESET_WINDOW
    SDL_CreateWindowAndRenderer(16*8, 24*8, 0, &tileset_window, &tileset_renderer);
    SDL_RenderSetScale(tileset_renderer, 1, 1);

    SDL_SetRenderDrawColor(tileset_renderer, 0, 0, 0, 255);
    SDL_RenderClear(tileset_renderer);

    drawTilesetWindow();
    SDL_RenderPresent(tileset_renderer);
    #endif

    
    #ifdef BG_WINDOW
    SDL_CreateWindowAndRenderer(32*8, 32*8, 0, &bg_window, &bg_renderer);
    SDL_RenderSetScale(bg_renderer, 1, 1);

    SDL_SetRenderDrawColor(bg_renderer, 0, 0, 0, 255);
    SDL_RenderClear(bg_renderer);

    drawBGWindow();
    SDL_RenderPresent(bg_renderer);
    #endif
}

void PPU::clock() {
    // updateTextures();
    handleSTAT();

    if (cycles <= 0) 
    switch (mode) {
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

        // if (ly == lyc) {
        //     stat |= 1 << 2;
        //     if (stat & 1 << 6) {
        //         LYCInterrupt();
        //     }
        // }
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
            if (framecount <= 0) {
                if (vram_accessed or oam_accessed) {
                    updateTileset();
                    updateBackgroundLayer();
                    updateWindowLayer();
                    vram_accessed = false;
                    oam_accessed = false;
                }
                drawBackground();
                
                #ifdef TILESET_WINDOW
                drawTilesetWindow();
                #endif
                #ifdef BG_WINDOW
                drawBGWindow();
                #endif
                
                #ifndef HIDE_SPRITES
                // drawing sprites.
                uint8_t oc = 0;
                // if (oam[0] != 0x90)
                    // std::cout << "breakpoint";
                for (int i = 0; i < 40; i++) {
                    uint8_t sprite_y = oam[oc++] - 16;
                    uint8_t sprite_x = oam[oc++] - 8;
                    uint8_t sprite_tile_index = oam[oc++];
                    uint8_t sprite_flags = oam[oc++];
            
                    SDL_Texture* tile = tileset[sprite_tile_index];
            
                    SDL_Rect tile_rect;
                    tile_rect.x = sprite_x;
                    tile_rect.y = sprite_y;
                    tile_rect.w = 8;   // Set the width of the tile
                    tile_rect.h = 8;   // Set the height of the tile
                    
                    // Determine the flip state based on sprite_flags
                    SDL_RendererFlip flip = SDL_FLIP_NONE;
                    if (sprite_flags & 0x20) {
                        flip = (SDL_RendererFlip)(flip | SDL_FLIP_HORIZONTAL);
                    }
                    if (sprite_flags & 0x40) {
                        flip = (SDL_RendererFlip)(flip | SDL_FLIP_VERTICAL);
                    }
                    
                    // Render the texture with the appropriate flip state
                    SDL_RenderCopyEx(renderer, tile, nullptr, &tile_rect, 0, nullptr, flip);
                }
                #endif
                
                drawWindow();
            }

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

            // SDL_RenderPresent(renderer);
            
            if (framecount <= 0) {
                SDL_RenderPresent(renderer);
                framecount = frameskip;
            } else {
                framecount--;
            }

            mode = PPUMODE::OAMREAD;
            cycles = 20;
            ly = 0;

            // JUST ENTERED OAM READ MODE FROM VBLANK
            // std::cout << "[DEBUG] OAMREAD" << std::endl;
        } else {
            cycles = 114;
            ++ly;
        }
    break;
    }

    cycles--;

    // -- THIS IS DONE BY handleSTAT() now.
    // setting the STAT register flags.
    // stat &= 0xf8;
    // stat |= (ly==lyc) << 2;
    // stat |= mode;
}

void PPU::VBlankInterrupt() {
    this->bus->cpu.requestInterrupt(SM83::InterruptFlags::VBlank);
}

void PPU::handleSTAT() {
    // mode changed in this clock cycle
    if ((stat_readonly_part & 0x3) != mode)
    if ((stat >> (mode+3)) & 0x01) {
        this->bus->cpu.requestInterrupt(SM83::InterruptFlags::Stat);
    }

    stat_readonly_part = ((lyc==ly) << 2) | (mode);

    if (lyc==ly && ((stat >> 6) & 0x01)) {
        this->bus->cpu.requestInterrupt(SM83::InterruptFlags::Stat);
    }

    stat &= 0xf8;
    stat |= stat_readonly_part;

}

void PPU::getTile(uint16_t addr, SDL_Texture* &texture) {
    uint16_t vram_addr = addr - 0x8000;
    
    // SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_STATIC, 8, 8);
    // if (!texture) {
    //     std::cerr << "Failed to create tile texture: " << SDL_GetError() << std::endl;
    //     return nullptr;
    // }
    
    std::vector<uint8_t> rgbValues;
    
    for (int row = 0; row < 8; row++) {
        uint8_t lower_dot_row_data = vram[vram_addr + row*2];
        uint8_t upper_dot_row_data = vram[vram_addr + row*2 + 1];
        
        for (int pixel = 0; pixel < 8; pixel++) {
            uint8_t lower_dot_data = (lower_dot_row_data >> (7-pixel)) & 0x1;
            uint8_t upper_dot_data = (upper_dot_row_data >> (7-pixel)) & 0x1;
            uint8_t color_id = (upper_dot_data << 1) | lower_dot_data;
            
            uint8_t r;
            uint8_t g;
            uint8_t b;

            r = (color_palette[color_id] >> 16) & 0xff;
            g = (color_palette[color_id] >> 8)  & 0xff;
            b = (color_palette[color_id])       & 0xff;
            
            rgbValues.push_back(r);
            rgbValues.push_back(g);
            rgbValues.push_back(b);
        }
    }
    
    SDL_UpdateTexture(texture, nullptr, rgbValues.data(), 8 * 3);
    // return texture;
}

void PPU::createTileset() {
    for (int tileindex = 0; tileindex < 0x180; tileindex++){
        SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_STATIC, 8, 8);
        if (!texture) {
            std::cerr << "Failed to create tile texture: " << SDL_GetError() << std::endl;
        }
        tileset[tileindex] = texture;
    }
}

void PPU::updateTileset() {
    uint16_t addr;
    // looping through every tile address
    uint16_t tileindex;
    for (tileindex = 0; tileindex < 0x180; tileindex++) {
        addr = 0x8000 + (tileindex)*0x10;
        getTile(addr, tileset[tileindex]);
    }
}

void PPU::updateBackgroundLayer() {

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
        // std::cout<<tile_rect.x<<std::endl;
        SDL_RenderCopy(renderer, tile, nullptr, &tile_rect);
    }
    SDL_SetRenderTarget(renderer, nullptr);
}

void PPU::drawBackground() {
    #ifndef FULL_VIEWPORT
    SDL_Rect viewport = {scx, scy, 160, 144};
    SDL_Rect display = {0, 0, 160, 144};
    SDL_RenderCopy(renderer, background_layer, &viewport, &display);
    #endif
    #ifdef FULL_VIEWPORT
    SDL_RenderCopy(renderer, background_layer, nullptr, nullptr);
    #endif
    // SDL_RenderPresent(renderer);
}


void PPU::updateWindowLayer() {

    uint16_t win_base_address;
    int tileset_index_offset = 0;
    bool tile_addressing_mode = (lcdc & (1 << 4));
    bool win_location_mode = (lcdc & (1 << 6));
    if (win_location_mode) 
        win_base_address = 0x9c00;
    else 
        win_base_address = 0x9800;

    if (!tile_addressing_mode) 
        tileset_index_offset = 256;
    
    uint16_t window_tile_index;
    SDL_SetRenderTarget(renderer, window_layer);
    for (window_tile_index = 0x00; window_tile_index < 0x400; window_tile_index++) {
        uint8_t tile_id = vram[win_base_address+window_tile_index - 0x8000];
        int tile_index = tile_id;

        // for breakpoint
        // if (background_tile_index == 0x104) {
        //     std::cout << "here" << std::endl;
        // }

        if (!tile_addressing_mode && tile_index > 127)
            tile_index -= 256;
        
        SDL_Texture* tile = tileset[tile_index+tileset_index_offset];

        SDL_Rect tile_rect;
        tile_rect.x = (window_tile_index % 32) * 8;
        tile_rect.y = (window_tile_index / 32) * 8;
        tile_rect.w = 8;
        tile_rect.h = 8;
        // std::cout<<tile_rect.x<<std::endl;
        SDL_RenderCopy(renderer, tile, nullptr, &tile_rect);
    }
    SDL_SetRenderTarget(renderer, nullptr);
}

void PPU::drawWindow() {
    if (((lcdc >> 5) & 1) && (lcdc & 1)) {
        #ifndef FULL_VIEWPORT
        SDL_Rect viewport = {0, 0, 256, 256};
        SDL_Rect display = {wx-7, wy, 256, 256};
        SDL_RenderCopy(renderer, window_layer, &viewport, &display);
        #endif
        #ifdef FULL_VIEWPORT
        SDL_RenderCopy(renderer, background_layer, nullptr, nullptr);
        #endif
        // SDL_RenderPresent(renderer);
    }
}


void PPU::updateTextures() {
    // if the last getpttr was for writing to vram or oam,
    if (cpuRead(last_pttr_addr) != last_pttr_value) {
        if (0x8000 <= last_pttr_addr && last_pttr_addr <= 0x97FF) {
            updateTileset();
        }
        if (last_pttr_addr <= 0x9FFF)
            updateBackgroundLayer();
        last_pttr_value = cpuRead(last_pttr_addr);
    }
}



#ifdef TILESET_WINDOW
void PPU::drawTilesetWindow() {
    uint16_t tileindex;
    for (tileindex = 0; tileindex < 0x180; tileindex++) {

        SDL_Texture* tile = tileset[tileindex];

        SDL_Rect tile_rect;
        tile_rect.x = (tileindex % 16) * 8;
        tile_rect.y = (tileindex / 16) * 8;
        tile_rect.w = 8;
        tile_rect.h = 8;
        SDL_RenderCopy(tileset_renderer, tile, nullptr, &tile_rect);
    }
    SDL_RenderPresent(tileset_renderer);
}
#endif

#ifdef BG_WINDOW
void PPU::drawBGWindow() {
    SDL_Rect rect;
    rect.x = 0;
    rect.y = 0;
    rect.w = 32*8;
    rect.h = 32*8;
    SDL_RenderCopy(bg_renderer, background_layer, nullptr, &rect);
    SDL_RenderPresent(bg_renderer);
}
#endif