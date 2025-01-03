#include "SM83.h"
#include "Bus.h"
#include <iostream>

SM83::SM83() {

    using x = SM83;
    unprefixed_opcode_lookup = { 
        {"NOP", &x::NOP, 4}, {"LD", &x::LD_BC_n16, 12}, {"LD", &x::LD_aBC_A, 8}, {"INC", &x::INC_BC, 8}, {"INC", &x::INC_B, 4},
        {"DEC", &x::DEC_B, 4}, {"LD", &x::LD_B_n8, 8}, {"RLCA", &x::RLCA, 4}, {"LD", &x::LD_aa16_SP, 20}, {"ADD", &x::ADD_HL_BC, 8},
        {"LD", &x::LD_A_aBC, 8}, {"DEC", &x::DEC_BC, 8}, {"INC", &x::INC_C, 4}, {"DEC", &x::DEC_C, 4}, {"LD", &x::LD_C_n8, 8},
        {"RRCA", &x::RRCA, 4}, {"STOP", &x::STOP_n8, 4}, {"LD", &x::LD_DE_n16, 12}, {"LD", &x::LD_aDE_A, 8}, {"INC", &x::INC_DE, 8},
        {"INC", &x::INC_D, 4}, {"DEC", &x::DEC_D, 4}, {"LD", &x::LD_D_n8, 8}, {"RLA", &x::RLA, 4}, {"JR", &x::JR_e8, 12},
        {"ADD", &x::ADD_HL_DE, 8}, {"LD", &x::LD_A_aDE, 8}, {"DEC", &x::DEC_DE, 8}, {"INC", &x::INC_E, 4}, {"DEC", &x::DEC_E, 4},
        {"LD", &x::LD_E_n8, 8}, {"RRA", &x::RRA, 4}, {"JR", &x::JR_NZ_e8, 12}, {"LD", &x::LD_HL_n16, 12}, {"LD", &x::LD_aHLI_A, 8},
        {"INC", &x::INC_HL, 8}, {"INC", &x::INC_H, 4}, {"DEC", &x::DEC_H, 4}, {"LD", &x::LD_H_n8, 8}, {"DAA", &x::DAA, 4},
        {"JR", &x::JR_Z_e8, 12}, {"ADD", &x::ADD_HL_HL, 8}, {"LD", &x::LD_A_aHLI, 8}, {"DEC", &x::DEC_HL, 8}, {"INC", &x::INC_L, 4},
        {"DEC", &x::DEC_L, 4}, {"LD", &x::LD_L_n8, 8}, {"CPL", &x::CPL, 4}, {"JR", &x::JR_NC_e8, 12}, {"LD", &x::LD_SP_n16, 12},
        {"LD", &x::LD_aHLD_A, 8}, {"INC", &x::INC_SP, 8}, {"INC", &x::INC_aHL, 12}, {"DEC", &x::DEC_aHL, 12}, {"LD", &x::LD_aHL_n8, 12},
        {"SCF", &x::SCF, 4}, {"JR", &x::JR_C_e8, 12}, {"ADD", &x::ADD_HL_SP, 8}, {"LD", &x::LD_A_aHLD, 8}, {"DEC", &x::DEC_SP, 8},
        {"INC", &x::INC_A, 4}, {"DEC", &x::DEC_A, 4}, {"LD", &x::LD_A_n8, 8}, {"CCF", &x::CCF, 4}, {"LD", &x::LD_B_B, 4},
        {"LD", &x::LD_B_C, 4}, {"LD", &x::LD_B_D, 4}, {"LD", &x::LD_B_E, 4}, {"LD", &x::LD_B_H, 4}, {"LD", &x::LD_B_L, 4},
        {"LD", &x::LD_B_aHL, 8}, {"LD", &x::LD_B_A, 4}, {"LD", &x::LD_C_B, 4}, {"LD", &x::LD_C_C, 4}, {"LD", &x::LD_C_D, 4},
        {"LD", &x::LD_C_E, 4}, {"LD", &x::LD_C_H, 4}, {"LD", &x::LD_C_L, 4}, {"LD", &x::LD_C_aHL, 8}, {"LD", &x::LD_C_A, 4},
        {"LD", &x::LD_D_B, 4}, {"LD", &x::LD_D_C, 4}, {"LD", &x::LD_D_D, 4}, {"LD", &x::LD_D_E, 4}, {"LD", &x::LD_D_H, 4},
        {"LD", &x::LD_D_L, 4}, {"LD", &x::LD_D_aHL, 8}, {"LD", &x::LD_D_A, 4}, {"LD", &x::LD_E_B, 4}, {"LD", &x::LD_E_C, 4},
        {"LD", &x::LD_E_D, 4}, {"LD", &x::LD_E_E, 4}, {"LD", &x::LD_E_H, 4}, {"LD", &x::LD_E_L, 4}, {"LD", &x::LD_E_aHL, 8},
        {"LD", &x::LD_E_A, 4}, {"LD", &x::LD_H_B, 4}, {"LD", &x::LD_H_C, 4}, {"LD", &x::LD_H_D, 4}, {"LD", &x::LD_H_E, 4},
        {"LD", &x::LD_H_H, 4}, {"LD", &x::LD_H_L, 4}, {"LD", &x::LD_H_aHL, 8}, {"LD", &x::LD_H_A, 4}, {"LD", &x::LD_L_B, 4},
        {"LD", &x::LD_L_C, 4}, {"LD", &x::LD_L_D, 4}, {"LD", &x::LD_L_E, 4}, {"LD", &x::LD_L_H, 4}, {"LD", &x::LD_L_L, 4},
        {"LD", &x::LD_L_aHL, 8}, {"LD", &x::LD_L_A, 4}, {"LD", &x::LD_aHL_B, 8}, {"LD", &x::LD_aHL_C, 8}, {"LD", &x::LD_aHL_D, 8},
        {"LD", &x::LD_aHL_E, 8}, {"LD", &x::LD_aHL_H, 8}, {"LD", &x::LD_aHL_L, 8}, {"HALT", &x::HALT, 4}, {"LD", &x::LD_aHL_A, 8},
        {"LD", &x::LD_A_B, 4}, {"LD", &x::LD_A_C, 4}, {"LD", &x::LD_A_D, 4}, {"LD", &x::LD_A_E, 4}, {"LD", &x::LD_A_H, 4},
        {"LD", &x::LD_A_L, 4}, {"LD", &x::LD_A_aHL, 8}, {"LD", &x::LD_A_A, 4}, {"ADD", &x::ADD_A_B, 4}, {"ADD", &x::ADD_A_C, 4},
        {"ADD", &x::ADD_A_D, 4}, {"ADD", &x::ADD_A_E, 4}, {"ADD", &x::ADD_A_H, 4}, {"ADD", &x::ADD_A_L, 4}, {"ADD", &x::ADD_A_aHL, 8},
        {"ADD", &x::ADD_A_A, 4}, {"ADC", &x::ADC_A_B, 4}, {"ADC", &x::ADC_A_C, 4}, {"ADC", &x::ADC_A_D, 4}, {"ADC", &x::ADC_A_E, 4},
        {"ADC", &x::ADC_A_H, 4}, {"ADC", &x::ADC_A_L, 4}, {"ADC", &x::ADC_A_aHL, 8}, {"ADC", &x::ADC_A_A, 4}, {"SUB", &x::SUB_A_B, 4},
        {"SUB", &x::SUB_A_C, 4}, {"SUB", &x::SUB_A_D, 4}, {"SUB", &x::SUB_A_E, 4}, {"SUB", &x::SUB_A_H, 4}, {"SUB", &x::SUB_A_L, 4},
        {"SUB", &x::SUB_A_aHL, 8}, {"SUB", &x::SUB_A_A, 4}, {"SBC", &x::SBC_A_B, 4}, {"SBC", &x::SBC_A_C, 4}, {"SBC", &x::SBC_A_D, 4},
        {"SBC", &x::SBC_A_E, 4}, {"SBC", &x::SBC_A_H, 4}, {"SBC", &x::SBC_A_L, 4}, {"SBC", &x::SBC_A_aHL, 8}, {"SBC", &x::SBC_A_A, 4},
        {"AND", &x::AND_A_B, 4}, {"AND", &x::AND_A_C, 4}, {"AND", &x::AND_A_D, 4}, {"AND", &x::AND_A_E, 4}, {"AND", &x::AND_A_H, 4},
        {"AND", &x::AND_A_L, 4}, {"AND", &x::AND_A_aHL, 8}, {"AND", &x::AND_A_A, 4}, {"XOR", &x::XOR_A_B, 4}, {"XOR", &x::XOR_A_C, 4},
        {"XOR", &x::XOR_A_D, 4}, {"XOR", &x::XOR_A_E, 4}, {"XOR", &x::XOR_A_H, 4}, {"XOR", &x::XOR_A_L, 4}, {"XOR", &x::XOR_A_aHL, 8},
        {"XOR", &x::XOR_A_A, 4}, {"OR", &x::OR_A_B, 4}, {"OR", &x::OR_A_C, 4}, {"OR", &x::OR_A_D, 4}, {"OR", &x::OR_A_E, 4},
        {"OR", &x::OR_A_H, 4}, {"OR", &x::OR_A_L, 4}, {"OR", &x::OR_A_aHL, 8}, {"OR", &x::OR_A_A, 4}, {"CP", &x::CP_A_B, 4},
        {"CP", &x::CP_A_C, 4}, {"CP", &x::CP_A_D, 4}, {"CP", &x::CP_A_E, 4}, {"CP", &x::CP_A_H, 4}, {"CP", &x::CP_A_L, 4},
        {"CP", &x::CP_A_aHL, 8}, {"CP", &x::CP_A_A, 4}, {"RET", &x::RET_NZ, 20}, {"POP", &x::POP_BC, 12}, {"JP", &x::JP_NZ_a16, 16},
        {"JP", &x::JP_a16, 16}, {"CALL", &x::CALL_NZ_a16, 24}, {"PUSH", &x::PUSH_BC, 16}, {"ADD", &x::ADD_A_n8, 8}, {"RST", &x::RST_00, 16},
        {"RET", &x::RET_Z, 20}, {"RET", &x::RET, 16}, {"JP", &x::JP_Z_a16, 16}, {"PREFIX", &x::PREFIX, 4}, {"CALL", &x::CALL_Z_a16, 24},
        {"CALL", &x::CALL_a16, 24}, {"ADC", &x::ADC_A_n8, 8}, {"RST", &x::RST_08, 16}, {"RET", &x::RET_NC, 20}, {"POP", &x::POP_DE, 12},
        {"JP", &x::JP_NC_a16, 16}, {"ILLEGAL_D3", &x::ILLEGAL_D3, 4}, {"CALL", &x::CALL_NC_a16, 24}, {"PUSH", &x::PUSH_DE, 16}, {"SUB", &x::SUB_A_n8, 8},
        {"RST", &x::RST_10, 16}, {"RET", &x::RET_C, 20}, {"RETI", &x::RETI, 16}, {"JP", &x::JP_C_a16, 16}, {"ILLEGAL_DB", &x::ILLEGAL_DB, 4},
        {"CALL", &x::CALL_C_a16, 24}, {"ILLEGAL_DD", &x::ILLEGAL_DD, 4}, {"SBC", &x::SBC_A_n8, 8}, {"RST", &x::RST_18, 16}, {"LDH", &x::LDH_aa8_A, 12},
        {"POP", &x::POP_HL, 12}, {"LDH", &x::LDH_aC_A, 8}, {"ILLEGAL_E3", &x::ILLEGAL_E3, 4}, {"ILLEGAL_E4", &x::ILLEGAL_E4, 4}, {"PUSH", &x::PUSH_HL, 16},
        {"AND", &x::AND_A_n8, 8}, {"RST", &x::RST_20, 16}, {"ADD", &x::ADD_SP_e8, 16}, {"JP", &x::JP_HL, 4}, {"LD", &x::LD_aa16_A, 16},
        {"ILLEGAL_EB", &x::ILLEGAL_EB, 4}, {"ILLEGAL_EC", &x::ILLEGAL_EC, 4}, {"ILLEGAL_ED", &x::ILLEGAL_ED, 4}, {"XOR", &x::XOR_A_n8, 8}, {"RST", &x::RST_28, 16},
        {"LDH", &x::LDH_A_aa8, 12}, {"POP", &x::POP_AF, 12}, {"LDH", &x::LDH_A_aC, 8}, {"DI", &x::DI, 4}, {"ILLEGAL_F4", &x::ILLEGAL_F4, 4},
        {"PUSH", &x::PUSH_AF, 16}, {"OR", &x::OR_A_n8, 8}, {"RST", &x::RST_30, 16}, {"LD", &x::LD_HL_SPI_e8, 12}, {"LD", &x::LD_SP_HL, 8},
        {"LD", &x::LD_A_aa16, 16}, {"EI", &x::EI, 4}, {"ILLEGAL_FC", &x::ILLEGAL_FC, 4}, {"ILLEGAL_FD", &x::ILLEGAL_FD, 4}, {"CP", &x::CP_A_n8, 8},
        {"RST", &x::RST_38, 16}, 
    };

    std::cout << "[DEBUG] opcodes loaded into the lookup table <-----\n";
}

