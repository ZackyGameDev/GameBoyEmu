#include "Cartridge.h"
#include <iostream>
#include <fstream>

Cartridge::Cartridge(const std::string filename) {

    std::cout << "[DEBUG] Loading ROM file: " << filename << "\n";
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    file.unsetf(std::ios::skipws);

    if (!file.is_open()) {
        std::cout << "[[ERROR]] ⚠️ FAILED TO LOAD ROM\n";
        throw std::runtime_error("Failed to open ROM file");
    }

    std::streampos filesize = file.tellg();
    unsigned int rombanksize = 0x8000;
    rom.resize(rombanksize);
    file.seekg(std::ios::beg);

    std::vector<char> read_buffer(rombanksize);
    
    file.read(read_buffer.data(), rombanksize);
    for (int i = 0; i < rombanksize; i++) {
        rom[i] = read_buffer[i];
        // if (i > 0x13f && i < 0x170)
        // std::cout << std::hex << i << " : " << (int)rom[i] << "\n";
    }
    
    read_buffer.clear();
    read_buffer.shrink_to_fit();

    // file.read(reinterpret_cast<char*>(), size);
    file.close();

    std::cout << "[DEBUG] ROM LOADED! <-------\n";
    
    for (auto &i : sram) i = 0x00;
}

Cartridge::~Cartridge() {
    rom.clear();
}

uint8_t Cartridge::read(uint16_t address) {
    return rom[address];
}

uint8_t* Cartridge::readPttr(uint16_t address) {
    return &rom[address];
}

void Cartridge::write(uint16_t address, uint8_t data) {
    throw std::runtime_error("Cannot write to ROM");
}