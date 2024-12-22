#pragma once
#include <vector>
#include <cstdint>
#include <string>

class Bus;

class SM83 {

public:
    SM83();
    ~SM83();

public:
    Bus *bus = nullptr;

public: 

    // CPU Registers
    uint8_t   a  = 0x00;
    uint8_t   f  = 0x00;
    uint8_t   b  = 0x00;
    uint8_t   c  = 0x00;
    uint8_t   d  = 0x00;
    uint8_t   e  = 0x00;
    uint8_t   h  = 0x00;
    uint8_t   l  = 0x00;
    uint16_t  sp = 0x0000;
    uint16_t  pc = 0x0000;

    // I'm sure therees a more clever way to achieve it, but personally im just maintaining separate variables
    // for two byte register addressing, and i'm going to have a function which syncs the hi lo and hilo registers
    // with each other.
    uint16_t af = 0x0000;
    uint16_t bc = 0x0000;
    uint16_t de = 0x0000;
    uint16_t hl = 0x0000;

    void updateRegisters16() { // this function must be called whenever the uint8_t register vars are affected
        this->af = (this->a << 8) | this->f;
        this->bc = (this->b << 8) | this->c;
        this->de = (this->d << 8) | this->e;
        this->hl = (this->h << 8) | this->l;
    }

    void updateRegisters8() { // this function must be calledd whenever the uint16_t register vars are affected
        this->a = this->af >> 8;
        this->f = this->af & 0x00FF;
        this->b = this->bc >> 8;
        this->c = this->bc & 0x00FF;
        this->d = this->de >> 8;
        this->e = this->de & 0x00FF;
        this->h = this->hl >> 8;
        this->l = this->hl & 0x00FF;
    }

    enum SM83FLAGS {
        fz = 1 << 7,
        fn = 1 << 6,
        fh = 1 << 5,
        fc = 1 << 4,
    };

    void setFlag(SM83FLAGS flag, bool value) {
        if (value) {
            f |= flag;
        } else {
            f &= ~flag;
        }
    }

    uint8_t getFlag(SM83FLAGS flag) {
        return (f & flag > 0) ? 1 : 0;
    }

    void connectBus(Bus *n) { bus = n; }

    void clock();

private:
    
    // to facilitate interaction with the bus
    uint8_t fetched = 0x00;
    uint16_t fetched16 = 0x0000;
    uint16_t addr_abs = 0x0000; // if an operand is [a16] then i deal with it using this variable manually 
    //uint8_t *target_register = nullptr; // for LDR instruction
    uint16_t temp = 0x0000;
    uint8_t read(uint16_t addr);
    uint8_t* readPttr(uint16_t addr);
    void write(uint16_t addr, uint8_t data);

    // Instructions
    struct Instruction {
        std::string name;
        //uint8_t (*operand1addrmode)(void) = nullptr;
        //uint8_t (*operand2addrmode)(void) = nullptr;
        uint8_t (SM83::*operate)(void) = nullptr;
        uint8_t cycles;
    };

    enum OperandName {
        A, F, B, C, D, E, H, L, AF, BC, DE, HL, SP, PC, D8, D16, N8, N16, A8, A16,
    };

    struct Operand {
        OperandName name;
        bool immediate;
        bool decrement = false;
        bool increment = false;
    };

    std::vector<Instruction> unprefixed_opcode_lookup;

    // addressing modes
//    uint8_t IMM(); uint8_t DRT(); uint8_t 
    uint8_t* process_operand(Operand operand);
    uint16_t* process_operand16(Operand operand);