SM83::~SM83() {

}

void SM83::clock() {
    uint8_t opcode = read(pc++);
    
    if (opcode != 0xcb) {
        (this->*unprefixed_opcode_lookup[opcode].operate)();
    }

}


uint8_t SM83::read(uint16_t addr) {
    return bus->cpuRead(addr);
}

uint8_t *SM83::readPttr(uint16_t addr) {
    return bus->cpuReadPttr(addr);
}

void SM83::write(uint16_t addr, uint8_t data) {
    bus->cpuWrite(addr, data);
}


uint8_t *SM83::process_operand(Operand operand) {
    uint8_t *value = nullptr;
    if (operand.immediate) {
        switch (operand.name) {
            case A:
                value = &a;
            case F: 
                value = &f;
            case B: 
                value = &b;
            case C:
                value = &c;
            case D:
                value = &d;
            case E:
                value = &e;
            case H:
                value = &h;
            case L: 
                value = &l;
            case N8:
                value = readPttr(pc++);
        }
    } else {
        int8_t di = operand.increment - operand.decrement;
        switch (operand.name) {
            case AF:
                value = readPttr((a << 8) | f);
                addToHiLo(a, f, di);
            case BC:
                value = readPttr((b << 8) | c);
                addToHiLo(b, c, di);
            case DE:
                value = readPttr((d << 8) | e);
                addToHiLo(d, e, di);
            case HL:
                value = readPttr((h << 8) | l);
                addToHiLo(h, l, di);
            case SP:
                value = readPttr(sp);
                sp += di;
            case A16: {
                uint8_t lo = read(pc++);
                uint8_t hi = read(pc++);
                value = readPttr((hi << 8) | lo);
            }
        }
    }

    return value;
}


