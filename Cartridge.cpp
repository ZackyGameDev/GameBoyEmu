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
    unsigned int rombanksize = 32768;
    rom.resize(rombanksize);
    file.seekg(0x150, std::ios::beg);

    char* read_buffer = new char[rombanksize];
    
    file.read(read_buffer, rombanksize);
    for (int i = 0; i < rombanksize; i++) {
        rom[i] = read_buffer[i];
    }
    
    delete[] read_buffer;

    // file.read(reinterpret_cast<char*>(), size);
    file.close();

    std::cout << "[DEBUG] ROM LOADED! <-------\n";
}

Cartridge::~Cartridge() {
    rom.clear();
}

uint8_t Cartridge::read(uint16_t address) {
    if (address >= 0x150) {
        return rom[address - 0x150];
    }
    return rom[address];
}

uint8_t* Cartridge::readPttr(uint16_t address) {
    if (address >= 0x150) {
        return &rom[address - 0x150];
    }
    return &rom[address];
}

void Cartridge::write(uint16_t address, uint8_t data) {
    throw std::runtime_error("Cannot write to ROM");
}