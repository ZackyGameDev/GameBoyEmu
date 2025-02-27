#pragma once
#include "definitions.h"
#include <vector>
#include <iostream>
#include <cstdint>
#include <array>
#include <string>

class MBC;
class Bus;

class Cartridge {
public:
    Cartridge(const std::string filename);
    ~Cartridge();

    std::array<uint8_t, 0x20000> sram;
private:
    struct RomHeader {
        char title[16];
        char license_code[2];
        uint8_t sgb_flag;
        uint8_t cart_type;
        uint8_t rom_size_id;
        uint8_t ram_size_id;
        uint8_t dest_code;
        uint8_t old_license_code;
        uint8_t rom_version_number;
        uint8_t header_checksum;
        uint16_t global_checksum;
    } header;

    Bus *bus = nullptr;
    MBC *mbc;

    // this variable will help later in checking if a byte pointer
    // retrieved through readPttr() was used to read the value or 
    // change said value.  
    uint16_t last_pttr_addr; 
    uint8_t  last_pttr_value;
    
public:    
    uint8_t rom_bank_bits; // = rom_size_id + 1  
    // i would've preferred to keep this in private.. 
    // but the MBCs need to be able to access this raw 
    // data by my design. 
    std::vector<uint8_t> rom;
    std::vector<uint8_t> cart_data; 
    
    void connectBus(Bus *n) { bus = n; }

    uint8_t read(uint16_t address);
    uint8_t* readPttr(uint16_t address);
    void write(uint16_t address, uint8_t data);
    void rectifyPttrWrites();

    // debug functions
    #ifdef DEBUGMODE_
    std::vector<uint16_t> getValidInstructionAddresses();
    std::vector<uint16_t> valid_instruction_addresses;
    #endif

};