uint16_t *SM83::process_operand16(Operand operand) {
    uint16_t* value = nullptr;
    if (operand.immediate) {
        switch (operand.name) {
            case AF:
                value = &af;
            case BC:
                value = &bc;
            case DE:
                value = &de;
            case HL:
                value = &hl;
            case SP:
                value = &sp;
            case N16: { 
                // n16 is never modified, only fetched. thus instead of trying to somehow 
                // get a pointer to (hram[addr] | (hram[addr+1] << 8)), i'm simply fetching 
                // it into a third variable and returning its reference.
                uint8_t lo = read(pc++);
                uint8_t hi = read(pc++);
                fetched16 = (hi << 8) | lo;
                value = &fetched16;
            }
        }
    } else {
        switch (operand.name) {
            // the memory is a vector<uint8_t> so instead of trying to figure
            // out how to return two different pointers to make up a uint16_t*, im simply letting the 
            // function deal with it manually by fetching the address into a variable.

            // only one case here because none of the opcodes have indirect addressing with a register as an operand.
            case A16: {
                uint8_t lo = read(pc++);
                uint8_t hi = read(pc++);
                addr_abs = (hi << 8) | lo; 
            }
        }
    }

    return value;
}


////// PRIMARY OPERATION CORE FUNCTIONS 

// LOAD INSTRUCTIONS
uint8_t SM83::LD(Operand target, Operand source) {
    uint8_t *targetValue = process_operand(target);
    uint8_t *sourceValue = process_operand(source);
    *targetValue = *sourceValue;

    updateRegisters16();
    return 0;
}

uint8_t SM83::LD16(Operand target, Operand source) {

    if (target.immediate) {
        uint16_t *targetValue = process_operand16(target);
        uint16_t *sourceValue = process_operand16(source);
        *targetValue = *sourceValue;
    } else {
        uint16_t *sourceValue = process_operand16(source);
        process_operand16(target); // this should put addr in this->addr_abs
        write(addr_abs, *sourceValue & 0xff);
        write(addr_abs + 1, *sourceValue >> 8);
    }

    updateRegisters8();
    return 0;
}


uint8_t SM83::LD_HL_SPDD() {

    int8_t e8 = toSigned(read(pc++));
    hl = sp + e8;

    // flags
    setFlag(fz, 0);
    setFlag(fn, 0);
    setFlag( fh, ((sp & 0x0F) + (e8 & 0x0F)) & 0x10 );
    setFlag( fc, ((sp & 0xFF) + (e8 & 0xFF)) & 0x100 );

    updateRegisters8();
    return 0;
}


