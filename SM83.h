#pragma once
#include "definitions.h"
#include <vector>
#include <cstdint>
#include <string>

// #define DEBUGMODE_

#ifdef DEBUGMODE_
// #define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <sstream>
#include <iomanip>
#endif 

class Bus;

class SM83 {

public:
    SM83();
    ~SM83();

    #ifdef DEBUGMODE_
    SDL_Window *registers_debug_window = nullptr;
    SDL_Renderer *registers_debug_renderer = nullptr;
    TTF_Font* sdl_ttf_font;
    SDL_Color sdl_color_white = {255, 255, 255, 255};
    SDL_Color sdl_color_black = {0, 0, 0, 255};
    void drawDebug();
    void opDebug();
    #endif

public:
    Bus *bus = nullptr;

public: 

    // CPU Registers
    uint8_t   a  = 0x01;
    uint8_t   f  = 0x00;
    uint8_t   b  = 0x00;
    uint8_t   c  = 0x13;
    uint8_t   d  = 0x00;
    uint8_t   e  = 0xD8;
    uint8_t   h  = 0x01;
    uint8_t   l  = 0x4D;
    uint16_t  sp = 0xFFFE;
    uint16_t  pc = 0x0100;

    // for debugging
    uint16_t last_executed_pc = 0x0000;

    // I'm sure therees a more clever way to achieve it, but personally im just maintaining separate variables
    // for two byte register addressing, and i'm going to have a function which syncs the hi lo and hilo registers
    // with each other.
    
    uint16_t af = 0x0000;
    uint16_t bc = 0x0000;
    uint16_t de = 0x0000;
    uint16_t hl = 0x0000;

    enum InterruptFlags {
        Joypad = 1 << 4,
        Serial = 1 << 3,
        Timer = 1 << 2,
        Stat = 1 << 1,
        VBlank = 1 << 0,
    };

    int8_t ime = 1; // global interrupt flag (not memory mapped) handled by DI and EI instructions
    // 1 is true, 0 is false. if ime > 1, then after every clock() function, the CPU will decrement ime
    uint8_t ie = 0x00 ; // interrupts enabled flags
    uint8_t if_ = 0xE1 ; // interrupts requested flags

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
    
    enum SM83Flags {
        fz = 1 << 7,
        fn = 1 << 6,
        fh = 1 << 5,
        fc = 1 << 4,
    };
     

    void setFlag(SM83Flags flag, bool value) {
        if (value) {
            f |= flag;
        } else {
            f &= ~flag;
        }
    }

    uint8_t getFlag(SM83Flags flag) {
        return (f & flag > 0) ? 1 : 0;
    }

    void requestInterrupt(InterruptFlags flag) {
        if_ |= flag;
    }

    void handleInterrupts();

    void connectBus(Bus *n) { bus = n; }

    // void boot();

    void clock();

private:
    
