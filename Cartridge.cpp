#include "Cartridge.h"
#include "MBC.h"
#include <iostream>
#include <fstream>

Cartridge::Cartridge(const std::string filename) {

    #pragma region ROMREAD

    rom.resize(0x8000);

    std::cout << "[DEBUG] Loading ROM file: " << filename << "\n";
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    file.unsetf(std::ios::skipws);

    if (!file.is_open()) {
        std::cout << "[[ERROR]] ⚠️ FAILED TO LOAD ROM\n";
        throw std::runtime_error("Failed to open ROM file");
    }

    std::streampos filesize = file.tellg();
    cart_data.resize(filesize);
    file.seekg(std::ios::beg);

    std::vector<char> read_buffer(filesize);
    
    file.read(read_buffer.data(), filesize);
    for (int i = 0; i < filesize; i++) {
        cart_data[i] = read_buffer[i];
        // if (i > 0x13f && i < 0x170)
        // std::cout << std::hex << i << " : " << (int)rom[i] << "\n";
    }
    
    // ridding the memory of the read buffer.
    read_buffer.clear();
    read_buffer.shrink_to_fit();

    // file.read(reinterpret_cast<char*>(), size);
    file.close();

    std::cout << "[DEBUG] ROM LOADED! <-------\n";
    
    #pragma region CART INIT

    if (filesize > 0x150) { // bootrom moment
    
        for (auto &i : sram) i = 0x00;

        uint16_t gp = 0x0134;
        // LOADING THE HEADER
        for (int i = 0; i < sizeof(RomHeader); ++i) {
            *(((char*)&header)+i) = cart_data[gp++];
        }
        // for (int i = 0; i < 16; ++i) {
        //     header.title[i] = cart_data[gp++];
        // }

        // for (int i = 0; i < 2; ++i) {
        //     header.license_code[i] = cart_data[gp++];
        // }
        
        // header.sgb_flag = cart_data[gp++];
        // header.cart_type = cart_data[gp++];
        // header.rom_size_id = cart_data[gp++];
        // header.ram_size_id = cart_data[gp++];
        // header.dest_code = cart_data[gp++];
        // header.old_license_code = cart_data[gp++];
        // header.rom_version_number = cart_data[gp++];
        // header.header_checksum = cart_data[gp++];
        // header.global_checksum = (cart_data[gp++] << 7) | cart_data[gp++];

        uint8_t rom_bank_bits = header.rom_size_id + 1;  

        std::cout 
        << "---------\nLOADED ROM\n--------\n" 
        << header.title << '\n'
        << "cart_type: " << std::hex << header.cart_type << std::dec << '\n';
        
    } else {
        header.cart_type = 0x00;
        header.rom_size_id = 0x00;
    }

    rom_bank_bits = header.rom_size_id + 1;
    switch (header.cart_type) {
        case 0x00:
            mbc = new MBC();
        case 0x01 ... 0x03:
            mbc = new MBC1();
        break;
        default:
            throw std::runtime_error("This cart type is not supported yet.");
    }
    mbc->connectCart(this);

    last_pttr_addr = 0x00;
    last_pttr_value = read(last_pttr_addr);

    #ifdef DEBUGMODE_
    valid_instruction_addresses = getValidInstructionAddresses();
    #endif
}

Cartridge::~Cartridge() {
    rom.clear();
}

uint8_t Cartridge::read(uint16_t address) {
    return mbc->cpuRead(address);
}

uint8_t* Cartridge::readPttr(uint16_t address) {
    uint8_t *data;
    data = mbc->cpuReadPttr(address);
    if (address <= 0x7FFF) {
        last_pttr_addr = address;
        last_pttr_value = *data;
    }
    return data;
}

void Cartridge::write(uint16_t address, uint8_t data) {
    return mbc->cpuWrite(address, data);
}

void Cartridge::rectifyPttrWrites() {
    // check if the last readPttr was used to write data
    if (last_pttr_addr <= 0x7FFF)
    if (mbc->cpuRead(last_pttr_addr) != last_pttr_value) {
        uint8_t* last_pttr = mbc->cpuReadPttr(last_pttr_addr); // get the new modified value
        write(last_pttr_addr, *last_pttr);   // write said value to the correct place
        *last_pttr = last_pttr_value;   // reset the modified value back to the correct one
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