uint8_t SM83::LDH(Operand source, Operand target) {
    uint8_t sourceValue = 0;
    if (source.name == C) {
        sourceValue = read(0xff00 + read(c));
    } else if (source.name == A8) {
        sourceValue = read(0xff00 + read(pc++));
    } else if (source.name = A) {
        sourceValue = a;
    }

    uint8_t *targetValue = nullptr;
    if (target.name == A) {
        targetValue = &a;
    } else if (target.name == C) {
        targetValue = readPttr(0xff00 + c);
    } else if (target.name == A8) {
        targetValue = readPttr(0xff00 + read(pc++));
    }

    *targetValue = sourceValue; 
    
    updateRegisters16();
    return 0;
}


uint8_t SM83::POP(Operand target) {
    uint8_t lo = read(sp++);
    uint8_t hi = read(sp++);

    uint16_t *target_register = process_operand16(target);
    
    *target_register = (hi << 8) | lo;

    // flags
    // since f register *is* my flag carrier, flags get set automatically

    updateRegisters8();
    return 0;
}

uint8_t SM83::PUSH(Operand target) {
    uint16_t *target_register = process_operand16(target);

    write(--sp, *target_register >> 8);
    write(--sp, *target_register & 0xff);

    updateRegisters8();
    return 0;
}

//// 8 BIT ALU INSTRUCTIONS 

// C and Z flags and the register update is called by this function only. 
// rest is by core functions (if present)

uint8_t SM83::PROCESS_ALU(ALUOperation operation) {

    switch (operation) {

    case enumCCF:
        setFlag(fc, !getFlag(fc));
        break;
    case enumCPL:
        a = ~a;
        setFlag(fn, 1);
        setFlag(fh, 1);
        break;
    case enumSCF:
        setFlag(fc, 1);
        setFlag(fn, 0);
        setFlag(fh, 0);
        break;
    case enumDAA:
        if (getFlag(fn)) {
            if (getFlag(fc)) { a -= 0x60; }
            if (getFlag(fh)) { a -= 0x06; }
        } else {
            if (getFlag(fc) || (a & 0xFF) > 0x99) { a += 0x60; setFlag(fc, 1); }
            if (getFlag(fh) || (a & 0x0F) > 0x09) { a += 0x06; }
        }
        setFlag(fz, a == 0);
        setFlag(fh, 0);
        break;
    }

    updateRegisters16();
    return 0;
}


uint8_t SM83::PROCESS_ALU(Operand target, ALUOperation operation) {
    uint8_t *targetValue = process_operand(target);

    switch (operation) {
    
    case INC:
        *targetValue += 1;
        setFlag(fh, *targetValue & 0x10);
        setFlag(fn, 0);
        break;
    case DEC:
        *targetValue -= 1;
        setFlag(fn, 1);
        setFlag(fh, *targetValue & 0x10);
        break;
    }
    
    setFlag(fz, *targetValue == 0);
    updateRegisters16();
    return 0;
}



uint8_t SM83::PROCESS_ALU(Operand target, Operand source, ALUOperation operation) {
    uint8_t *targetValue = process_operand(target);
    uint8_t *sourceValue = process_operand(source);

    switch (operation) {
        case ADD:
            SM83::ADD8(targetValue, sourceValue);
            break;
        case ADC:
            SM83::ADD8(targetValue, sourceValue, true);
            break;
        case SUB:
            SM83::SUB8(targetValue, sourceValue);
            break;
        case SBC:
            SM83::SUB8(targetValue, sourceValue, true);
            break;
        case AND:
            *targetValue &= *sourceValue;
            setFlag(fh, 1);
            break;
        case XOR:
            *targetValue ^= *sourceValue;
            setFlag(fh, 0);
            break;
        case OR:
            *targetValue |= *sourceValue;
            setFlag(fh, 0);
            break;
        case CP: {
            uint16_t result = *targetValue - *sourceValue;
            setFlag(fn, 1);
            setFlag(fh, (*targetValue & 0x0F) + (*sourceValue & 0x0F) & 0x10);
            setFlag(fz, result == 0);
            setFlag(fc, result & 0x0100);
            return 0;
        }
    }

    setFlag(fz, *targetValue & 0xff == 0);
    setFlag(fc, *targetValue & 0x0100);

    updateRegisters16();
    return 0;
}



uint8_t SM83::PROCESS_ALU16(Operand target, ALUOperation operation) {

    uint16_t *targetValue = process_operand16(target);

    switch (operation) {
    case INC:
        *targetValue++;
        break;
    case DEC:
        *targetValue--;
        break;
    }

    updateRegisters8();
    return 0;

}


uint8_t SM83::PROCESS_ALU16(Operand target, Operand source, ALUOperation operation) {

    uint16_t *targetValue = process_operand16(target);

    uint16_t storage = 0;
    uint16_t *sourceValue = &storage;

    if (source.name == E8) { 
        // something tells me this implementation is not supposed to be this easy. 
        // but it really depends how C++ handles uint and int behind the scenes
        // so i wont know which method of implementation is correct until i 
        // actually test it out.
    
        *sourceValue = static_cast<uint16_t>(toSigned(read(pc++)));
    } else {
        sourceValue = process_operand16(source);
    }

    switch (operation) {
    // theres only one operation in 16 bit ALU other than inc and dec. so i'm 
    // so i'm not separating addition out into a different core helper function.
    case ADD: 
        uint32_t result = *targetValue + *sourceValue;

        setFlag(fh, ((*targetValue & 0xFFF) + (*sourceValue & 0xFFF)) & 0x1000);
        setFlag(fc, result > 0xFFFF);
        setFlag(fn, 0);
        
        *targetValue = result & 0xFFFF;
    }

    updateRegisters8();
    return 0;
}



// CORE HELPERS

uint8_t SM83::ADD8(uint8_t *targetValue, uint8_t *sourceValue, bool isADC) {
    short carry = 0;
    if (isADC) {
        carry = getFlag(fc);
    }

    uint16_t result = *targetValue + *sourceValue + carry;

    // flags
    setFlag(fn, 0);
    setFlag(fh, ((*targetValue & 0x0F) + (*sourceValue & 0x0F) + carry) & 0x10);
    // Flags C and Z are set by the function wrapping this one (PROCESS_ALU)

    *targetValue = result & 0xff;

    return 0;
}

