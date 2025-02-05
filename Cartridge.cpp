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

    #ifdef DEBUGMODE_
    valid_instruction_addresses = getValidInstructionAddresses();
    #endif
}

Cartridge::~Cartridge() {
    rom.clear();
}

uint8_t Cartridge::read(uint16_t address) {
    if (address < 0x8000) {
        return rom[address];
    } 
    return sram[address - 0xa000];
}

uint8_t* Cartridge::readPttr(uint16_t address) {
    
    if (address < 0x8000) {
        return &rom[address];
    }
    return &sram[address - 0xa000];
}

void Cartridge::write(uint16_t address, uint8_t data) {
    if (address < 0x8000) {
        throw std::runtime_error("Attempted to write to ROM");
    } else if (0xa000 <= address and address <= 0xbfff) {
        sram[address - 0xa000] = data;
    } 
}

#ifdef DEBUGMODE_
std::vector<uint16_t> Cartridge::getValidInstructionAddresses() {
    std::ifstream file("ROMS/validaddr.out"); // open the file
    std::vector<uint16_t> addresses;
    std::string address;

    // read each line in the file
    while (std::getline(file, address)) {
        // convert the line to an integer and add it to the vector
        addresses.push_back(std::stoi(address));
    }

    file.close(); // close the file

    return addresses;
}
#endif 