    // to facilitate interaction with the bus
    uint8_t fetched = 0x00;
    uint16_t fetched16 = 0x0000;
    uint16_t addr_abs = 0x0000; // if an operand is [a16] then i deal with it using this variable manually 
    //uint8_t *target_register = nullptr; // for LDR instruction
    uint8_t cycles = 0;
    uint8_t opcode = 0;
    std::vector<std::string> unprefixed_instructions_names;
    std::string last_instruction = "NOP";
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
        A, F, B, C, D, E, H, L, AF, BC, DE, HL, SP, PC, N8, N16, A8, A16, E8, 
        NZ, NC, Z, // for the jump conditions
    };

    enum ALUOperation {
        ADD, ADC, SUB, SBC, XOR, AND, OR, CP, INC, DEC, enumCCF, enumCPL, enumSCF, enumDAA, 
    };

    struct Operand {
        OperandName name;
        bool immediate;
        bool decrement = false;
        bool increment = false;
    };

    std::vector<Instruction> unprefixed_opcode_lookup;
    std::vector<Instruction> prefixed_opcode_lookup;

    uint8_t* process_operand(Operand operand);
    uint16_t* process_operand16(Operand operand);

    // instruction operations/wrappers
    // uint8_t NOP(); uint8_t LDBC(); uint8_t LDmBC(); uint8_t INCBC(); uint8_t INCB(); uint8_t DECB(); uint8_t LDB(); uint8_t RLCA();
    // uint8_t LDaddr(); uint8_t ADDHL(); uint8_t LDA(); uint8_t

    // UNPREFIXED
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

    // PREFIXED
    uint8_t RLC_B(); uint8_t RLC_C(); uint8_t RLC_D(); uint8_t RLC_E(); uint8_t RLC_H(); uint8_t RLC_L(); uint8_t RLC_aHL();
    uint8_t RLC_A(); uint8_t RRC_B(); uint8_t RRC_C(); uint8_t RRC_D(); uint8_t RRC_E(); uint8_t RRC_H(); uint8_t RRC_L();
    uint8_t RRC_aHL(); uint8_t RRC_A(); uint8_t RL_B(); uint8_t RL_C(); uint8_t RL_D(); uint8_t RL_E(); uint8_t RL_H();
    uint8_t RL_L(); uint8_t RL_aHL(); uint8_t RL_A(); uint8_t RR_B(); uint8_t RR_C(); uint8_t RR_D(); uint8_t RR_E();
    uint8_t RR_H(); uint8_t RR_L(); uint8_t RR_aHL(); uint8_t RR_A(); uint8_t SLA_B(); uint8_t SLA_C(); uint8_t SLA_D();
    uint8_t SLA_E(); uint8_t SLA_H(); uint8_t SLA_L(); uint8_t SLA_aHL(); uint8_t SLA_A(); uint8_t SRA_B(); uint8_t SRA_C();
    uint8_t SRA_D(); uint8_t SRA_E(); uint8_t SRA_H(); uint8_t SRA_L(); uint8_t SRA_aHL(); uint8_t SRA_A(); uint8_t SWAP_B();
    uint8_t SWAP_C(); uint8_t SWAP_D(); uint8_t SWAP_E(); uint8_t SWAP_H(); uint8_t SWAP_L(); uint8_t SWAP_aHL(); uint8_t SWAP_A();
    uint8_t SRL_B(); uint8_t SRL_C(); uint8_t SRL_D(); uint8_t SRL_E(); uint8_t SRL_H(); uint8_t SRL_L(); uint8_t SRL_aHL();
    uint8_t SRL_A(); uint8_t BIT_0_B(); uint8_t BIT_0_C(); uint8_t BIT_0_D(); uint8_t BIT_0_E(); uint8_t BIT_0_H(); uint8_t BIT_0_L();
    uint8_t BIT_0_aHL(); uint8_t BIT_0_A(); uint8_t BIT_1_B(); uint8_t BIT_1_C(); uint8_t BIT_1_D(); uint8_t BIT_1_E(); uint8_t BIT_1_H();
    uint8_t BIT_1_L(); uint8_t BIT_1_aHL(); uint8_t BIT_1_A(); uint8_t BIT_2_B(); uint8_t BIT_2_C(); uint8_t BIT_2_D(); uint8_t BIT_2_E();
    uint8_t BIT_2_H(); uint8_t BIT_2_L(); uint8_t BIT_2_aHL(); uint8_t BIT_2_A(); uint8_t BIT_3_B(); uint8_t BIT_3_C(); uint8_t BIT_3_D();
    uint8_t BIT_3_E(); uint8_t BIT_3_H(); uint8_t BIT_3_L(); uint8_t BIT_3_aHL(); uint8_t BIT_3_A(); uint8_t BIT_4_B(); uint8_t BIT_4_C();
    uint8_t BIT_4_D(); uint8_t BIT_4_E(); uint8_t BIT_4_H(); uint8_t BIT_4_L(); uint8_t BIT_4_aHL(); uint8_t BIT_4_A(); uint8_t BIT_5_B();
    uint8_t BIT_5_C(); uint8_t BIT_5_D(); uint8_t BIT_5_E(); uint8_t BIT_5_H(); uint8_t BIT_5_L(); uint8_t BIT_5_aHL(); uint8_t BIT_5_A();
    uint8_t BIT_6_B(); uint8_t BIT_6_C(); uint8_t BIT_6_D(); uint8_t BIT_6_E(); uint8_t BIT_6_H(); uint8_t BIT_6_L(); uint8_t BIT_6_aHL();
    uint8_t BIT_6_A(); uint8_t BIT_7_B(); uint8_t BIT_7_C(); uint8_t BIT_7_D(); uint8_t BIT_7_E(); uint8_t BIT_7_H(); uint8_t BIT_7_L();
    uint8_t BIT_7_aHL(); uint8_t BIT_7_A(); uint8_t RES_0_B(); uint8_t RES_0_C(); uint8_t RES_0_D(); uint8_t RES_0_E(); uint8_t RES_0_H();
    uint8_t RES_0_L(); uint8_t RES_0_aHL(); uint8_t RES_0_A(); uint8_t RES_1_B(); uint8_t RES_1_C(); uint8_t RES_1_D(); uint8_t RES_1_E();
    uint8_t RES_1_H(); uint8_t RES_1_L(); uint8_t RES_1_aHL(); uint8_t RES_1_A(); uint8_t RES_2_B(); uint8_t RES_2_C(); uint8_t RES_2_D();
    uint8_t RES_2_E(); uint8_t RES_2_H(); uint8_t RES_2_L(); uint8_t RES_2_aHL(); uint8_t RES_2_A(); uint8_t RES_3_B(); uint8_t RES_3_C();
    uint8_t RES_3_D(); uint8_t RES_3_E(); uint8_t RES_3_H(); uint8_t RES_3_L(); uint8_t RES_3_aHL(); uint8_t RES_3_A(); uint8_t RES_4_B();
    uint8_t RES_4_C(); uint8_t RES_4_D(); uint8_t RES_4_E(); uint8_t RES_4_H(); uint8_t RES_4_L(); uint8_t RES_4_aHL(); uint8_t RES_4_A();
    uint8_t RES_5_B(); uint8_t RES_5_C(); uint8_t RES_5_D(); uint8_t RES_5_E(); uint8_t RES_5_H(); uint8_t RES_5_L(); uint8_t RES_5_aHL();
    uint8_t RES_5_A(); uint8_t RES_6_B(); uint8_t RES_6_C(); uint8_t RES_6_D(); uint8_t RES_6_E(); uint8_t RES_6_H(); uint8_t RES_6_L();
    uint8_t RES_6_aHL(); uint8_t RES_6_A(); uint8_t RES_7_B(); uint8_t RES_7_C(); uint8_t RES_7_D(); uint8_t RES_7_E(); uint8_t RES_7_H();
    uint8_t RES_7_L(); uint8_t RES_7_aHL(); uint8_t RES_7_A(); uint8_t SET_0_B(); uint8_t SET_0_C(); uint8_t SET_0_D(); uint8_t SET_0_E();
    uint8_t SET_0_H(); uint8_t SET_0_L(); uint8_t SET_0_aHL(); uint8_t SET_0_A(); uint8_t SET_1_B(); uint8_t SET_1_C(); uint8_t SET_1_D();
    uint8_t SET_1_E(); uint8_t SET_1_H(); uint8_t SET_1_L(); uint8_t SET_1_aHL(); uint8_t SET_1_A(); uint8_t SET_2_B(); uint8_t SET_2_C();
    uint8_t SET_2_D(); uint8_t SET_2_E(); uint8_t SET_2_H(); uint8_t SET_2_L(); uint8_t SET_2_aHL(); uint8_t SET_2_A(); uint8_t SET_3_B();
    uint8_t SET_3_C(); uint8_t SET_3_D(); uint8_t SET_3_E(); uint8_t SET_3_H(); uint8_t SET_3_L(); uint8_t SET_3_aHL(); uint8_t SET_3_A();
    uint8_t SET_4_B(); uint8_t SET_4_C(); uint8_t SET_4_D(); uint8_t SET_4_E(); uint8_t SET_4_H(); uint8_t SET_4_L(); uint8_t SET_4_aHL();
    uint8_t SET_4_A(); uint8_t SET_5_B(); uint8_t SET_5_C(); uint8_t SET_5_D(); uint8_t SET_5_E(); uint8_t SET_5_H(); uint8_t SET_5_L();
    uint8_t SET_5_aHL(); uint8_t SET_5_A(); uint8_t SET_6_B(); uint8_t SET_6_C(); uint8_t SET_6_D(); uint8_t SET_6_E(); uint8_t SET_6_H();
    uint8_t SET_6_L(); uint8_t SET_6_aHL(); uint8_t SET_6_A(); uint8_t SET_7_B(); uint8_t SET_7_C(); uint8_t SET_7_D(); uint8_t SET_7_E();
    uint8_t SET_7_H(); uint8_t SET_7_L(); uint8_t SET_7_aHL(); uint8_t SET_7_A();


    // instruction wrappers internals;
    uint8_t LD(Operand target, Operand source);
    uint8_t LD16(Operand target, Operand source);
    uint8_t LD_HL_SPDD();
    uint8_t LDH(Operand source, Operand target);
    uint8_t POP(Operand target);
    uint8_t PUSH(Operand target);

    uint8_t PROCESS_ALU(ALUOperation operation);
    uint8_t PROCESS_ALU(Operand target, ALUOperation operation);
    uint8_t PROCESS_ALU(Operand target, Operand source, ALUOperation operation);
    uint8_t PROCESS_ALU16(Operand target, ALUOperation operation);
    uint8_t PROCESS_ALU16(Operand target, Operand source, ALUOperation operation);

    uint8_t JUMPTO(Operand address);
    uint8_t JUMPTO(OperandName condition, Operand address);
    uint8_t CALL(Operand address);
    uint8_t CALL(OperandName condition, Operand address);
    uint8_t RST(uint8_t address_lo);
    uint8_t RETURNFROMFUNCTION();
    uint8_t RETURNFROMFUNCTION(OperandName condition);

    uint8_t ENABLEINTERRUPTS();
    uint8_t DISABLEINTERRUPTS();
    uint8_t RETURNANDEI();

    uint8_t ROTATELEFT(Operand operand, bool reversed=false, bool through_carry=false);
    uint8_t ROTATEREGALEFT(bool reversed=false, bool through_carry=false);
    uint8_t SHIFTLEFT(Operand operand, bool reversed=false, bool is_logical=false);
    uint8_t SWAP(Operand operand);
    uint8_t BIT(uint8_t bit, Operand operand);
    uint8_t RES(uint8_t bit, Operand operand);
    uint8_t SET(uint8_t bit, Operand operand);

private:
    // these are technically core functions, but they are called by wrapping core functions 
    uint8_t ADD8(uint8_t *targetValue, uint8_t *sourceValue, bool isADC=false);
    uint8_t SUB8(uint8_t *targetValue, uint8_t *sourceValue, bool isSBC=false);



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


    #ifdef DEBUGMODE_

    // Function to convert integer to a formatted hex string
    std::string formatHex(int value, int width) {
        std::stringstream ss;
        ss << "0x" << std::hex << std::setfill('0') << std::setw(width) << value;
        return ss.str();
    }

    // Function to render text
    void renderText(SDL_Renderer* renderer, TTF_Font* font, const std::string& text, int x, int y, SDL_Color color) {
        SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

        int w = surface->w;
        int h = surface->h;

        SDL_Rect destRect = {x, y, w, h};
        SDL_RenderCopy(renderer, texture, nullptr, &destRect);

        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
    }

    #endif

};