uint8_t SM83::SUB8(uint8_t *targetValue, uint8_t *sourceValue, bool isSBC) {
    short borrow = 0;
    if (isSBC) {
        borrow = getFlag(fc);
    }

    uint16_t result = *targetValue - *sourceValue - borrow;

    // flags
    setFlag(fn, 1);
    setFlag(fh, ((*targetValue & 0x0F) - (*sourceValue & 0x0F) - borrow) & 0x10);

    *targetValue = result & 0xff;

    return 0;
}


//// WRAPPER FUNCTIONS

uint8_t SM83::NOP() { return 0; }
uint8_t SM83::LD_BC_n16() { return LD16({BC, true}, {N16, true}); }
uint8_t SM83::LD_aBC_A() { return LD({BC, false}, {A, true}); }
uint8_t SM83::INC_BC() { return PROCESS_ALU16({BC, true}, INC); }
uint8_t SM83::INC_B() { return PROCESS_ALU({B, true}, INC); }
uint8_t SM83::DEC_B() { return PROCESS_ALU({B, true}, DEC); }
uint8_t SM83::LD_B_n8() { return LD({B, true}, {N8, true}); }
uint8_t SM83::RLCA() { return 0; }
uint8_t SM83::LD_aa16_SP() { return LD16({A16, false}, {SP, true}); }
uint8_t SM83::ADD_HL_BC() { return PROCESS_ALU16({HL, true}, {BC, true}, ADD); }
uint8_t SM83::LD_A_aBC() { return LD({A, true}, {BC, false}); }
uint8_t SM83::DEC_BC() { return PROCESS_ALU16({BC, true}, DEC); }
uint8_t SM83::INC_C() { return PROCESS_ALU({C, true}, INC); }
uint8_t SM83::DEC_C() { return PROCESS_ALU({C, true}, DEC); }
uint8_t SM83::LD_C_n8() { return LD({C, true}, {N8, true}); }
uint8_t SM83::RRCA() { return 0; }
uint8_t SM83::STOP_n8() { return 0; }
uint8_t SM83::LD_DE_n16() { return LD16({DE, true}, {N16, true}); }
uint8_t SM83::LD_aDE_A() { return LD({DE, false}, {A, true}); }
uint8_t SM83::INC_DE() { return PROCESS_ALU16({DE, true}, INC); }
uint8_t SM83::INC_D() { return PROCESS_ALU({D, true}, INC); }
uint8_t SM83::DEC_D() { return PROCESS_ALU({D, true}, DEC); }
uint8_t SM83::LD_D_n8() { return LD({D, true}, {N8, true}); }
uint8_t SM83::RLA() { return 0; }
uint8_t SM83::JR_e8() { return 0; }
uint8_t SM83::ADD_HL_DE() { return PROCESS_ALU16({HL, true}, {DE, true}, ADD); }
uint8_t SM83::LD_A_aDE() { return LD({A, true}, {DE, false}); }
uint8_t SM83::DEC_DE() { return PROCESS_ALU16({DE, true}, DEC); }
uint8_t SM83::INC_E() { return PROCESS_ALU({E, true}, INC); }
uint8_t SM83::DEC_E() { return PROCESS_ALU({E, true}, DEC); }
uint8_t SM83::LD_E_n8() { return LD({E, true}, {N8, true}); }
uint8_t SM83::RRA() { return 0; }
uint8_t SM83::JR_NZ_e8() { return 0; }
uint8_t SM83::LD_HL_n16() { return LD16({HL, true}, {N16, true}); }
uint8_t SM83::LD_aHLI_A() { return LD({HL, false, .increment=true}, {A, true}); }
uint8_t SM83::INC_HL() { return PROCESS_ALU16({HL, true}, INC); }
uint8_t SM83::INC_H() { return PROCESS_ALU({H, true}, INC); }
uint8_t SM83::DEC_H() { return PROCESS_ALU({H, true}, DEC); }
uint8_t SM83::LD_H_n8() { return LD({H, true}, {N8, true}); }
uint8_t SM83::DAA() { return PROCESS_ALU(enumDAA); }
uint8_t SM83::JR_Z_e8() { return 0; }
uint8_t SM83::ADD_HL_HL() { return PROCESS_ALU16({HL, true}, {HL, true}, ADD); }
uint8_t SM83::LD_A_aHLI() { return LD({A, true}, {HL, false, .increment=true}); }
uint8_t SM83::DEC_HL() { return PROCESS_ALU16({HL, true}, DEC); }
uint8_t SM83::INC_L() { return PROCESS_ALU({L, true}, INC); }
uint8_t SM83::DEC_L() { return PROCESS_ALU({L, true}, DEC); }
uint8_t SM83::LD_L_n8() { return LD({L, true}, {N8, true}); }
uint8_t SM83::CPL() { return PROCESS_ALU(enumCPL); }
uint8_t SM83::JR_NC_e8() { return 0; }
uint8_t SM83::LD_SP_n16() { return LD16({SP, true}, {N16, true}); }
uint8_t SM83::LD_aHLD_A() { return LD({HL, false, .decrement=true}, {A, true}); }
uint8_t SM83::INC_SP() { return PROCESS_ALU16({SP, true}, INC); }
uint8_t SM83::INC_aHL() { return PROCESS_ALU({HL, false}, INC); }
uint8_t SM83::DEC_aHL() { return PROCESS_ALU({HL, false}, DEC); }
uint8_t SM83::LD_aHL_n8() { return LD({HL, false}, {N8, true}); }
uint8_t SM83::SCF() { return PROCESS_ALU(enumSCF); }
uint8_t SM83::JR_C_e8() { return 0; }
uint8_t SM83::ADD_HL_SP() { return PROCESS_ALU16({HL, true}, {SP, true}, ADD); }
uint8_t SM83::LD_A_aHLD() { return LD({A, true}, {HL, false, .decrement=true}); }
uint8_t SM83::DEC_SP() { return PROCESS_ALU16({SP, true}, DEC); }
uint8_t SM83::INC_A() { return PROCESS_ALU({A, true}, INC); }
uint8_t SM83::DEC_A() { return PROCESS_ALU({A, true}, DEC); }
uint8_t SM83::LD_A_n8() { return LD({A, true}, {N8, true}); }
uint8_t SM83::CCF() { return PROCESS_ALU(enumCCF); }
uint8_t SM83::LD_B_B() { return LD({B, true}, {B, true}); }
uint8_t SM83::LD_B_C() { return LD({B, true}, {C, true}); }
uint8_t SM83::LD_B_D() { return LD({B, true}, {D, true}); }
uint8_t SM83::LD_B_E() { return LD({B, true}, {E, true}); }
uint8_t SM83::LD_B_H() { return LD({B, true}, {H, true}); }
uint8_t SM83::LD_B_L() { return LD({B, true}, {L, true}); }
uint8_t SM83::LD_B_aHL() { return LD({B, true}, {HL, false}); }
uint8_t SM83::LD_B_A() { return LD({B, true}, {A, true}); }
uint8_t SM83::LD_C_B() { return LD({C, true}, {B, true}); }
uint8_t SM83::LD_C_C() { return LD({C, true}, {C, true}); }
uint8_t SM83::LD_C_D() { return LD({C, true}, {D, true}); }
uint8_t SM83::LD_C_E() { return LD({C, true}, {E, true}); }
uint8_t SM83::LD_C_H() { return LD({C, true}, {H, true}); }
uint8_t SM83::LD_C_L() { return LD({C, true}, {L, true}); }
uint8_t SM83::LD_C_aHL() { return LD({C, true}, {HL, false}); }
uint8_t SM83::LD_C_A() { return LD({C, true}, {A, true}); }
uint8_t SM83::LD_D_B() { return LD({D, true}, {B, true}); }
uint8_t SM83::LD_D_C() { return LD({D, true}, {C, true}); }
uint8_t SM83::LD_D_D() { return LD({D, true}, {D, true}); }
uint8_t SM83::LD_D_E() { return LD({D, true}, {E, true}); }
uint8_t SM83::LD_D_H() { return LD({D, true}, {H, true}); }
uint8_t SM83::LD_D_L() { return LD({D, true}, {L, true}); }
uint8_t SM83::LD_D_aHL() { return LD({D, true}, {HL, false}); }
uint8_t SM83::LD_D_A() { return LD({D, true}, {A, true}); }
uint8_t SM83::LD_E_B() { return LD({E, true}, {B, true}); }
uint8_t SM83::LD_E_C() { return LD({E, true}, {C, true}); }
uint8_t SM83::LD_E_D() { return LD({E, true}, {D, true}); }
uint8_t SM83::LD_E_E() { return LD({E, true}, {E, true}); }
uint8_t SM83::LD_E_H() { return LD({E, true}, {H, true}); }
uint8_t SM83::LD_E_L() { return LD({E, true}, {L, true}); }
uint8_t SM83::LD_E_aHL() { return LD({E, true}, {HL, false}); }
uint8_t SM83::LD_E_A() { return LD({E, true}, {A, true}); }
uint8_t SM83::LD_H_B() { return LD({H, true}, {B, true}); }
uint8_t SM83::LD_H_C() { return LD({H, true}, {C, true}); }
uint8_t SM83::LD_H_D() { return LD({H, true}, {D, true}); }
uint8_t SM83::LD_H_E() { return LD({H, true}, {E, true}); }
uint8_t SM83::LD_H_H() { return LD({H, true}, {H, true}); }
uint8_t SM83::LD_H_L() { return LD({H, true}, {L, true}); }
uint8_t SM83::LD_H_aHL() { return LD({H, true}, {HL, false}); }
uint8_t SM83::LD_H_A() { return LD({H, true}, {A, true}); }
uint8_t SM83::LD_L_B() { return LD({L, true}, {B, true}); }
uint8_t SM83::LD_L_C() { return LD({L, true}, {C, true}); }
uint8_t SM83::LD_L_D() { return LD({L, true}, {D, true}); }
uint8_t SM83::LD_L_E() { return LD({L, true}, {E, true}); }
uint8_t SM83::LD_L_H() { return LD({L, true}, {H, true}); }
uint8_t SM83::LD_L_L() { return LD({L, true}, {L, true}); }
uint8_t SM83::LD_L_aHL() { return LD({L, true}, {HL, false}); }
uint8_t SM83::LD_L_A() { return LD({L, true}, {A, true}); }
uint8_t SM83::LD_aHL_B() { return LD({HL, false}, {B, true}); }
uint8_t SM83::LD_aHL_C() { return LD({HL, false}, {C, true}); }
uint8_t SM83::LD_aHL_D() { return LD({HL, false}, {D, true}); }
uint8_t SM83::LD_aHL_E() { return LD({HL, false}, {E, true}); }
uint8_t SM83::LD_aHL_H() { return LD({HL, false}, {H, true}); }
uint8_t SM83::LD_aHL_L() { return LD({HL, false}, {L, true}); }
uint8_t SM83::HALT() { return 0; }
uint8_t SM83::LD_aHL_A() { return LD({HL, false}, {A, true}); }
uint8_t SM83::LD_A_B() { return LD({A, true}, {B, true}); }
uint8_t SM83::LD_A_C() { return LD({A, true}, {C, true}); }
uint8_t SM83::LD_A_D() { return LD({A, true}, {D, true}); }
uint8_t SM83::LD_A_E() { return LD({A, true}, {E, true}); }
uint8_t SM83::LD_A_H() { return LD({A, true}, {H, true}); }
uint8_t SM83::LD_A_L() { return LD({A, true}, {L, true}); }
uint8_t SM83::LD_A_aHL() { return LD({A, true}, {HL, false}); }
uint8_t SM83::LD_A_A() { return LD({A, true}, {A, true}); }
uint8_t SM83::ADD_A_B() { return PROCESS_ALU({A, true}, {B, true}, ADD); }
uint8_t SM83::ADD_A_C() { return PROCESS_ALU({A, true}, {C, true}, ADD); }
uint8_t SM83::ADD_A_D() { return PROCESS_ALU({A, true}, {D, true}, ADD); }
uint8_t SM83::ADD_A_E() { return PROCESS_ALU({A, true}, {E, true}, ADD); }
uint8_t SM83::ADD_A_H() { return PROCESS_ALU({A, true}, {H, true}, ADD); }
uint8_t SM83::ADD_A_L() { return PROCESS_ALU({A, true}, {L, true}, ADD); }
uint8_t SM83::ADD_A_aHL() { return PROCESS_ALU({A, true}, {HL, false}, ADD); }
uint8_t SM83::ADD_A_A() { return PROCESS_ALU({A, true}, {A, true}, ADD); }
uint8_t SM83::ADC_A_B() { return PROCESS_ALU({A, true}, {B, true}, ADC); }
uint8_t SM83::ADC_A_C() { return PROCESS_ALU({A, true}, {C, true}, ADC); }
uint8_t SM83::ADC_A_D() { return PROCESS_ALU({A, true}, {D, true}, ADC); }
uint8_t SM83::ADC_A_E() { return PROCESS_ALU({A, true}, {E, true}, ADC); }
uint8_t SM83::ADC_A_H() { return PROCESS_ALU({A, true}, {H, true}, ADC); }
uint8_t SM83::ADC_A_L() { return PROCESS_ALU({A, true}, {L, true}, ADC); }
uint8_t SM83::ADC_A_aHL() { return PROCESS_ALU({A, true}, {HL, false}, ADC); }
uint8_t SM83::ADC_A_A() { return PROCESS_ALU({A, true}, {A, true}, ADC); }
uint8_t SM83::SUB_A_B() { return PROCESS_ALU({A, true}, {B, true}, SUB); }
uint8_t SM83::SUB_A_C() { return PROCESS_ALU({A, true}, {C, true}, SUB); }
uint8_t SM83::SUB_A_D() { return PROCESS_ALU({A, true}, {D, true}, SUB); }
uint8_t SM83::SUB_A_E() { return PROCESS_ALU({A, true}, {E, true}, SUB); }
uint8_t SM83::SUB_A_H() { return PROCESS_ALU({A, true}, {H, true}, SUB); }
uint8_t SM83::SUB_A_L() { return PROCESS_ALU({A, true}, {L, true}, SUB); }
uint8_t SM83::SUB_A_aHL() { return PROCESS_ALU({A, true}, {HL, false}, SUB); }
uint8_t SM83::SUB_A_A() { return PROCESS_ALU({A, true}, {A, true}, SUB); }
uint8_t SM83::SBC_A_B() { return PROCESS_ALU({A, true}, {B, true}, SBC); }
uint8_t SM83::SBC_A_C() { return PROCESS_ALU({A, true}, {C, true}, SBC); }
uint8_t SM83::SBC_A_D() { return PROCESS_ALU({A, true}, {D, true}, SBC); }
uint8_t SM83::SBC_A_E() { return PROCESS_ALU({A, true}, {E, true}, SBC); }
uint8_t SM83::SBC_A_H() { return PROCESS_ALU({A, true}, {H, true}, SBC); }
uint8_t SM83::SBC_A_L() { return PROCESS_ALU({A, true}, {L, true}, SBC); }
uint8_t SM83::SBC_A_aHL() { return PROCESS_ALU({A, true}, {HL, false}, SBC); }
uint8_t SM83::SBC_A_A() { return PROCESS_ALU({A, true}, {A, true}, SBC); }
uint8_t SM83::AND_A_B() { return PROCESS_ALU({A, true}, {B, true}, AND); }
uint8_t SM83::AND_A_C() { return PROCESS_ALU({A, true}, {C, true}, AND); }
uint8_t SM83::AND_A_D() { return PROCESS_ALU({A, true}, {D, true}, AND); }
uint8_t SM83::AND_A_E() { return PROCESS_ALU({A, true}, {E, true}, AND); }
uint8_t SM83::AND_A_H() { return PROCESS_ALU({A, true}, {H, true}, AND); }
uint8_t SM83::AND_A_L() { return PROCESS_ALU({A, true}, {L, true}, AND); }
uint8_t SM83::AND_A_aHL() { return PROCESS_ALU({A, true}, {HL, false}, AND); }
uint8_t SM83::AND_A_A() { return PROCESS_ALU({A, true}, {A, true}, AND); }
uint8_t SM83::XOR_A_B() { return PROCESS_ALU({A, true}, {B, true}, XOR); }
uint8_t SM83::XOR_A_C() { return PROCESS_ALU({A, true}, {C, true}, XOR); }
uint8_t SM83::XOR_A_D() { return PROCESS_ALU({A, true}, {D, true}, XOR); }
uint8_t SM83::XOR_A_E() { return PROCESS_ALU({A, true}, {E, true}, XOR); }
uint8_t SM83::XOR_A_H() { return PROCESS_ALU({A, true}, {H, true}, XOR); }
uint8_t SM83::XOR_A_L() { return PROCESS_ALU({A, true}, {L, true}, XOR); }
uint8_t SM83::XOR_A_aHL() { return PROCESS_ALU({A, true}, {HL, false}, XOR); }
uint8_t SM83::XOR_A_A() { return PROCESS_ALU({A, true}, {A, true}, XOR); }
uint8_t SM83::OR_A_B() { return PROCESS_ALU({A, true}, {B, true}, OR); }
uint8_t SM83::OR_A_C() { return PROCESS_ALU({A, true}, {C, true}, OR); }
uint8_t SM83::OR_A_D() { return PROCESS_ALU({A, true}, {D, true}, OR); }
uint8_t SM83::OR_A_E() { return PROCESS_ALU({A, true}, {E, true}, OR); }
uint8_t SM83::OR_A_H() { return PROCESS_ALU({A, true}, {H, true}, OR); }
uint8_t SM83::OR_A_L() { return PROCESS_ALU({A, true}, {L, true}, OR); }
uint8_t SM83::OR_A_aHL() { return PROCESS_ALU({A, true}, {HL, false}, OR); }
uint8_t SM83::OR_A_A() { return PROCESS_ALU({A, true}, {A, true}, OR); }
uint8_t SM83::CP_A_B() { return PROCESS_ALU({A, true}, {B, true}, CP); }
uint8_t SM83::CP_A_C() { return PROCESS_ALU({A, true}, {C, true}, CP); }
uint8_t SM83::CP_A_D() { return PROCESS_ALU({A, true}, {D, true}, CP); }
uint8_t SM83::CP_A_E() { return PROCESS_ALU({A, true}, {E, true}, CP); }
uint8_t SM83::CP_A_H() { return PROCESS_ALU({A, true}, {H, true}, CP); }
uint8_t SM83::CP_A_L() { return PROCESS_ALU({A, true}, {L, true}, CP); }
uint8_t SM83::CP_A_aHL() { return PROCESS_ALU({A, true}, {HL, false}, CP); }
uint8_t SM83::CP_A_A() { return PROCESS_ALU({A, true}, {A, true}, CP); }
uint8_t SM83::RET_NZ() { return 0; }
uint8_t SM83::POP_BC() { return POP({BC, true}); }
uint8_t SM83::JP_NZ_a16() { return 0; }
uint8_t SM83::JP_a16() { return 0; }
uint8_t SM83::CALL_NZ_a16() { return 0; }
uint8_t SM83::PUSH_BC() { return PUSH({BC, true}); }
uint8_t SM83::ADD_A_n8() { return PROCESS_ALU({A, true}, {N8, true}, ADD); }
uint8_t SM83::RST_00() { return 0; }
uint8_t SM83::RET_Z() { return 0; }
uint8_t SM83::RET() { return 0; }
uint8_t SM83::JP_Z_a16() { return 0; }
uint8_t SM83::PREFIX() { return 0; }
uint8_t SM83::CALL_Z_a16() { return 0; }
uint8_t SM83::CALL_a16() { return 0; }
uint8_t SM83::ADC_A_n8() { return PROCESS_ALU({A, true}, {N8, true}, ADC); }
uint8_t SM83::RST_08() { return 0; }
uint8_t SM83::RET_NC() { return 0; }
uint8_t SM83::POP_DE() { return POP({DE, true}); }
uint8_t SM83::JP_NC_a16() { return 0; }
uint8_t SM83::ILLEGAL_D3() { return 0; }
uint8_t SM83::CALL_NC_a16() { return 0; }
uint8_t SM83::PUSH_DE() { return PUSH({DE, true}); }
uint8_t SM83::SUB_A_n8() { return PROCESS_ALU({A, true}, {N8, true}, SUB); }
uint8_t SM83::RST_10() { return 0; }
uint8_t SM83::RET_C() { return 0; }
uint8_t SM83::RETI() { return 0; }
uint8_t SM83::JP_C_a16() { return 0; }
uint8_t SM83::ILLEGAL_DB() { return 0; }
uint8_t SM83::CALL_C_a16() { return 0; }
uint8_t SM83::ILLEGAL_DD() { return 0; }
uint8_t SM83::SBC_A_n8() { return PROCESS_ALU({A, true}, {N8, true}, SBC); }
uint8_t SM83::RST_18() { return 0; }
uint8_t SM83::LDH_aa8_A() { return LDH({A8, false}, {A, true}); }
uint8_t SM83::POP_HL() { return POP({HL, true}); }
uint8_t SM83::LDH_aC_A() { return LDH({C, false}, {A, true}); }
uint8_t SM83::ILLEGAL_E3() { return 0; }
uint8_t SM83::ILLEGAL_E4() { return 0; }
uint8_t SM83::PUSH_HL() { return PUSH({HL, true}); }
uint8_t SM83::AND_A_n8() { return PROCESS_ALU({A, true}, {N8, true}, AND); }
uint8_t SM83::RST_20() { return 0; }
uint8_t SM83::ADD_SP_e8() { return PROCESS_ALU16({SP, true}, {E8, true}, ADD); }
uint8_t SM83::JP_HL() { return 0; }
uint8_t SM83::LD_aa16_A() { return LD({A16, false}, {A, true}); }
uint8_t SM83::ILLEGAL_EB() { return 0; }
uint8_t SM83::ILLEGAL_EC() { return 0; }
uint8_t SM83::ILLEGAL_ED() { return 0; }
uint8_t SM83::XOR_A_n8() { return PROCESS_ALU({A, true}, {N8, true}, XOR); }
uint8_t SM83::RST_28() { return 0; }
uint8_t SM83::LDH_A_aa8() { return LDH({A, true}, {A8, false}); }
uint8_t SM83::POP_AF() { return POP({AF, true}); }
uint8_t SM83::LDH_A_aC() { return LDH({A, true}, {C, false}); }
uint8_t SM83::DI() { return 0; }
uint8_t SM83::ILLEGAL_F4() { return 0; }
uint8_t SM83::PUSH_AF() { return PUSH({AF, true}); }
uint8_t SM83::OR_A_n8() { return PROCESS_ALU({A, true}, {N8, true}, OR); }
uint8_t SM83::RST_30() { return 0; }
uint8_t SM83::LD_HL_SPI_e8() { return LD_HL_SPDD(); }
uint8_t SM83::LD_SP_HL() { return LD16({SP, true}, {HL, true}); }
uint8_t SM83::LD_A_aa16() { return LD({A, true}, {A16, false}); }
uint8_t SM83::EI() { return 0; }
uint8_t SM83::ILLEGAL_FC() { return 0; }
uint8_t SM83::ILLEGAL_FD() { return 0; }
uint8_t SM83::CP_A_n8() { return PROCESS_ALU({A, true}, {N8, true}, CP); }
uint8_t SM83::RST_38() { return 0; }