    // instruction operations/wrappers
    //uint8_t NOP(); uint8_t LDBC(); uint8_t LDmBC(); uint8_t INCBC(); uint8_t INCB(); uint8_t DECB(); uint8_t LDB(); uint8_t RLCA();
    //uint8_t LDaddr(); uint8_t ADDHL(); uint8_t LDA(); uint8_t 
    uint8_t NOP(); uint8_t LD_BC_n16(); uint8_t LD_aBC_A(); uint8_t INC_BC(); uint8_t INC_B(); uint8_t DEC_B(); uint8_t LD_B_n8();
    uint8_t RLCA(); uint8_t LD_aa16_SP(); uint8_t ADD_HL_BC(); uint8_t LD_A_aBC(); uint8_t DEC_BC(); uint8_t INC_C(); uint8_t DEC_C();
    uint8_t LD_C_n8(); uint8_t RRCA(); uint8_t STOP_n8(); uint8_t LD_DE_n16(); uint8_t LD_aDE_A(); uint8_t INC_DE(); uint8_t INC_D();
    uint8_t DEC_D(); uint8_t LD_D_n8(); uint8_t RLA(); uint8_t JR_e8(); uint8_t ADD_HL_DE(); uint8_t LD_A_aDE(); uint8_t DEC_DE();
    uint8_t INC_E(); uint8_t DEC_E(); uint8_t LD_E_n8(); uint8_t RRA(); uint8_t JR_NZ_e8(); uint8_t LD_HL_n16(); uint8_t LD_aHLI_A();
    uint8_t INC_HL(); uint8_t INC_H(); uint8_t DEC_H(); uint8_t LD_H_n8(); uint8_t DAA(); uint8_t JR_Z_e8(); uint8_t ADD_HL_HL();
    uint8_t LD_A_aHLI(); uint8_t DEC_HL(); uint8_t INC_L(); uint8_t DEC_L(); uint8_t LD_L_n8(); uint8_t CPL(); uint8_t JR_NC_e8();
    uint8_t LD_SP_n16(); uint8_t LD_aHLD_A(); uint8_t INC_SP(); uint8_t INC_aHL(); uint8_t DEC_aHL(); uint8_t LD_aHL_n8(); uint8_t SCF();
    uint8_t JR_C_e8(); uint8_t ADD_HL_SP(); uint8_t LD_A_aHLD(); uint8_t DEC_SP(); uint8_t INC_A(); uint8_t DEC_A(); uint8_t LD_A_n8();
    uint8_t CCF(); uint8_t LD_B_B(); uint8_t LD_B_C(); uint8_t LD_B_D(); uint8_t LD_B_E(); uint8_t LD_B_H(); uint8_t LD_B_L();
    uint8_t LD_B_aHL(); uint8_t LD_B_A(); uint8_t LD_C_B(); uint8_t LD_C_C(); uint8_t LD_C_D(); uint8_t LD_C_E(); uint8_t LD_C_H();
    uint8_t LD_C_L(); uint8_t LD_C_aHL(); uint8_t LD_C_A(); uint8_t LD_D_B(); uint8_t LD_D_C(); uint8_t LD_D_D(); uint8_t LD_D_E();
    uint8_t LD_D_H(); uint8_t LD_D_L(); uint8_t LD_D_aHL(); uint8_t LD_D_A(); uint8_t LD_E_B(); uint8_t LD_E_C(); uint8_t LD_E_D();
    uint8_t LD_E_E(); uint8_t LD_E_H(); uint8_t LD_E_L(); uint8_t LD_E_aHL(); uint8_t LD_E_A(); uint8_t LD_H_B(); uint8_t LD_H_C();
    uint8_t LD_H_D(); uint8_t LD_H_E(); uint8_t LD_H_H(); uint8_t LD_H_L(); uint8_t LD_H_aHL(); uint8_t LD_H_A(); uint8_t LD_L_B();
    uint8_t LD_L_C(); uint8_t LD_L_D(); uint8_t LD_L_E(); uint8_t LD_L_H(); uint8_t LD_L_L(); uint8_t LD_L_aHL(); uint8_t LD_L_A();
    uint8_t LD_aHL_B(); uint8_t LD_aHL_C(); uint8_t LD_aHL_D(); uint8_t LD_aHL_E(); uint8_t LD_aHL_H(); uint8_t LD_aHL_L(); uint8_t HALT();
    uint8_t LD_aHL_A(); uint8_t LD_A_B(); uint8_t LD_A_C(); uint8_t LD_A_D(); uint8_t LD_A_E(); uint8_t LD_A_H(); uint8_t LD_A_L();
    uint8_t LD_A_aHL(); uint8_t LD_A_A(); uint8_t ADD_A_B(); uint8_t ADD_A_C(); uint8_t ADD_A_D(); uint8_t ADD_A_E(); uint8_t ADD_A_H();
    uint8_t ADD_A_L(); uint8_t ADD_A_aHL(); uint8_t ADD_A_A(); uint8_t ADC_A_B(); uint8_t ADC_A_C(); uint8_t ADC_A_D(); uint8_t ADC_A_E();
    uint8_t ADC_A_H(); uint8_t ADC_A_L(); uint8_t ADC_A_aHL(); uint8_t ADC_A_A(); uint8_t SUB_A_B(); uint8_t SUB_A_C(); uint8_t SUB_A_D();
    uint8_t SUB_A_E(); uint8_t SUB_A_H(); uint8_t SUB_A_L(); uint8_t SUB_A_aHL(); uint8_t SUB_A_A(); uint8_t SBC_A_B(); uint8_t SBC_A_C();
    uint8_t SBC_A_D(); uint8_t SBC_A_E(); uint8_t SBC_A_H(); uint8_t SBC_A_L(); uint8_t SBC_A_aHL(); uint8_t SBC_A_A(); uint8_t AND_A_B();
    uint8_t AND_A_C(); uint8_t AND_A_D(); uint8_t AND_A_E(); uint8_t AND_A_H(); uint8_t AND_A_L(); uint8_t AND_A_aHL(); uint8_t AND_A_A();
    uint8_t XOR_A_B(); uint8_t XOR_A_C(); uint8_t XOR_A_D(); uint8_t XOR_A_E(); uint8_t XOR_A_H(); uint8_t XOR_A_L(); uint8_t XOR_A_aHL();
    uint8_t XOR_A_A(); uint8_t OR_A_B(); uint8_t OR_A_C(); uint8_t OR_A_D(); uint8_t OR_A_E(); uint8_t OR_A_H(); uint8_t OR_A_L();
    uint8_t OR_A_aHL(); uint8_t OR_A_A(); uint8_t CP_A_B(); uint8_t CP_A_C(); uint8_t CP_A_D(); uint8_t CP_A_E(); uint8_t CP_A_H();
    uint8_t CP_A_L(); uint8_t CP_A_aHL(); uint8_t CP_A_A(); uint8_t RET_NZ(); uint8_t POP_BC(); uint8_t JP_NZ_a16(); uint8_t JP_a16();
    uint8_t CALL_NZ_a16(); uint8_t PUSH_BC(); uint8_t ADD_A_n8(); uint8_t RST_00(); uint8_t RET_Z(); uint8_t RET(); uint8_t JP_Z_a16();
    uint8_t PREFIX(); uint8_t CALL_Z_a16(); uint8_t CALL_a16(); uint8_t ADC_A_n8(); uint8_t RST_08(); uint8_t RET_NC(); uint8_t POP_DE();
    uint8_t JP_NC_a16(); uint8_t ILLEGAL_D3(); uint8_t CALL_NC_a16(); uint8_t PUSH_DE(); uint8_t SUB_A_n8(); uint8_t RST_10(); uint8_t RET_C();
    uint8_t RETI(); uint8_t JP_C_a16(); uint8_t ILLEGAL_DB(); uint8_t CALL_C_a16(); uint8_t ILLEGAL_DD(); uint8_t SBC_A_n8(); uint8_t RST_18();
    uint8_t LDH_aa8_A(); uint8_t POP_HL(); uint8_t LDH_aC_A(); uint8_t ILLEGAL_E3(); uint8_t ILLEGAL_E4(); uint8_t PUSH_HL(); uint8_t AND_A_n8();
    uint8_t RST_20(); uint8_t ADD_SP_e8(); uint8_t JP_HL(); uint8_t LD_aa16_A(); uint8_t ILLEGAL_EB(); uint8_t ILLEGAL_EC(); uint8_t ILLEGAL_ED();
    uint8_t XOR_A_n8(); uint8_t RST_28(); uint8_t LDH_A_aa8(); uint8_t POP_AF(); uint8_t LDH_A_aC(); uint8_t DI(); uint8_t ILLEGAL_F4();
    uint8_t PUSH_AF(); uint8_t OR_A_n8(); uint8_t RST_30(); uint8_t LD_HL_SPI_e8(); uint8_t LD_SP_HL(); uint8_t LD_A_aa16(); uint8_t EI();
    uint8_t ILLEGAL_FC(); uint8_t ILLEGAL_FD(); uint8_t CP_A_n8(); uint8_t RST_38(); 


    // instruction wrappers internals;
    uint8_t LD(Operand target, Operand source);
    uint8_t LD16(Operand target, Operand source);
    uint8_t LD_HL_SPDD();
    uint8_t POP(Operand target);
    uint8_t PUSH(Operand target);

private:

    // useful utility functions
    void addToHiLo(uint8_t &hi, uint8_t &lo, int8_t to_add) {
        uint16_t result = (hi << 8) | (lo);
        result += to_add;
        hi = result >> 8;
        lo = result & 0xff;
    }

    int8_t toSigned(uint8_t b) {
        return (b > 127) ? (b - 256) : b;
    }

};
