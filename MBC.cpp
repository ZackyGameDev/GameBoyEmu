#include "MBC.h"
#include <iostream>
#include "Cartridge.h"


#pragma region MBC BASE

MBC::MBC() {

}

MBC::~MBC() {

}

void MBC::cpuWrite(uint16_t addr, uint8_t data) {
    if (0xA000 <= addr && addr <= 0xBFFF)
        cart->sram[addr-0xA000] = data;
    else 
        printf("That is not an address implemented for MBCBASE.\n");
        // throw std::runtime_error("Invalid address range for MBC write.");
}

uint8_t MBC::cpuRead(uint16_t addr) {
    uint8_t data = 0x00;

    if (0x0000 <= addr && addr <= 0x7FFF)
        data = cart->cart_data[addr];
    else if (0xA000 <= addr && addr <= 0xBFFF)
        data = cart->sram[addr-0xA000];
    else
        throw std::runtime_error("That is not an address implemented for MBCBASE.");

    return data;
}

uint8_t *MBC::cpuReadPttr(uint16_t addr) {
    uint8_t *data = nullptr;

    if (0x0000 <= addr && addr <= 0x7FFF)
        data = &(cart->cart_data[addr]);
    else if (0xA000 <= addr && addr <= 0xBFFF)
        data = &(cart->sram[addr-0xA000]);
    else
        throw std::runtime_error("That is not an address implemented for MBCBASE.");

    return data;
}



#pragma region MBC1

MBC1::MBC1() {

}

MBC1::~MBC1() {

}

void MBC1::cpuWrite(uint16_t addr, uint8_t data) {
    if (0x0000 <= addr && addr <= 0x1FFF)
        sram_enabled = data;
    else if (0x2000 <= addr && addr <= 0x3FFF) {
        rom_bank_number = data & 0x1f; // because only the bottom five bits are taken
        if (rom_bank_number == 0x00)
            rom_bank_number = 0x01; // the rectification feature
    } else if (0x4000 <= addr && addr <= 0x5FFF)
        ram_bank_number = data & 0b0000'0011; 
    else if (0x6000 <= addr && addr <= 0x7FFF)
        banking_mode = data & 0x01; 
    else if (0xA000 <= addr && addr <= 0xBFFF) {
        if (sram_enabled & 0xa) { // if sram is enabled
            uint8_t bank = ram_bank_number;
            if (banking_mode == 1) {
                bank = 0;
            }
            cart->sram[addr-0xA000 + (bank)*0x4000] = data;
        }
    } else
        throw std::runtime_error("Invalid address range for MBC1 write.");
}

uint8_t MBC1::cpuRead(uint16_t addr) {
    uint8_t data = 0x00;
    uint8_t bank;

    if (0x0000 <= addr && addr <= 0x3FFF)
        data = cart->cart_data[addr];
    else if (0x4000 <= addr && addr <= 0x7FFF) {
        bank = (ram_bank_number << 5) | rom_bank_number;
        if (banking_mode == 0) {
            bank = bank & 0x1f; // in this mode RAM bank number is not extension of rom register
        }

        // bank = bank & cart->rom_bank_bits; // im leaving this line here as hall of shame
        bank = bank & ~((~0)<<cart->rom_bank_bits);
        data = cart->cart_data[addr + (bank-1)*0x4000];
    } else if (0xA000 <= addr && addr <= 0xBFFF) {
        if (sram_enabled & 0xa == 0) {
            data = no_data;
        } else {
            bank = ram_bank_number;
            if (banking_mode == 1) {
                bank = 0;
            }
            data = cart->sram[addr-0xA000 + (bank)*0x4000];
        }
    } else
        throw std::runtime_error("That is not an address implemented for MBC1.");

    return data;
}

uint8_t *MBC1::cpuReadPttr(uint16_t addr) {
    uint8_t *data = nullptr;
    uint8_t bank;
    
    if (0x0000 <= addr && addr <= 0x3FFF)
        data = &cart->cart_data[addr];
    else if (0x4000 <= addr && addr <= 0x7FFF) {
        bank = (ram_bank_number << 5) | rom_bank_number;
        if (banking_mode == 0) {
            bank = bank & 0x1f; // in this mode RAM bank number is not extension of rom register
        }

        bank = bank & ~(0xff << cart->rom_bank_bits);
        data = &cart->cart_data[addr + (bank-1)*0x4000];
    } else if (0xA000 <= addr && addr <= 0xBFFF) {
        if (sram_enabled & 0xa == 0) {
            data = &no_data;
        } else {
            bank = ram_bank_number;
            if (banking_mode == 1) {
                bank = 0;
            }
            data = &cart->sram[addr-0xA000 + (bank)*0x4000];
        }
    } else
        throw std::runtime_error("That is not an address implemented for MBC1.");
    
    return data;
}