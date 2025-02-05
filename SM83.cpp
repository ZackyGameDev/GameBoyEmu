#include "SM83.h"
#include "Bus.h"
#include <iostream>
#include <iomanip>

#ifdef DEBUGMODE_
#include<algorithm>
#endif

SM83::SM83() {
    updateRegisters16();
    using x = SM83;

    unprefixed_opcode_lookup = { 
        {"NOP", &x::NOP, 1}, {"LD", &x::LD_BC_n16, 3}, {"LD", &x::LD_aBC_A, 2}, {"INC", &x::INC_BC, 2}, {"INC", &x::INC_B, 1},
        {"DEC", &x::DEC_B, 1}, {"LD", &x::LD_B_n8, 2}, {"RLCA", &x::RLCA, 1}, {"LD", &x::LD_aa16_SP, 5}, {"ADD", &x::ADD_HL_BC, 2},
        {"LD", &x::LD_A_aBC, 2}, {"DEC", &x::DEC_BC, 2}, {"INC", &x::INC_C, 1}, {"DEC", &x::DEC_C, 1}, {"LD", &x::LD_C_n8, 2},
        {"RRCA", &x::RRCA, 1}, {"STOP", &x::STOP_n8, 1}, {"LD", &x::LD_DE_n16, 3}, {"LD", &x::LD_aDE_A, 2}, {"INC", &x::INC_DE, 2},
        {"INC", &x::INC_D, 1}, {"DEC", &x::DEC_D, 1}, {"LD", &x::LD_D_n8, 2}, {"RLA", &x::RLA, 1}, {"JR", &x::JR_e8, 3},
        {"ADD", &x::ADD_HL_DE, 2}, {"LD", &x::LD_A_aDE, 2}, {"DEC", &x::DEC_DE, 2}, {"INC", &x::INC_E, 1}, {"DEC", &x::DEC_E, 1},
        {"LD", &x::LD_E_n8, 2}, {"RRA", &x::RRA, 1}, {"JR", &x::JR_NZ_e8, 2}, {"LD", &x::LD_HL_n16, 3}, {"LD", &x::LD_aHLI_A, 2},
        {"INC", &x::INC_HL, 2}, {"INC", &x::INC_H, 1}, {"DEC", &x::DEC_H, 1}, {"LD", &x::LD_H_n8, 2}, {"DAA", &x::DAA, 1},
        {"JR", &x::JR_Z_e8, 2}, {"ADD", &x::ADD_HL_HL, 2}, {"LD", &x::LD_A_aHLI, 2}, {"DEC", &x::DEC_HL, 2}, {"INC", &x::INC_L, 1},
        {"DEC", &x::DEC_L, 1}, {"LD", &x::LD_L_n8, 2}, {"CPL", &x::CPL, 1}, {"JR", &x::JR_NC_e8, 2}, {"LD", &x::LD_SP_n16, 3},
        {"LD", &x::LD_aHLD_A, 2}, {"INC", &x::INC_SP, 2}, {"INC", &x::INC_aHL, 3}, {"DEC", &x::DEC_aHL, 3}, {"LD", &x::LD_aHL_n8, 3},
        {"SCF", &x::SCF, 1}, {"JR", &x::JR_C_e8, 2}, {"ADD", &x::ADD_HL_SP, 2}, {"LD", &x::LD_A_aHLD, 2}, {"DEC", &x::DEC_SP, 2},
        {"INC", &x::INC_A, 1}, {"DEC", &x::DEC_A, 1}, {"LD", &x::LD_A_n8, 2}, {"CCF", &x::CCF, 1}, {"LD", &x::LD_B_B, 1},
        {"LD", &x::LD_B_C, 1}, {"LD", &x::LD_B_D, 1}, {"LD", &x::LD_B_E, 1}, {"LD", &x::LD_B_H, 1}, {"LD", &x::LD_B_L, 1},
        {"LD", &x::LD_B_aHL, 2}, {"LD", &x::LD_B_A, 1}, {"LD", &x::LD_C_B, 1}, {"LD", &x::LD_C_C, 1}, {"LD", &x::LD_C_D, 1},
        {"LD", &x::LD_C_E, 1}, {"LD", &x::LD_C_H, 1}, {"LD", &x::LD_C_L, 1}, {"LD", &x::LD_C_aHL, 2}, {"LD", &x::LD_C_A, 1},
        {"LD", &x::LD_D_B, 1}, {"LD", &x::LD_D_C, 1}, {"LD", &x::LD_D_D, 1}, {"LD", &x::LD_D_E, 1}, {"LD", &x::LD_D_H, 1},
        {"LD", &x::LD_D_L, 1}, {"LD", &x::LD_D_aHL, 2}, {"LD", &x::LD_D_A, 1}, {"LD", &x::LD_E_B, 1}, {"LD", &x::LD_E_C, 1},
        {"LD", &x::LD_E_D, 1}, {"LD", &x::LD_E_E, 1}, {"LD", &x::LD_E_H, 1}, {"LD", &x::LD_E_L, 1}, {"LD", &x::LD_E_aHL, 2},
        {"LD", &x::LD_E_A, 1}, {"LD", &x::LD_H_B, 1}, {"LD", &x::LD_H_C, 1}, {"LD", &x::LD_H_D, 1}, {"LD", &x::LD_H_E, 1},
        {"LD", &x::LD_H_H, 1}, {"LD", &x::LD_H_L, 1}, {"LD", &x::LD_H_aHL, 2}, {"LD", &x::LD_H_A, 1}, {"LD", &x::LD_L_B, 1},
        {"LD", &x::LD_L_C, 1}, {"LD", &x::LD_L_D, 1}, {"LD", &x::LD_L_E, 1}, {"LD", &x::LD_L_H, 1}, {"LD", &x::LD_L_L, 1},
        {"LD", &x::LD_L_aHL, 2}, {"LD", &x::LD_L_A, 1}, {"LD", &x::LD_aHL_B, 2}, {"LD", &x::LD_aHL_C, 2}, {"LD", &x::LD_aHL_D, 2},
        {"LD", &x::LD_aHL_E, 2}, {"LD", &x::LD_aHL_H, 2}, {"LD", &x::LD_aHL_L, 2}, {"HALT", &x::HALT, 1}, {"LD", &x::LD_aHL_A, 2},
        {"LD", &x::LD_A_B, 1}, {"LD", &x::LD_A_C, 1}, {"LD", &x::LD_A_D, 1}, {"LD", &x::LD_A_E, 1}, {"LD", &x::LD_A_H, 1},
        {"LD", &x::LD_A_L, 1}, {"LD", &x::LD_A_aHL, 2}, {"LD", &x::LD_A_A, 1}, {"ADD", &x::ADD_A_B, 1}, {"ADD", &x::ADD_A_C, 1},
        {"ADD", &x::ADD_A_D, 1}, {"ADD", &x::ADD_A_E, 1}, {"ADD", &x::ADD_A_H, 1}, {"ADD", &x::ADD_A_L, 1}, {"ADD", &x::ADD_A_aHL, 2},
        {"ADD", &x::ADD_A_A, 1}, {"ADC", &x::ADC_A_B, 1}, {"ADC", &x::ADC_A_C, 1}, {"ADC", &x::ADC_A_D, 1}, {"ADC", &x::ADC_A_E, 1},
        {"ADC", &x::ADC_A_H, 1}, {"ADC", &x::ADC_A_L, 1}, {"ADC", &x::ADC_A_aHL, 2}, {"ADC", &x::ADC_A_A, 1}, {"SUB", &x::SUB_A_B, 1},
        {"SUB", &x::SUB_A_C, 1}, {"SUB", &x::SUB_A_D, 1}, {"SUB", &x::SUB_A_E, 1}, {"SUB", &x::SUB_A_H, 1}, {"SUB", &x::SUB_A_L, 1},
        {"SUB", &x::SUB_A_aHL, 2}, {"SUB", &x::SUB_A_A, 1}, {"SBC", &x::SBC_A_B, 1}, {"SBC", &x::SBC_A_C, 1}, {"SBC", &x::SBC_A_D, 1},
        {"SBC", &x::SBC_A_E, 1}, {"SBC", &x::SBC_A_H, 1}, {"SBC", &x::SBC_A_L, 1}, {"SBC", &x::SBC_A_aHL, 2}, {"SBC", &x::SBC_A_A, 1},
        {"AND", &x::AND_A_B, 1}, {"AND", &x::AND_A_C, 1}, {"AND", &x::AND_A_D, 1}, {"AND", &x::AND_A_E, 1}, {"AND", &x::AND_A_H, 1},
        {"AND", &x::AND_A_L, 1}, {"AND", &x::AND_A_aHL, 2}, {"AND", &x::AND_A_A, 1}, {"XOR", &x::XOR_A_B, 1}, {"XOR", &x::XOR_A_C, 1},
        {"XOR", &x::XOR_A_D, 1}, {"XOR", &x::XOR_A_E, 1}, {"XOR", &x::XOR_A_H, 1}, {"XOR", &x::XOR_A_L, 1}, {"XOR", &x::XOR_A_aHL, 2},
        {"XOR", &x::XOR_A_A, 1}, {"OR", &x::OR_A_B, 1}, {"OR", &x::OR_A_C, 1}, {"OR", &x::OR_A_D, 1}, {"OR", &x::OR_A_E, 1},
        {"OR", &x::OR_A_H, 1}, {"OR", &x::OR_A_L, 1}, {"OR", &x::OR_A_aHL, 2}, {"OR", &x::OR_A_A, 1}, {"CP", &x::CP_A_B, 1},
        {"CP", &x::CP_A_C, 1}, {"CP", &x::CP_A_D, 1}, {"CP", &x::CP_A_E, 1}, {"CP", &x::CP_A_H, 1}, {"CP", &x::CP_A_L, 1},
        {"CP", &x::CP_A_aHL, 2}, {"CP", &x::CP_A_A, 1}, {"RET", &x::RET_NZ, 2}, {"POP", &x::POP_BC, 3}, {"JP", &x::JP_NZ_a16, 3},
        {"JP", &x::JP_a16, 4}, {"CALL", &x::CALL_NZ_a16, 3}, {"PUSH", &x::PUSH_BC, 4}, {"ADD", &x::ADD_A_n8, 2}, {"RST", &x::RST_00, 4},
        {"RET", &x::RET_Z, 2}, {"RET", &x::RET, 4}, {"JP", &x::JP_Z_a16, 3}, {"PREFIX", &x::PREFIX, 1}, {"CALL", &x::CALL_Z_a16, 3},
        {"CALL", &x::CALL_a16, 6}, {"ADC", &x::ADC_A_n8, 2}, {"RST", &x::RST_08, 4}, {"RET", &x::RET_NC, 2}, {"POP", &x::POP_DE, 3},
        {"JP", &x::JP_NC_a16, 3}, {"ILLEGAL_D3", &x::ILLEGAL_D3, 1}, {"CALL", &x::CALL_NC_a16, 3}, {"PUSH", &x::PUSH_DE, 4}, {"SUB", &x::SUB_A_n8, 2},
        {"RST", &x::RST_10, 4}, {"RET", &x::RET_C, 2}, {"RETI", &x::RETI, 4}, {"JP", &x::JP_C_a16, 3}, {"ILLEGAL_DB", &x::ILLEGAL_DB, 1},
        {"CALL", &x::CALL_C_a16, 3}, {"ILLEGAL_DD", &x::ILLEGAL_DD, 1}, {"SBC", &x::SBC_A_n8, 2}, {"RST", &x::RST_18, 4}, {"LDH", &x::LDH_aa8_A, 3},
        {"POP", &x::POP_HL, 3}, {"LDH", &x::LDH_aC_A, 2}, {"ILLEGAL_E3", &x::ILLEGAL_E3, 1}, {"ILLEGAL_E4", &x::ILLEGAL_E4, 1}, {"PUSH", &x::PUSH_HL, 4},
        {"AND", &x::AND_A_n8, 2}, {"RST", &x::RST_20, 4}, {"ADD", &x::ADD_SP_e8, 4}, {"JP", &x::JP_HL, 1}, {"LD", &x::LD_aa16_A, 4},
        {"ILLEGAL_EB", &x::ILLEGAL_EB, 1}, {"ILLEGAL_EC", &x::ILLEGAL_EC, 1}, {"ILLEGAL_ED", &x::ILLEGAL_ED, 1}, {"XOR", &x::XOR_A_n8, 2}, {"RST", &x::RST_28, 4},
        {"LDH", &x::LDH_A_aa8, 3}, {"POP", &x::POP_AF, 3}, {"LDH", &x::LDH_A_aC, 2}, {"DI", &x::DI, 1}, {"ILLEGAL_F4", &x::ILLEGAL_F4, 1},
        {"PUSH", &x::PUSH_AF, 4}, {"OR", &x::OR_A_n8, 2}, {"RST", &x::RST_30, 4}, {"LD", &x::LD_HL_SPI_e8, 3}, {"LD", &x::LD_SP_HL, 2},
        {"LD", &x::LD_A_aa16, 4}, {"EI", &x::EI, 1}, {"ILLEGAL_FC", &x::ILLEGAL_FC, 1}, {"ILLEGAL_FD", &x::ILLEGAL_FD, 1}, {"CP", &x::CP_A_n8, 2},
        {"RST", &x::RST_38, 4}, 
    };

    prefixed_opcode_lookup = {
        {"RLC", &x::RLC_B, 2}, {"RLC", &x::RLC_C, 2}, {"RLC", &x::RLC_D, 2}, {"RLC", &x::RLC_E, 2}, {"RLC", &x::RLC_H, 2}, {"RLC", &x::RLC_L, 2},
        {"RLC", &x::RLC_aHL, 4}, {"RLC", &x::RLC_A, 2}, {"RRC", &x::RRC_B, 2}, {"RRC", &x::RRC_C, 2}, {"RRC", &x::RRC_D, 2}, {"RRC", &x::RRC_E, 2},
        {"RRC", &x::RRC_H, 2}, {"RRC", &x::RRC_L, 2}, {"RRC", &x::RRC_aHL, 4}, {"RRC", &x::RRC_A, 2}, {"RL", &x::RL_B, 2}, {"RL", &x::RL_C, 2},
        {"RL", &x::RL_D, 2}, {"RL", &x::RL_E, 2}, {"RL", &x::RL_H, 2}, {"RL", &x::RL_L, 2}, {"RL", &x::RL_aHL, 4}, {"RL", &x::RL_A, 2},
        {"RR", &x::RR_B, 2}, {"RR", &x::RR_C, 2}, {"RR", &x::RR_D, 2}, {"RR", &x::RR_E, 2}, {"RR", &x::RR_H, 2}, {"RR", &x::RR_L, 2},
        {"RR", &x::RR_aHL, 4}, {"RR", &x::RR_A, 2}, {"SLA", &x::SLA_B, 2}, {"SLA", &x::SLA_C, 2}, {"SLA", &x::SLA_D, 2}, {"SLA", &x::SLA_E, 2},
        {"SLA", &x::SLA_H, 2}, {"SLA", &x::SLA_L, 2}, {"SLA", &x::SLA_aHL, 4}, {"SLA", &x::SLA_A, 2}, {"SRA", &x::SRA_B, 2}, {"SRA", &x::SRA_C, 2},
        {"SRA", &x::SRA_D, 2}, {"SRA", &x::SRA_E, 2}, {"SRA", &x::SRA_H, 2}, {"SRA", &x::SRA_L, 2}, {"SRA", &x::SRA_aHL, 4}, {"SRA", &x::SRA_A, 2},
        {"SWAP", &x::SWAP_B, 2}, {"SWAP", &x::SWAP_C, 2}, {"SWAP", &x::SWAP_D, 2}, {"SWAP", &x::SWAP_E, 2}, {"SWAP", &x::SWAP_H, 2}, {"SWAP", &x::SWAP_L, 2},  
        {"SWAP", &x::SWAP_aHL, 4}, {"SWAP", &x::SWAP_A, 2}, {"SRL", &x::SRL_B, 2}, {"SRL", &x::SRL_C, 2}, {"SRL", &x::SRL_D, 2}, {"SRL", &x::SRL_E, 2},       
        {"SRL", &x::SRL_H, 2}, {"SRL", &x::SRL_L, 2}, {"SRL", &x::SRL_aHL, 4}, {"SRL", &x::SRL_A, 2}, {"BIT", &x::BIT_0_B, 2}, {"BIT", &x::BIT_0_C, 2},       
        {"BIT", &x::BIT_0_D, 2}, {"BIT", &x::BIT_0_E, 2}, {"BIT", &x::BIT_0_H, 2}, {"BIT", &x::BIT_0_L, 2}, {"BIT", &x::BIT_0_aHL, 3}, {"BIT", &x::BIT_0_A, 2},
        {"BIT", &x::BIT_1_B, 2}, {"BIT", &x::BIT_1_C, 2}, {"BIT", &x::BIT_1_D, 2}, {"BIT", &x::BIT_1_E, 2}, {"BIT", &x::BIT_1_H, 2}, {"BIT", &x::BIT_1_L, 2},  
        {"BIT", &x::BIT_1_aHL, 3}, {"BIT", &x::BIT_1_A, 2}, {"BIT", &x::BIT_2_B, 2}, {"BIT", &x::BIT_2_C, 2}, {"BIT", &x::BIT_2_D, 2}, {"BIT", &x::BIT_2_E, 2},
        {"BIT", &x::BIT_2_H, 2}, {"BIT", &x::BIT_2_L, 2}, {"BIT", &x::BIT_2_aHL, 3}, {"BIT", &x::BIT_2_A, 2}, {"BIT", &x::BIT_3_B, 2}, {"BIT", &x::BIT_3_C, 2},
        {"BIT", &x::BIT_3_D, 2}, {"BIT", &x::BIT_3_E, 2}, {"BIT", &x::BIT_3_H, 2}, {"BIT", &x::BIT_3_L, 2}, {"BIT", &x::BIT_3_aHL, 3}, {"BIT", &x::BIT_3_A, 2},
        {"BIT", &x::BIT_4_B, 2}, {"BIT", &x::BIT_4_C, 2}, {"BIT", &x::BIT_4_D, 2}, {"BIT", &x::BIT_4_E, 2}, {"BIT", &x::BIT_4_H, 2}, {"BIT", &x::BIT_4_L, 2},  
        {"BIT", &x::BIT_4_aHL, 3}, {"BIT", &x::BIT_4_A, 2}, {"BIT", &x::BIT_5_B, 2}, {"BIT", &x::BIT_5_C, 2}, {"BIT", &x::BIT_5_D, 2}, {"BIT", &x::BIT_5_E, 2},
        {"BIT", &x::BIT_5_H, 2}, {"BIT", &x::BIT_5_L, 2}, {"BIT", &x::BIT_5_aHL, 3}, {"BIT", &x::BIT_5_A, 2}, {"BIT", &x::BIT_6_B, 2}, {"BIT", &x::BIT_6_C, 2},
        {"BIT", &x::BIT_6_D, 2}, {"BIT", &x::BIT_6_E, 2}, {"BIT", &x::BIT_6_H, 2}, {"BIT", &x::BIT_6_L, 2}, {"BIT", &x::BIT_6_aHL, 3}, {"BIT", &x::BIT_6_A, 2},
        {"BIT", &x::BIT_7_B, 2}, {"BIT", &x::BIT_7_C, 2}, {"BIT", &x::BIT_7_D, 2}, {"BIT", &x::BIT_7_E, 2}, {"BIT", &x::BIT_7_H, 2}, {"BIT", &x::BIT_7_L, 2},  
        {"BIT", &x::BIT_7_aHL, 3}, {"BIT", &x::BIT_7_A, 2}, {"RES", &x::RES_0_B, 2}, {"RES", &x::RES_0_C, 2}, {"RES", &x::RES_0_D, 2}, {"RES", &x::RES_0_E, 2},
        {"RES", &x::RES_0_H, 2}, {"RES", &x::RES_0_L, 2}, {"RES", &x::RES_0_aHL, 4}, {"RES", &x::RES_0_A, 2}, {"RES", &x::RES_1_B, 2}, {"RES", &x::RES_1_C, 2},
        {"RES", &x::RES_1_D, 2}, {"RES", &x::RES_1_E, 2}, {"RES", &x::RES_1_H, 2}, {"RES", &x::RES_1_L, 2}, {"RES", &x::RES_1_aHL, 4}, {"RES", &x::RES_1_A, 2},
        {"RES", &x::RES_2_B, 2}, {"RES", &x::RES_2_C, 2}, {"RES", &x::RES_2_D, 2}, {"RES", &x::RES_2_E, 2}, {"RES", &x::RES_2_H, 2}, {"RES", &x::RES_2_L, 2},  
        {"RES", &x::RES_2_aHL, 4}, {"RES", &x::RES_2_A, 2}, {"RES", &x::RES_3_B, 2}, {"RES", &x::RES_3_C, 2}, {"RES", &x::RES_3_D, 2}, {"RES", &x::RES_3_E, 2},
        {"RES", &x::RES_3_H, 2}, {"RES", &x::RES_3_L, 2}, {"RES", &x::RES_3_aHL, 4}, {"RES", &x::RES_3_A, 2}, {"RES", &x::RES_4_B, 2}, {"RES", &x::RES_4_C, 2},
        {"RES", &x::RES_4_D, 2}, {"RES", &x::RES_4_E, 2}, {"RES", &x::RES_4_H, 2}, {"RES", &x::RES_4_L, 2}, {"RES", &x::RES_4_aHL, 4}, {"RES", &x::RES_4_A, 2},
        {"RES", &x::RES_5_B, 2}, {"RES", &x::RES_5_C, 2}, {"RES", &x::RES_5_D, 2}, {"RES", &x::RES_5_E, 2}, {"RES", &x::RES_5_H, 2}, {"RES", &x::RES_5_L, 2},  
        {"RES", &x::RES_5_aHL, 4}, {"RES", &x::RES_5_A, 2}, {"RES", &x::RES_6_B, 2}, {"RES", &x::RES_6_C, 2}, {"RES", &x::RES_6_D, 2}, {"RES", &x::RES_6_E, 2},
        {"RES", &x::RES_6_H, 2}, {"RES", &x::RES_6_L, 2}, {"RES", &x::RES_6_aHL, 4}, {"RES", &x::RES_6_A, 2}, {"RES", &x::RES_7_B, 2}, {"RES", &x::RES_7_C, 2},
        {"RES", &x::RES_7_D, 2}, {"RES", &x::RES_7_E, 2}, {"RES", &x::RES_7_H, 2}, {"RES", &x::RES_7_L, 2}, {"RES", &x::RES_7_aHL, 4}, {"RES", &x::RES_7_A, 2},
        {"SET", &x::SET_0_B, 2}, {"SET", &x::SET_0_C, 2}, {"SET", &x::SET_0_D, 2}, {"SET", &x::SET_0_E, 2}, {"SET", &x::SET_0_H, 2}, {"SET", &x::SET_0_L, 2},  
        {"SET", &x::SET_0_aHL, 4}, {"SET", &x::SET_0_A, 2}, {"SET", &x::SET_1_B, 2}, {"SET", &x::SET_1_C, 2}, {"SET", &x::SET_1_D, 2}, {"SET", &x::SET_1_E, 2},
        {"SET", &x::SET_1_H, 2}, {"SET", &x::SET_1_L, 2}, {"SET", &x::SET_1_aHL, 4}, {"SET", &x::SET_1_A, 2}, {"SET", &x::SET_2_B, 2}, {"SET", &x::SET_2_C, 2},
        {"SET", &x::SET_2_D, 2}, {"SET", &x::SET_2_E, 2}, {"SET", &x::SET_2_H, 2}, {"SET", &x::SET_2_L, 2}, {"SET", &x::SET_2_aHL, 4}, {"SET", &x::SET_2_A, 2},
        {"SET", &x::SET_3_B, 2}, {"SET", &x::SET_3_C, 2}, {"SET", &x::SET_3_D, 2}, {"SET", &x::SET_3_E, 2}, {"SET", &x::SET_3_H, 2}, {"SET", &x::SET_3_L, 2},  
        {"SET", &x::SET_3_aHL, 4}, {"SET", &x::SET_3_A, 2}, {"SET", &x::SET_4_B, 2}, {"SET", &x::SET_4_C, 2}, {"SET", &x::SET_4_D, 2}, {"SET", &x::SET_4_E, 2},
        {"SET", &x::SET_4_H, 2}, {"SET", &x::SET_4_L, 2}, {"SET", &x::SET_4_aHL, 4}, {"SET", &x::SET_4_A, 2}, {"SET", &x::SET_5_B, 2}, {"SET", &x::SET_5_C, 2},
        {"SET", &x::SET_5_D, 2}, {"SET", &x::SET_5_E, 2}, {"SET", &x::SET_5_H, 2}, {"SET", &x::SET_5_L, 2}, {"SET", &x::SET_5_aHL, 4}, {"SET", &x::SET_5_A, 2},
        {"SET", &x::SET_6_B, 2}, {"SET", &x::SET_6_C, 2}, {"SET", &x::SET_6_D, 2}, {"SET", &x::SET_6_E, 2}, {"SET", &x::SET_6_H, 2}, {"SET", &x::SET_6_L, 2},  
        {"SET", &x::SET_6_aHL, 4}, {"SET", &x::SET_6_A, 2}, {"SET", &x::SET_7_B, 2}, {"SET", &x::SET_7_C, 2}, {"SET", &x::SET_7_D, 2}, {"SET", &x::SET_7_E, 2},
        {"SET", &x::SET_7_H, 2}, {"SET", &x::SET_7_L, 2}, {"SET", &x::SET_7_aHL, 4}, {"SET", &x::SET_7_A, 2},
    };

    std::cout << "[DEBUG] opcodes loaded into the lookup tables <-----\n";
    unprefixed_instructions_names = {
        "NOP", "LD BC n16", "LD BC A", "INC BC", "INC B", "DEC B", "LD B n8", "RLCA", "LD a16 SP", "ADD HL BC", "LD A BC", "DEC BC", "INC C", "DEC C", "LD C n8", "RRCA",
        "STOP n8", "LD DE n16", "LD DE A", "INC DE", "INC D", "DEC D", "LD D n8", "RLA", "JR e8", "ADD HL DE", "LD A DE", "DEC DE", "INC E", "DEC E", "LD E n8", "RRA",
        "JR NZ e8", "LD HL n16", "LD HL A", "INC HL", "INC H", "DEC H", "LD H n8", "DAA", "JR Z e8", "ADD HL HL", "LD A HL", "DEC HL", "INC L", "DEC L", "LD L n8", "CPL",
        "JR NC e8", "LD SP n16", "LD HL A", "INC SP", "INC HL", "DEC HL", "LD HL n8", "SCF", "JR C e8", "ADD HL SP", "LD A HL", "DEC SP", "INC A", "DEC A", "LD A n8", "CCF",
        "LD B B", "LD B C", "LD B D", "LD B E", "LD B H", "LD B L", "LD B HL", "LD B A", "LD C B", "LD C C", "LD C D", "LD C E", "LD C H", "LD C L", "LD C HL", "LD C A",
        "LD D B", "LD D C", "LD D D", "LD D E", "LD D H", "LD D L", "LD D HL", "LD D A", "LD E B", "LD E C", "LD E D", "LD E E", "LD E H", "LD E L", "LD E HL", "LD E A",
        "LD H B", "LD H C", "LD H D", "LD H E", "LD H H", "LD H L", "LD H HL", "LD H A", "LD L B", "LD L C", "LD L D", "LD L E", "LD L H", "LD L L", "LD L HL", "LD L A",
        "LD HL B", "LD HL C", "LD HL D", "LD HL E", "LD HL H", "LD HL L", "HALT", "LD HL A", "LD A B", "LD A C", "LD A D", "LD A E", "LD A H", "LD A L", "LD A HL", "LD A A",
        "ADD A B", "ADD A C", "ADD A D", "ADD A E", "ADD A H", "ADD A L", "ADD A HL", "ADD A A", "ADC A B", "ADC A C", "ADC A D", "ADC A E", "ADC A H", "ADC A L", "ADC A HL", "ADC A A",
        "SUB A B", "SUB A C", "SUB A D", "SUB A E", "SUB A H", "SUB A L", "SUB A HL", "SUB A A", "SBC A B", "SBC A C", "SBC A D", "SBC A E", "SBC A H", "SBC A L", "SBC A HL", "SBC A A",
        "AND A B", "AND A C", "AND A D", "AND A E", "AND A H", "AND A L", "AND A HL", "AND A A", "XOR A B", "XOR A C", "XOR A D", "XOR A E", "XOR A H", "XOR A L", "XOR A HL", "XOR A A",
        "OR A B", "OR A C", "OR A D", "OR A E", "OR A H", "OR A L", "OR A HL", "OR A A", "CP A B", "CP A C", "CP A D", "CP A E", "CP A H", "CP A L", "CP A HL", "CP A A",
        "RET NZ", "POP BC", "JP NZ a16", "JP a16", "CALL NZ a16", "PUSH BC", "ADD A n8", "RST $00", "RET Z", "RET", "JP Z a16", "PREFIX", "CALL Z a16", "CALL a16", "ADC A n8", "RST $08",
        "RET NC", "POP DE", "JP NC a16", "ILLEGAL_D3", "CALL NC a16", "PUSH DE", "SUB A n8", "RST $10", "RET C", "RETI", "JP C a16", "ILLEGAL_DB", "CALL C a16", "ILLEGAL_DD", "SBC A n8", "RST $18",
        "LDH a8 A", "POP HL", "LDH C A", "ILLEGAL_E3", "ILLEGAL_E4", "PUSH HL", "AND A n8", "RST $20", "ADD SP e8", "JP HL", "LD a16 A", "ILLEGAL_EB", "ILLEGAL_EC", "ILLEGAL_ED", "XOR A n8", "RST $28",
        "LDH A a8", "POP AF", "LDH A C", "DI", "ILLEGAL_F4", "PUSH AF", "OR A n8", "RST $30", "LD HL SP e8", "LD SP HL", "LD A a16", "EI", "ILLEGAL_FC", "ILLEGAL_FD", "CP A n8", "RST $38",
    };

    #ifdef DEBUGMODE_ 
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    sdl_ttf_font = TTF_OpenFont("./fonts/Early GameBoy.ttf", 12);
    if(!sdl_ttf_font) {
        printf("TTF_OpenFont: %s\n", TTF_GetError());
    }
    SDL_CreateWindowAndRenderer(160*2, 144*2, 0, &registers_debug_window, &registers_debug_renderer);
    SDL_DisplayMode current_display;
    SDL_GetCurrentDisplayMode(0, &current_display); 
    SDL_SetWindowPosition(registers_debug_window, current_display.w/2 + 170*2, current_display.h/2 - 144*2);
    SDL_RenderSetScale(registers_debug_renderer, 1, 1);

    SDL_SetRenderDrawColor(registers_debug_renderer, 0x0, 0x0, 0x0, 0xff);
    SDL_RenderClear(registers_debug_renderer);

    SDL_RenderPresent(registers_debug_renderer);

    #endif

    #ifdef LOG_PC
    // clearing log file
    std::ofstream outfile("codewriteroutput/pc.log");
    if (outfile.is_open()) outfile.close();
    #endif
}

SM83::~SM83() {

}

#ifdef DEBUGMODE_
void SM83::drawDebug() {
    SDL_SetRenderDrawColor(registers_debug_renderer, 0x00, 0x00, 0x00, 255);
    SDL_RenderClear(registers_debug_renderer);

    // Render register values
    int y = 10;
    renderText(registers_debug_renderer, sdl_ttf_font, "a  = " + formatHex(a,  2), 10, y, sdl_color_white); y += 12;
    renderText(registers_debug_renderer, sdl_ttf_font, "f  = " + formatHex(f,  2), 10, y, sdl_color_white); y += 12;
    renderText(registers_debug_renderer, sdl_ttf_font, "b  = " + formatHex(b,  2), 10, y, sdl_color_white); y += 12;
    renderText(registers_debug_renderer, sdl_ttf_font, "c  = " + formatHex(c,  2), 10, y, sdl_color_white); y += 12;
    renderText(registers_debug_renderer, sdl_ttf_font, "d  = " + formatHex(d,  2), 10, y, sdl_color_white); y += 12;
    renderText(registers_debug_renderer, sdl_ttf_font, "e  = " + formatHex(e,  2), 10, y, sdl_color_white); y += 12;
    renderText(registers_debug_renderer, sdl_ttf_font, "h  = " + formatHex(h,  2), 10, y, sdl_color_white); y += 12;
    renderText(registers_debug_renderer, sdl_ttf_font, "l  = " + formatHex(l,  2), 10, y, sdl_color_white); y += 12;
    renderText(registers_debug_renderer, sdl_ttf_font, "sp = " + formatHex(sp, 4), 10, y, sdl_color_white); y += 12;
    renderText(registers_debug_renderer, sdl_ttf_font, "pc = " + formatHex(pc, 4), 10, y, sdl_color_white); y += 12;
    renderText(registers_debug_renderer, sdl_ttf_font, "af = " + formatHex(af, 4), 10, y, sdl_color_white); y += 12;
    renderText(registers_debug_renderer, sdl_ttf_font, "bc = " + formatHex(bc, 4), 10, y, sdl_color_white); y += 12;
    renderText(registers_debug_renderer, sdl_ttf_font, "de = " + formatHex(de, 4), 10, y, sdl_color_white); y += 12;
    renderText(registers_debug_renderer, sdl_ttf_font, "hl = " + formatHex(hl, 4), 10, y, sdl_color_white); y += 12;
    renderText(registers_debug_renderer, sdl_ttf_font, "last pc = " + formatHex(last_executed_pc, 4), 10, y, sdl_color_white); y += 12;
    renderText(registers_debug_renderer, sdl_ttf_font, last_instruction, 10, y, sdl_color_white); y += 12;
    renderText(registers_debug_renderer, sdl_ttf_font, "opcode = " + formatHex(opcode, 4), 10, y, sdl_color_white); y += 12;

    SDL_RenderPresent(registers_debug_renderer);
    
    // Handle events
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            std::cout << "quit sensed. ignoring." << std::endl;
        }
    }
}
#endif

void SM83::handleInterrupts() {
        
    for (int interrupt_bit = 0; interrupt_bit < 5; interrupt_bit++) {
        uint8_t interrupt = 1 << interrupt_bit;
        if ((ie & interrupt) && (ime == 1) && (if_ & interrupt)) {
            ime = 0;
            // ie &= ~interrupt;
            if_ &= ~interrupt; // reset all interrupt flags 
            // and then handle the interrupt;

            switch (interrupt) {
                case InterruptFlags::Joypad:
                    PUSH({PC, true}); // save the pre-call PC to stack for RET to retreive later
                    pc = 0x0060;
                break;
                case InterruptFlags::Serial:
                    PUSH({PC, true});
                    pc = 0x0058;
                break;
                case InterruptFlags::Timer:
                    PUSH({PC, true});
                    pc = 0x0050;
                break;
                case InterruptFlags::Stat:
                    PUSH({PC, true});
                    pc = 0x0048;
                break;
                case InterruptFlags::VBlank:
                    PUSH({PC, true});
                    pc = 0x0040;
                break;
            }
            updateRegisters8(); // just for good measure (?)
            cycles += 5;
            break;
        }

    } 
}

// void SM83::boot() {

//     pc = 0x00;
//     while (pc < 0x100) {
//         opcode = read(pc++);
//         last_instruction = unprefixed_instructions_names[opcode];
//         if (opcode == 0xcb) {
//             last_executed_pc = pc;
//             opcode = read(pc++);
//             last_instruction = unprefixed_instructions_names[opcode];
//             (this->*prefixed_opcode_lookup[opcode].operate)();
//         } else {
//             (this->*unprefixed_opcode_lookup[opcode].operate)();
//         }
//         #ifdef DEBUGMODE_
//         drawDebug();
//         #endif
//     }

//     pc = 0x100;
//     // unload the bootrom
//     bus->bootrom = bus->cart; // lazy fix

// }

void SM83::clock() {

    if (cycles == 0) {
        if (ime > 1) ime--; // this is part of IE instruction implementation. because it acts one instruction after the IE instruct.

        // if boot rom finished, unload it.
        if (0x100 <= pc and pc <= 0x102) {
            bus->bootrom = bus->cart; // lazy fix
            std::cout << "Boot rom unloaded" << std::endl;
        }


        last_executed_pc = pc;
        opcode = read(pc++);
        last_instruction = unprefixed_instructions_names[opcode];
        uint8_t additional_clock_cycles = 0;

        // std::cout << "[PC|OP] " << std::hex << pc-1 << '|' << (int)opcode << std::dec << std::endl;

        #ifdef DEBUGMODE_

        if (std::find(bus->cart.valid_instruction_addresses.begin(), 
                       bus->cart.valid_instruction_addresses.end(), 
                            last_executed_pc) == bus->cart.valid_instruction_addresses.end()) 
        {
            std::cout << "INVALID INSTRUCITON! " << std::hex << last_executed_pc << std::dec << std::endl;
        }
        this->drawDebug();
        // std::cout << "----------------------------" << std::endl << "----------------------------" << std::endl << "----------------------------" << std::endl;
        // std::cout << std::hex << std::setfill('0');
        // std::cout << "a  = 0x" << std::setw(2) << static_cast<int>(a) << std::endl;
        // std::cout << "f  = 0x" << std::setw(2) << static_cast<int>(f) << std::endl;
        // std::cout << "b  = 0x" << std::setw(2) << static_cast<int>(b) << std::endl;
        // std::cout << "c  = 0x" << std::setw(2) << static_cast<int>(c) << std::endl;
        // std::cout << "d  = 0x" << std::setw(2) << static_cast<int>(d) << std::endl;
        // std::cout << "e  = 0x" << std::setw(2) << static_cast<int>(e) << std::endl;
        // std::cout << "h  = 0x" << std::setw(2) << static_cast<int>(h) << std::endl;
        // std::cout << "l  = 0x" << std::setw(2) << static_cast<int>(l) << std::endl;
        // std::cout << "sp = 0x" << std::setw(4) << sp << std::endl;
        // std::cout << "pc = 0x" << std::setw(4) << pc << std::endl;

        // std::cout << "af = 0x" << std::setw(4) << af << std::endl;
        // std::cout << "bc = 0x" << std::setw(4) << bc << std::endl;
        // std::cout << "de = 0x" << std::setw(4) << de << std::endl;
        // std::cout << "hl = 0x" << std::setw(4) << hl << std::endl;

        #endif



        if (opcode == 0xcb) {
            // last_executed_pc = pc;
            opcode = read(pc++);
            additional_clock_cycles = (this->*prefixed_opcode_lookup[opcode].operate)();
            cycles = prefixed_opcode_lookup[opcode].cycles + additional_clock_cycles + prefixed_opcode_lookup[0xcb].cycles;
            if (pc-last_executed_pc != 2)
                std::cout << "[ERROR] some prefixed instruction disturbed pc too much!" << std::endl;
                pc = last_executed_pc + 2; // manually setting the correct pc position just in case
        } else {
            additional_clock_cycles = (this->*unprefixed_opcode_lookup[opcode].operate)();
            cycles = unprefixed_opcode_lookup[opcode].cycles + additional_clock_cycles;

            bus->joypad.update();
        }

        #ifdef LOG_PC
        logLastPC();
        #endif
        handleInterrupts();
    }

    cycles--;
    // std::cout << "CPU CLOCKED" << std::endl;


}


#ifdef DEBUGMODE_

void SM83::opDebug() {
    pc = 0x160;
    for (uint8_t opcode = 0; opcode < 0x100; opcode++) {
        if (opcode == 0xcb) continue;
        (this->*unprefixed_opcode_lookup[opcode].operate)();
    }

}

#endif

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
                value = &a; break;
            case F: 
                value = &f; break;
            case B: 
                value = &b; break;
            case C:
                value = &c; break;
            case D:
                value = &d; break;
            case E:
                value = &e; break;
            case H:
                value = &h; break;
            case L: 
                value = &l; break;
            case N8:
                value = readPttr(pc++); break;
        }
    } else {
        int8_t di = operand.increment - operand.decrement;
        switch (operand.name) {
            case AF:
                value = readPttr((a << 8) | f);
                addToHiLo(a, f, di);
                break;
            case BC:
                value = readPttr((b << 8) | c);
                addToHiLo(b, c, di);
                break;
            case DE:
                value = readPttr((d << 8) | e);
                addToHiLo(d, e, di);
                break;
            case HL:
                value = readPttr((h << 8) | l);
                addToHiLo(h, l, di);
                break;
            case SP:
                value = readPttr(sp);
                sp += di;
                break;
            case A16: {
                uint8_t lo = read(pc++);
                uint8_t hi = read(pc++);
                value = readPttr((hi << 8) | lo);
                break;
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
                value = &af; break;
            case BC:
                value = &bc; break;
            case DE:
                value = &de; break;
            case HL:
                value = &hl; break;
            case SP:
                value = &sp; break;
            case PC:
                value = &pc; break;
            case N16: { 
                // n16 is never modified, only fetched. thus instead of trying to somehow 
                // get a pointer to (hram[addr] | (hram[addr+1] << 8)), i'm simply fetching 
                // it into a third variable and returning its reference.
                uint8_t lo = read(pc++);
                uint8_t hi = read(pc++);
                fetched16 = (hi << 8) | lo;
                value = &fetched16;
                break;
            }

            case A16: { 
                // n16 is never modified, only fetched. thus instead of trying to somehow 
                // get a pointer to (hram[addr] | (hram[addr+1] << 8)), i'm simply fetching 
                // it into a third variable and returning its reference.
                uint8_t lo = read(pc++);
                uint8_t hi = read(pc++);
                fetched16 = (hi << 8) | lo;
                value = &fetched16;
                break;
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
                break;
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


uint8_t SM83::LDH(Operand target, Operand source) {
    uint8_t sourceValue = 0;
    if (source.name == C) {
        sourceValue = read(0xff00 | c);
    } else if (source.name == A8) {
        sourceValue = read(0xff00 | read(pc++));
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
        setFlag(fn, 0);
        setFlag(fh, 0);
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
        if (getFlag(fn)) { // subtraction
            if (getFlag(fc)) { a -= 0x60; }
            if (getFlag(fh)) { a -= 0x06; }
        } else {           // addition
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
        setFlag(fh, (*targetValue & 0x0F) == 0x0F);
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
            setFlag(fz, *targetValue == 0);
            setFlag(fn, 0);
            setFlag(fh, 1);
            setFlag(fc, 0);
            break;
        case XOR:
            *targetValue ^= *sourceValue;
            setFlag(fz, *targetValue == 0);
            setFlag(fn, 0);
            setFlag(fh, 0);
            setFlag(fc, 0);
            break;
        case OR:
            *targetValue |= *sourceValue;
            setFlag(fz, *targetValue == 0);
            setFlag(fn, 0);
            setFlag(fh, 0);
            setFlag(fc, 0);
            break;
        case CP: {
            uint16_t result = *targetValue - *sourceValue;
            setFlag(fn, 1);
            // setFlag(fh, (*targetValue & 0x0F) + (*sourceValue & 0x0F) & 0x10);
            setFlag(fh, ((*targetValue & 0x0F) < (*sourceValue & 0x0F)));
            setFlag(fz, (result && 0xff) == 0);
            // setFlag(fc, result & 0x0100);
            setFlag(fc, (*targetValue < *sourceValue));
            
            updateRegisters16();
            return 0;
        }
    }

    setFlag(fz, *targetValue & 0xff == 0);

    updateRegisters16();
    return 0;
}



uint8_t SM83::PROCESS_ALU16(Operand target, ALUOperation operation) {

    uint16_t *targetValue = process_operand16(target);

    switch (operation) {
    case INC:
        *targetValue = *targetValue + 1;
        break;
    case DEC:
        *targetValue = *targetValue - 1;
        break;
    }

    updateRegisters8();
    return 0;

}


uint8_t SM83::PROCESS_ALU16(Operand target, Operand source, ALUOperation operation) {

    uint16_t *targetValue = process_operand16(target);

    uint16_t storage = 0;
    int8_t signed_storage = 0;
    uint16_t *sourceValue = &storage;

    if (source.name == E8) { 
        // something tells me this implementation is not supposed to be this easy. 
        // but it really depends how C++ handles uint and int behind the scenes
        // so i wont know which method of implementation is correct until i 
        // actually test it out.
    
        signed_storage = (toSigned(read(pc++)));// if it is e8 this value will be non zero
    } else {
        sourceValue = process_operand16(source); // else this will be non zero
    }

    switch (operation) {
    // theres only one operation in 16 bit ALU other than inc and dec. so i'm 
    // so i'm not separating addition out into a different core helper function.
    case ADD: {
        uint32_t result = *targetValue + *sourceValue + signed_storage;

        setFlag(fh, ((*targetValue & 0xFFF) + (*sourceValue & 0xFFF)) & 0x1000);
        setFlag(fc, result > 0xFFFF);
        setFlag(fn, 0);
        if (source.name == OperandName::E8) {
            setFlag(fz, 0);
            setFlag(fn, 0);
            setFlag(fh, ((*targetValue & 0x0F) + (signed_storage & 0x0F)) & 0x10);
            setFlag(fc, ((*targetValue & 0xFF) + (signed_storage & 0xFF)) & 0x100);
        }
        
        *targetValue = result & 0xFFFF;
        break;

    }
    }

    updateRegisters8();
    return 0;
}


uint8_t SM83::JUMPTO(Operand address) {

    // if arg was E8 that means this is a relative jump instruction
    if (address.name == E8) {
        int16_t relative_address = (toSigned(read(pc++)));
        pc += relative_address;
    } else {
        uint16_t *target_address = process_operand16(address);
        pc = *target_address;
    }

    updateRegisters8();
    return 0;
}


uint8_t SM83::JUMPTO(OperandName condition, Operand address) {
    
    bool conditiontrue = false;

    switch (condition) {
        case C: conditiontrue = getFlag(fc); break;
        case NC: conditiontrue = !getFlag(fc); break;
        case Z: conditiontrue = getFlag(fz); break;
        case NZ: conditiontrue = !getFlag(fz); break;
    }

    if (not conditiontrue) {
        // skip ahead to the next instruction byte;    
        if (address.name == N16) pc += 2; 
        else if (address.name == E8) pc += 1;
        return 0;
    }   

    JUMPTO(address);
    return 1; // it will take 4 additional clock cycles (i.e. 1 machine cycle) if the condition was true 

}


uint8_t SM83::CALL(Operand address) {
    uint16_t *target_address = process_operand16(address);
    PUSH({PC, true}); // save the pre-call PC to stack for RET to retreive later
    pc = *target_address;

    updateRegisters8();
    return 0;
}


uint8_t SM83::CALL(OperandName condition, Operand address) {

    bool conditiontrue = false;

    switch (condition) {
        case C: conditiontrue = getFlag(fc); break;
        case NC: conditiontrue = !getFlag(fc); break;
        case Z: conditiontrue = getFlag(fz); break;
        case NZ: conditiontrue = !getFlag(fz); break;
    }

    if (not conditiontrue) {
        // skip ahead to the next instruction byte;    
        pc += 2; 
        return 0;
    }  

    CALL(address);
    return 3;
}

uint8_t SM83::RST(uint8_t address_lo) {
    
    PUSH({PC, true});
    pc = 0x0000 + address_lo;

    updateRegisters8();
    return 0;

}


uint8_t SM83::RETURNFROMFUNCTION() {
    POP({PC, true});
    return 0;
}

uint8_t SM83::RETURNFROMFUNCTION(OperandName condition) {
    switch (condition) {
        case C: if (not getFlag(fc)) return 0;
        case NC: if (not !getFlag(fc)); return 0;
        case Z: if (not getFlag(fz)); return 0;
        case NZ: if (not !getFlag(fz)); return 0;
    }

    POP({PC, true});
    return 3; // takes 12 additional cycles if the condition is true;
}

uint8_t SM83::RETURNANDEI() {
    EI();
    RETURNFROMFUNCTION();
    return 0;
}

uint8_t SM83::ENABLEINTERRUPTS() {
    ime = 2;
    return 0;
}

uint8_t SM83::DISABLEINTERRUPTS() {
    ime = 0;
    return 0;
}


uint8_t SM83::ROTATELEFT(Operand operand, bool reversed, bool through_carry) {
    uint8_t *targetValue = process_operand(operand);

    if (reversed) {
        uint8_t new_carry = *targetValue & 0x01;
        *targetValue = (*targetValue >> 1) | (through_carry ? getFlag(fc) : new_carry);
        setFlag(fc, new_carry);
    } else {
        uint8_t new_carry = *targetValue & 0x80;
        *targetValue = (*targetValue << 1) | (through_carry ? getFlag(fc) : new_carry);
        setFlag(fc, new_carry);
    }
    
    setFlag(fz, *targetValue == 0);
    setFlag(fn, 0);
    setFlag(fh, 0);
    updateRegisters16();
    return 0;
}

uint8_t SM83::ROTATEREGALEFT(bool reversed, bool through_carry) {

    ROTATELEFT({A, true}, reversed, through_carry);
    setFlag(fz, 0);
    updateRegisters16();
    return 0;
}

uint8_t SM83::SHIFTLEFT(Operand operand, bool reversed, bool is_logical) {
    uint8_t *targetValue = process_operand(operand);


    if (reversed) {
        if (is_logical) {
            setFlag(fc, *targetValue & 0x01);
            *targetValue = (*targetValue >> 1);
        } else {
            setFlag(fc, *targetValue & 0x01);
            *targetValue = (*targetValue >> 1);
            *targetValue |= (*targetValue & 0x40) << 1;
        }
    } else {
        setFlag(fc, *targetValue & 0x80);
        *targetValue = (*targetValue << 1);
    }

    setFlag(fz, *targetValue == 0);
    setFlag(fn, 0);
    setFlag(fh, 0);
    updateRegisters16();
    return 0;
}

uint8_t SM83::SWAP(Operand operand) {
    uint8_t *targetValue = process_operand(operand);

    uint8_t lo = *targetValue & 0x0F;
    uint8_t hi = *targetValue & 0xF0;

    *targetValue = (lo << 4) | (hi >> 4);

    setFlag(fz, *targetValue == 0);
    setFlag(fn, 0);
    setFlag(fh, 0);
    setFlag(fc, 0);
    updateRegisters16();
    return 0;
}

uint8_t SM83::BIT(uint8_t bit, Operand operand) {
    uint8_t *targetValue = process_operand(operand);

    setFlag(fz, (*targetValue & (1 << bit)) == 0);
    setFlag(fh, 1);
    setFlag(fn, 0);
    updateRegisters16();
    return 0;
}

uint8_t SM83::RES(uint8_t bit, Operand operand) {
    uint8_t *targetValue = process_operand(operand);

    *targetValue &= ~(1 << bit);

    updateRegisters16();
    return 0;
}

uint8_t SM83::SET(uint8_t bit, Operand operand) {
    uint8_t *targetValue = process_operand(operand);

    *targetValue |= (1 << bit);

    updateRegisters16();
    return 0;
}


// CORE HELPERS

uint8_t SM83::ADD8(uint8_t *targetValue, uint8_t *sourceValue, bool isADC) {
    uint8_t carry = isADC ? getFlag(fc) : 0;

    uint16_t result = *targetValue + *sourceValue + carry;

    // flags
    setFlag(fn, 0);
    setFlag(fh, ((*targetValue & 0x0F) + (*sourceValue & 0x0F) + carry) & 0x10);
    setFlag(fc, result & 0x0100);
    // Flags Z is set by the function wrapping this one (PROCESS_ALU)

    *targetValue = result & 0xff;

    updateRegisters16(); // because the flags are stored in an 8 bit register
    return 0;
}

uint8_t SM83::SUB8(uint8_t *targetValue, uint8_t *sourceValue, bool isSBC) {
    short borrow = isSBC ? getFlag(fc) : 0;

    uint16_t result = *targetValue - *sourceValue - borrow;

    // flags
    setFlag(fn, 1);
    // setFlag(fh, ((*targetValue & 0x0F) - (*sourceValue & 0x0F) - borrow) & 0x10);
    setFlag(fh, ((*targetValue & 0x0F) < ((*sourceValue & 0x0F) + borrow)));
    // setFlag(fc, result & 0x0100);
    setFlag(fc, (*targetValue < (*sourceValue + borrow)));
    // flag z is set by function wrapping this one. (PROCESSALU)

    *targetValue = result & 0xff;

    updateRegisters16(); // because the flags are stored in an 8 bit register
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
uint8_t SM83::RLCA() { return ROTATEREGALEFT(false, false); }
uint8_t SM83::LD_aa16_SP() { return LD16({A16, false}, {SP, true}); }
uint8_t SM83::ADD_HL_BC() { return PROCESS_ALU16({HL, true}, {BC, true}, ADD); }
uint8_t SM83::LD_A_aBC() { return LD({A, true}, {BC, false}); }
uint8_t SM83::DEC_BC() { return PROCESS_ALU16({BC, true}, DEC); }
uint8_t SM83::INC_C() { return PROCESS_ALU({C, true}, INC); }
uint8_t SM83::DEC_C() { return PROCESS_ALU({C, true}, DEC); }
uint8_t SM83::LD_C_n8() { return LD({C, true}, {N8, true}); }
uint8_t SM83::RRCA() { return ROTATEREGALEFT(true, false); }
uint8_t SM83::STOP_n8() { pc++; return 0; }
uint8_t SM83::LD_DE_n16() { return LD16({DE, true}, {N16, true}); }
uint8_t SM83::LD_aDE_A() { return LD({DE, false}, {A, true}); }
uint8_t SM83::INC_DE() { return PROCESS_ALU16({DE, true}, INC); }
uint8_t SM83::INC_D() { return PROCESS_ALU({D, true}, INC); }
uint8_t SM83::DEC_D() { return PROCESS_ALU({D, true}, DEC); }
uint8_t SM83::LD_D_n8() { return LD({D, true}, {N8, true}); }
uint8_t SM83::RLA() { return ROTATEREGALEFT(false, true); }
uint8_t SM83::JR_e8() { return JUMPTO({E8, true}); }
uint8_t SM83::ADD_HL_DE() { return PROCESS_ALU16({HL, true}, {DE, true}, ADD); }
uint8_t SM83::LD_A_aDE() { return LD({A, true}, {DE, false}); }
uint8_t SM83::DEC_DE() { return PROCESS_ALU16({DE, true}, DEC); }
uint8_t SM83::INC_E() { return PROCESS_ALU({E, true}, INC); }
uint8_t SM83::DEC_E() { return PROCESS_ALU({E, true}, DEC); }
uint8_t SM83::LD_E_n8() { return LD({E, true}, {N8, true}); }
uint8_t SM83::RRA() { return ROTATEREGALEFT(true, true); }
uint8_t SM83::JR_NZ_e8() { return JUMPTO(NZ, {E8, true}); }
uint8_t SM83::LD_HL_n16() { return LD16({HL, true}, {N16, true}); }
uint8_t SM83::LD_aHLI_A() { return LD({HL, false, .increment=true}, {A, true}); }
uint8_t SM83::INC_HL() { return PROCESS_ALU16({HL, true}, INC); }
uint8_t SM83::INC_H() { return PROCESS_ALU({H, true}, INC); }
uint8_t SM83::DEC_H() { return PROCESS_ALU({H, true}, DEC); }
uint8_t SM83::LD_H_n8() { return LD({H, true}, {N8, true}); }
uint8_t SM83::DAA() { return PROCESS_ALU(enumDAA); }
uint8_t SM83::JR_Z_e8() { return JUMPTO(Z, {E8, true}); }
uint8_t SM83::ADD_HL_HL() { return PROCESS_ALU16({HL, true}, {HL, true}, ADD); }
uint8_t SM83::LD_A_aHLI() { return LD({A, true}, {HL, false, .increment=true}); }
uint8_t SM83::DEC_HL() { return PROCESS_ALU16({HL, true}, DEC); }
uint8_t SM83::INC_L() { return PROCESS_ALU({L, true}, INC); }
uint8_t SM83::DEC_L() { return PROCESS_ALU({L, true}, DEC); }
uint8_t SM83::LD_L_n8() { return LD({L, true}, {N8, true}); }
uint8_t SM83::CPL() { return PROCESS_ALU(enumCPL); }
uint8_t SM83::JR_NC_e8() { return JUMPTO(NC, {E8, true}); }
uint8_t SM83::LD_SP_n16() { return LD16({SP, true}, {N16, true}); }
uint8_t SM83::LD_aHLD_A() { return LD({HL, false, .decrement=true}, {A, true}); }
uint8_t SM83::INC_SP() { return PROCESS_ALU16({SP, true}, INC); }
uint8_t SM83::INC_aHL() { return PROCESS_ALU({HL, false}, INC); }
uint8_t SM83::DEC_aHL() { return PROCESS_ALU({HL, false}, DEC); }
uint8_t SM83::LD_aHL_n8() { return LD({HL, false}, {N8, true}); }
uint8_t SM83::SCF() { return PROCESS_ALU(enumSCF); }
uint8_t SM83::JR_C_e8() { return JUMPTO(C, {E8, true}); }
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
uint8_t SM83::HALT() { std::cout << "SYSTEM HALT REQUESTED!"; return 0; }
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
uint8_t SM83::RET_NZ() { return RETURNFROMFUNCTION(NZ); }
uint8_t SM83::POP_BC() { return POP({BC, true}); }
uint8_t SM83::JP_NZ_a16() { return JUMPTO(NZ, {A16, true}); }
uint8_t SM83::JP_a16() { return JUMPTO({A16, true}); }
uint8_t SM83::CALL_NZ_a16() { return CALL(NZ, {A16, true}); }
uint8_t SM83::PUSH_BC() { return PUSH({BC, true}); }
uint8_t SM83::ADD_A_n8() { return PROCESS_ALU({A, true}, {N8, true}, ADD); }
uint8_t SM83::RST_00() { return RST(0x00); }
uint8_t SM83::RET_Z() { return RETURNFROMFUNCTION(Z); }
uint8_t SM83::RET() { return POP({PC, true}); }
uint8_t SM83::JP_Z_a16() { return JUMPTO(Z, {A16, true}); }
uint8_t SM83::PREFIX() { return 0; }
uint8_t SM83::CALL_Z_a16() { return CALL(Z, {A16, true}); }
uint8_t SM83::CALL_a16() { return CALL({A16, true}); }
uint8_t SM83::ADC_A_n8() { return PROCESS_ALU({A, true}, {N8, true}, ADC); }
uint8_t SM83::RST_08() { return RST(0x08); }
uint8_t SM83::RET_NC() { return RETURNFROMFUNCTION(NC); }
uint8_t SM83::POP_DE() { return POP({DE, true}); }
uint8_t SM83::JP_NC_a16() { return JUMPTO(NC, {A16, true}); }
uint8_t SM83::ILLEGAL_D3() { return 0; }
uint8_t SM83::CALL_NC_a16() { return CALL(NC, {A16, true}); }
uint8_t SM83::PUSH_DE() { return PUSH({DE, true}); }
uint8_t SM83::SUB_A_n8() { return PROCESS_ALU({A, true}, {N8, true}, SUB); }
uint8_t SM83::RST_10() { return RST(0x10); }
uint8_t SM83::RET_C() { return RETURNFROMFUNCTION(C); }
uint8_t SM83::RETI() { return RETURNANDEI(); }
uint8_t SM83::JP_C_a16() { return JUMPTO(C, {A16, true}); }
uint8_t SM83::ILLEGAL_DB() { return 0; }
uint8_t SM83::CALL_C_a16() { return CALL(C, {A16, true}); }
uint8_t SM83::ILLEGAL_DD() { return 0; }
uint8_t SM83::SBC_A_n8() { return PROCESS_ALU({A, true}, {N8, true}, SBC); }
uint8_t SM83::RST_18() { return RST(0x18); }
uint8_t SM83::LDH_aa8_A() { return LDH({A8, false}, {A, true}); }
uint8_t SM83::POP_HL() { return POP({HL, true}); }
uint8_t SM83::LDH_aC_A() { return LDH({C, false}, {A, true}); }
uint8_t SM83::ILLEGAL_E3() { return 0; }
uint8_t SM83::ILLEGAL_E4() { return 0; }
uint8_t SM83::PUSH_HL() { return PUSH({HL, true}); }
uint8_t SM83::AND_A_n8() { return PROCESS_ALU({A, true}, {N8, true}, AND); }
uint8_t SM83::RST_20() { return RST(0x20); }
uint8_t SM83::ADD_SP_e8() { return PROCESS_ALU16({SP, true}, {E8, true}, ADD); }
uint8_t SM83::JP_HL() { return JUMPTO({HL, true}); }
uint8_t SM83::LD_aa16_A() { return LD({A16, false}, {A, true}); }
uint8_t SM83::ILLEGAL_EB() { return 0; }
uint8_t SM83::ILLEGAL_EC() { return 0; }
uint8_t SM83::ILLEGAL_ED() { return 0; }
uint8_t SM83::XOR_A_n8() { return PROCESS_ALU({A, true}, {N8, true}, XOR); }
uint8_t SM83::RST_28() { return RST(0x28); }
uint8_t SM83::LDH_A_aa8() { return LDH({A, true}, {A8, false}); }
uint8_t SM83::POP_AF() { return POP({AF, true}); }
uint8_t SM83::LDH_A_aC() { return LDH({A, true}, {C, false}); }
uint8_t SM83::DI() { return DISABLEINTERRUPTS(); }
uint8_t SM83::ILLEGAL_F4() { return 0; }
uint8_t SM83::PUSH_AF() { return PUSH({AF, true}); }
uint8_t SM83::OR_A_n8() { return PROCESS_ALU({A, true}, {N8, true}, OR); }
uint8_t SM83::RST_30() { return RST(0x30); }
uint8_t SM83::LD_HL_SPI_e8() { return LD_HL_SPDD(); }
uint8_t SM83::LD_SP_HL() { return LD16({SP, true}, {HL, true}); }
uint8_t SM83::LD_A_aa16() { return LD({A, true}, {A16, false}); }
uint8_t SM83::EI() { return ENABLEINTERRUPTS(); }
uint8_t SM83::ILLEGAL_FC() { return 0; }
uint8_t SM83::ILLEGAL_FD() { return 0; }
uint8_t SM83::CP_A_n8() { return PROCESS_ALU({A, true}, {N8, true}, CP); }
uint8_t SM83::RST_38() { return RST(0x38); }



// CB PREFIXED
uint8_t SM83::RLC_B() { return ROTATELEFT({B, true}, false, true); }
uint8_t SM83::RLC_C() { return ROTATELEFT({C, true}, false, true); }
uint8_t SM83::RLC_D() { return ROTATELEFT({D, true}, false, true); }
uint8_t SM83::RLC_E() { return ROTATELEFT({E, true}, false, true); }
uint8_t SM83::RLC_H() { return ROTATELEFT({H, true}, false, true); }
uint8_t SM83::RLC_L() { return ROTATELEFT({L, true}, false, true); }
uint8_t SM83::RLC_aHL() { return ROTATELEFT({HL, false}, false, true); }
uint8_t SM83::RLC_A() { return ROTATELEFT({A, true}, false, true); }
uint8_t SM83::RRC_B() { return ROTATELEFT({B, true}, true, true); }
uint8_t SM83::RRC_C() { return ROTATELEFT({C, true}, true, true); }
uint8_t SM83::RRC_D() { return ROTATELEFT({D, true}, true, true); }
uint8_t SM83::RRC_E() { return ROTATELEFT({E, true}, true, true); }
uint8_t SM83::RRC_H() { return ROTATELEFT({H, true}, true, true); }
uint8_t SM83::RRC_L() { return ROTATELEFT({L, true}, true, true); }
uint8_t SM83::RRC_aHL() { return ROTATELEFT({HL, false}, true, true); }
uint8_t SM83::RRC_A() { return ROTATELEFT({A, true}, true, true); }
uint8_t SM83::RL_B() { return ROTATELEFT({B, true}, false, false); }
uint8_t SM83::RL_C() { return ROTATELEFT({C, true}, false, false); }
uint8_t SM83::RL_D() { return ROTATELEFT({D, true}, false, false); }
uint8_t SM83::RL_E() { return ROTATELEFT({E, true}, false, false); }
uint8_t SM83::RL_H() { return ROTATELEFT({H, true}, false, false); }
uint8_t SM83::RL_L() { return ROTATELEFT({L, true}, false, false); }
uint8_t SM83::RL_aHL() { return ROTATELEFT({HL, false}, false, false); }
uint8_t SM83::RL_A() { return ROTATELEFT({A, true}, false, false); }
uint8_t SM83::RR_B() { return ROTATELEFT({B, true}, true, false); }
uint8_t SM83::RR_C() { return ROTATELEFT({C, true}, true, false); }
uint8_t SM83::RR_D() { return ROTATELEFT({D, true}, true, false); }
uint8_t SM83::RR_E() { return ROTATELEFT({E, true}, true, false); }
uint8_t SM83::RR_H() { return ROTATELEFT({H, true}, true, false); }
uint8_t SM83::RR_L() { return ROTATELEFT({L, true}, true, false); }
uint8_t SM83::RR_aHL() { return ROTATELEFT({HL, false}, true, false); }
uint8_t SM83::RR_A() { return ROTATELEFT({A, true}, true, false); }
uint8_t SM83::SLA_B() { return SHIFTLEFT({B, true}, false, false); }
uint8_t SM83::SLA_C() { return SHIFTLEFT({C, true}, false, false); }
uint8_t SM83::SLA_D() { return SHIFTLEFT({D, true}, false, false); }
uint8_t SM83::SLA_E() { return SHIFTLEFT({E, true}, false, false); }
uint8_t SM83::SLA_H() { return SHIFTLEFT({H, true}, false, false); }
uint8_t SM83::SLA_L() { return SHIFTLEFT({L, true}, false, false); }
uint8_t SM83::SLA_aHL() { return SHIFTLEFT({HL, false}, false, false); }
uint8_t SM83::SLA_A() { return SHIFTLEFT({A, true}, false, false); }
uint8_t SM83::SRA_B() { return SHIFTLEFT({B, true}, true, false); }
uint8_t SM83::SRA_C() { return SHIFTLEFT({C, true}, true, false); }
uint8_t SM83::SRA_D() { return SHIFTLEFT({D, true}, true, false); }
uint8_t SM83::SRA_E() { return SHIFTLEFT({E, true}, true, false); }
uint8_t SM83::SRA_H() { return SHIFTLEFT({H, true}, true, false); }
uint8_t SM83::SRA_L() { return SHIFTLEFT({L, true}, true, false); }
uint8_t SM83::SRA_aHL() { return SHIFTLEFT({HL, false}, true, false); }
uint8_t SM83::SRA_A() { return SHIFTLEFT({A, true}, true, false); }
uint8_t SM83::SWAP_B() { return SWAP({B, true}); }
uint8_t SM83::SWAP_C() { return SWAP({C, true}); }
uint8_t SM83::SWAP_D() { return SWAP({D, true}); }
uint8_t SM83::SWAP_E() { return SWAP({E, true}); }
uint8_t SM83::SWAP_H() { return SWAP({H, true}); }
uint8_t SM83::SWAP_L() { return SWAP({L, true}); }
uint8_t SM83::SWAP_aHL() { return SWAP({HL, false}); }
uint8_t SM83::SWAP_A() { return SWAP({A, true}); }
uint8_t SM83::SRL_B() { return SHIFTLEFT({B, true}, true, true); }
uint8_t SM83::SRL_C() { return SHIFTLEFT({C, true}, true, true); }
uint8_t SM83::SRL_D() { return SHIFTLEFT({D, true}, true, true); }
uint8_t SM83::SRL_E() { return SHIFTLEFT({E, true}, true, true); }
uint8_t SM83::SRL_H() { return SHIFTLEFT({H, true}, true, true); }
uint8_t SM83::SRL_L() { return SHIFTLEFT({L, true}, true, true); }
uint8_t SM83::SRL_aHL() { return SHIFTLEFT({HL, false}, true, true); }
uint8_t SM83::SRL_A() { return SHIFTLEFT({A, true}, true, true); }
uint8_t SM83::BIT_0_B() { return BIT(0, {B, true}); }
uint8_t SM83::BIT_0_C() { return BIT(0, {C, true}); }
uint8_t SM83::BIT_0_D() { return BIT(0, {D, true}); }
uint8_t SM83::BIT_0_E() { return BIT(0, {E, true}); }
uint8_t SM83::BIT_0_H() { return BIT(0, {H, true}); }
uint8_t SM83::BIT_0_L() { return BIT(0, {L, true}); }
uint8_t SM83::BIT_0_aHL() { return BIT(0, {HL, false}); }
uint8_t SM83::BIT_0_A() { return BIT(0, {A, true}); }
uint8_t SM83::BIT_1_B() { return BIT(1, {B, true}); }
uint8_t SM83::BIT_1_C() { return BIT(1, {C, true}); }
uint8_t SM83::BIT_1_D() { return BIT(1, {D, true}); }
uint8_t SM83::BIT_1_E() { return BIT(1, {E, true}); }
uint8_t SM83::BIT_1_H() { return BIT(1, {H, true}); }
uint8_t SM83::BIT_1_L() { return BIT(1, {L, true}); }
uint8_t SM83::BIT_1_aHL() { return BIT(1, {HL, false}); }
uint8_t SM83::BIT_1_A() { return BIT(1, {A, true}); }
uint8_t SM83::BIT_2_B() { return BIT(2, {B, true}); }
uint8_t SM83::BIT_2_C() { return BIT(2, {C, true}); }
uint8_t SM83::BIT_2_D() { return BIT(2, {D, true}); }
uint8_t SM83::BIT_2_E() { return BIT(2, {E, true}); }
uint8_t SM83::BIT_2_H() { return BIT(2, {H, true}); }
uint8_t SM83::BIT_2_L() { return BIT(2, {L, true}); }
uint8_t SM83::BIT_2_aHL() { return BIT(2, {HL, false}); }
uint8_t SM83::BIT_2_A() { return BIT(2, {A, true}); }
uint8_t SM83::BIT_3_B() { return BIT(3, {B, true}); }
uint8_t SM83::BIT_3_C() { return BIT(3, {C, true}); }
uint8_t SM83::BIT_3_D() { return BIT(3, {D, true}); }
uint8_t SM83::BIT_3_E() { return BIT(3, {E, true}); }
uint8_t SM83::BIT_3_H() { return BIT(3, {H, true}); }
uint8_t SM83::BIT_3_L() { return BIT(3, {L, true}); }
uint8_t SM83::BIT_3_aHL() { return BIT(3, {HL, false}); }
uint8_t SM83::BIT_3_A() { return BIT(3, {A, true}); }
uint8_t SM83::BIT_4_B() { return BIT(4, {B, true}); }
uint8_t SM83::BIT_4_C() { return BIT(4, {C, true}); }
uint8_t SM83::BIT_4_D() { return BIT(4, {D, true}); }
uint8_t SM83::BIT_4_E() { return BIT(4, {E, true}); }
uint8_t SM83::BIT_4_H() { return BIT(4, {H, true}); }
uint8_t SM83::BIT_4_L() { return BIT(4, {L, true}); }
uint8_t SM83::BIT_4_aHL() { return BIT(4, {HL, false}); }
uint8_t SM83::BIT_4_A() { return BIT(4, {A, true}); }
uint8_t SM83::BIT_5_B() { return BIT(5, {B, true}); }
uint8_t SM83::BIT_5_C() { return BIT(5, {C, true}); }
uint8_t SM83::BIT_5_D() { return BIT(5, {D, true}); }
uint8_t SM83::BIT_5_E() { return BIT(5, {E, true}); }
uint8_t SM83::BIT_5_H() { return BIT(5, {H, true}); }
uint8_t SM83::BIT_5_L() { return BIT(5, {L, true}); }
uint8_t SM83::BIT_5_aHL() { return BIT(5, {HL, false}); }
uint8_t SM83::BIT_5_A() { return BIT(5, {A, true}); }
uint8_t SM83::BIT_6_B() { return BIT(6, {B, true}); }
uint8_t SM83::BIT_6_C() { return BIT(6, {C, true}); }
uint8_t SM83::BIT_6_D() { return BIT(6, {D, true}); }
uint8_t SM83::BIT_6_E() { return BIT(6, {E, true}); }
uint8_t SM83::BIT_6_H() { return BIT(6, {H, true}); }
uint8_t SM83::BIT_6_L() { return BIT(6, {L, true}); }
uint8_t SM83::BIT_6_aHL() { return BIT(6, {HL, false}); }
uint8_t SM83::BIT_6_A() { return BIT(6, {A, true}); }
uint8_t SM83::BIT_7_B() { return BIT(7, {B, true}); }
uint8_t SM83::BIT_7_C() { return BIT(7, {C, true}); }
uint8_t SM83::BIT_7_D() { return BIT(7, {D, true}); }
uint8_t SM83::BIT_7_E() { return BIT(7, {E, true}); }
uint8_t SM83::BIT_7_H() { return BIT(7, {H, true}); }
uint8_t SM83::BIT_7_L() { return BIT(7, {L, true}); }
uint8_t SM83::BIT_7_aHL() { return BIT(7, {HL, false}); }
uint8_t SM83::BIT_7_A() { return BIT(7, {A, true}); }
uint8_t SM83::RES_0_B() { return RES(0, {B, true}); }
uint8_t SM83::RES_0_C() { return RES(0, {C, true}); }
uint8_t SM83::RES_0_D() { return RES(0, {D, true}); }
uint8_t SM83::RES_0_E() { return RES(0, {E, true}); }
uint8_t SM83::RES_0_H() { return RES(0, {H, true}); }
uint8_t SM83::RES_0_L() { return RES(0, {L, true}); }
uint8_t SM83::RES_0_aHL() { return RES(0, {HL, false}); }
uint8_t SM83::RES_0_A() { return RES(0, {A, true}); }
uint8_t SM83::RES_1_B() { return RES(1, {B, true}); }
uint8_t SM83::RES_1_C() { return RES(1, {C, true}); }
uint8_t SM83::RES_1_D() { return RES(1, {D, true}); }
uint8_t SM83::RES_1_E() { return RES(1, {E, true}); }
uint8_t SM83::RES_1_H() { return RES(1, {H, true}); }
uint8_t SM83::RES_1_L() { return RES(1, {L, true}); }
uint8_t SM83::RES_1_aHL() { return RES(1, {HL, false}); }
uint8_t SM83::RES_1_A() { return RES(1, {A, true}); }
uint8_t SM83::RES_2_B() { return RES(2, {B, true}); }
uint8_t SM83::RES_2_C() { return RES(2, {C, true}); }
uint8_t SM83::RES_2_D() { return RES(2, {D, true}); }
uint8_t SM83::RES_2_E() { return RES(2, {E, true}); }
uint8_t SM83::RES_2_H() { return RES(2, {H, true}); }
uint8_t SM83::RES_2_L() { return RES(2, {L, true}); }
uint8_t SM83::RES_2_aHL() { return RES(2, {HL, false}); }
uint8_t SM83::RES_2_A() { return RES(2, {A, true}); }
uint8_t SM83::RES_3_B() { return RES(3, {B, true}); }
uint8_t SM83::RES_3_C() { return RES(3, {C, true}); }
uint8_t SM83::RES_3_D() { return RES(3, {D, true}); }
uint8_t SM83::RES_3_E() { return RES(3, {E, true}); }
uint8_t SM83::RES_3_H() { return RES(3, {H, true}); }
uint8_t SM83::RES_3_L() { return RES(3, {L, true}); }
uint8_t SM83::RES_3_aHL() { return RES(3, {HL, false}); }
uint8_t SM83::RES_3_A() { return RES(3, {A, true}); }
uint8_t SM83::RES_4_B() { return RES(4, {B, true}); }
uint8_t SM83::RES_4_C() { return RES(4, {C, true}); }
uint8_t SM83::RES_4_D() { return RES(4, {D, true}); }
uint8_t SM83::RES_4_E() { return RES(4, {E, true}); }
uint8_t SM83::RES_4_H() { return RES(4, {H, true}); }
uint8_t SM83::RES_4_L() { return RES(4, {L, true}); }
uint8_t SM83::RES_4_aHL() { return RES(4, {HL, false}); }
uint8_t SM83::RES_4_A() { return RES(4, {A, true}); }
uint8_t SM83::RES_5_B() { return RES(5, {B, true}); }
uint8_t SM83::RES_5_C() { return RES(5, {C, true}); }
uint8_t SM83::RES_5_D() { return RES(5, {D, true}); }
uint8_t SM83::RES_5_E() { return RES(5, {E, true}); }
uint8_t SM83::RES_5_H() { return RES(5, {H, true}); }
uint8_t SM83::RES_5_L() { return RES(5, {L, true}); }
uint8_t SM83::RES_5_aHL() { return RES(5, {HL, false}); }
uint8_t SM83::RES_5_A() { return RES(5, {A, true}); }
uint8_t SM83::RES_6_B() { return RES(6, {B, true}); }
uint8_t SM83::RES_6_C() { return RES(6, {C, true}); }
uint8_t SM83::RES_6_D() { return RES(6, {D, true}); }
uint8_t SM83::RES_6_E() { return RES(6, {E, true}); }
uint8_t SM83::RES_6_H() { return RES(6, {H, true}); }
uint8_t SM83::RES_6_L() { return RES(6, {L, true}); }
uint8_t SM83::RES_6_aHL() { return RES(6, {HL, false}); }
uint8_t SM83::RES_6_A() { return RES(6, {A, true}); }
uint8_t SM83::RES_7_B() { return RES(7, {B, true}); }
uint8_t SM83::RES_7_C() { return RES(7, {C, true}); }
uint8_t SM83::RES_7_D() { return RES(7, {D, true}); }
uint8_t SM83::RES_7_E() { return RES(7, {E, true}); }
uint8_t SM83::RES_7_H() { return RES(7, {H, true}); }
uint8_t SM83::RES_7_L() { return RES(7, {L, true}); }
uint8_t SM83::RES_7_aHL() { return RES(7, {HL, false}); }
uint8_t SM83::RES_7_A() { return RES(7, {A, true}); }
uint8_t SM83::SET_0_B() { return SET(0, {B, true}); }
uint8_t SM83::SET_0_C() { return SET(0, {C, true}); }
uint8_t SM83::SET_0_D() { return SET(0, {D, true}); }
uint8_t SM83::SET_0_E() { return SET(0, {E, true}); }
uint8_t SM83::SET_0_H() { return SET(0, {H, true}); }
uint8_t SM83::SET_0_L() { return SET(0, {L, true}); }
uint8_t SM83::SET_0_aHL() { return SET(0, {HL, false}); }
uint8_t SM83::SET_0_A() { return SET(0, {A, true}); }
uint8_t SM83::SET_1_B() { return SET(1, {B, true}); }
uint8_t SM83::SET_1_C() { return SET(1, {C, true}); }
uint8_t SM83::SET_1_D() { return SET(1, {D, true}); }
uint8_t SM83::SET_1_E() { return SET(1, {E, true}); }
uint8_t SM83::SET_1_H() { return SET(1, {H, true}); }
uint8_t SM83::SET_1_L() { return SET(1, {L, true}); }
uint8_t SM83::SET_1_aHL() { return SET(1, {HL, false}); }
uint8_t SM83::SET_1_A() { return SET(1, {A, true}); }
uint8_t SM83::SET_2_B() { return SET(2, {B, true}); }
uint8_t SM83::SET_2_C() { return SET(2, {C, true}); }
uint8_t SM83::SET_2_D() { return SET(2, {D, true}); }
uint8_t SM83::SET_2_E() { return SET(2, {E, true}); }
uint8_t SM83::SET_2_H() { return SET(2, {H, true}); }
uint8_t SM83::SET_2_L() { return SET(2, {L, true}); }
uint8_t SM83::SET_2_aHL() { return SET(2, {HL, false}); }
uint8_t SM83::SET_2_A() { return SET(2, {A, true}); }
uint8_t SM83::SET_3_B() { return SET(3, {B, true}); }
uint8_t SM83::SET_3_C() { return SET(3, {C, true}); }
uint8_t SM83::SET_3_D() { return SET(3, {D, true}); }
uint8_t SM83::SET_3_E() { return SET(3, {E, true}); }
uint8_t SM83::SET_3_H() { return SET(3, {H, true}); }
uint8_t SM83::SET_3_L() { return SET(3, {L, true}); }
uint8_t SM83::SET_3_aHL() { return SET(3, {HL, false}); }
uint8_t SM83::SET_3_A() { return SET(3, {A, true}); }
uint8_t SM83::SET_4_B() { return SET(4, {B, true}); }
uint8_t SM83::SET_4_C() { return SET(4, {C, true}); }
uint8_t SM83::SET_4_D() { return SET(4, {D, true}); }
uint8_t SM83::SET_4_E() { return SET(4, {E, true}); }
uint8_t SM83::SET_4_H() { return SET(4, {H, true}); }
uint8_t SM83::SET_4_L() { return SET(4, {L, true}); }
uint8_t SM83::SET_4_aHL() { return SET(4, {HL, false}); }
uint8_t SM83::SET_4_A() { return SET(4, {A, true}); }
uint8_t SM83::SET_5_B() { return SET(5, {B, true}); }
uint8_t SM83::SET_5_C() { return SET(5, {C, true}); }
uint8_t SM83::SET_5_D() { return SET(5, {D, true}); }
uint8_t SM83::SET_5_E() { return SET(5, {E, true}); }
uint8_t SM83::SET_5_H() { return SET(5, {H, true}); }
uint8_t SM83::SET_5_L() { return SET(5, {L, true}); }
uint8_t SM83::SET_5_aHL() { return SET(5, {HL, false}); }
uint8_t SM83::SET_5_A() { return SET(5, {A, true}); }
uint8_t SM83::SET_6_B() { return SET(6, {B, true}); }
uint8_t SM83::SET_6_C() { return SET(6, {C, true}); }
uint8_t SM83::SET_6_D() { return SET(6, {D, true}); }
uint8_t SM83::SET_6_E() { return SET(6, {E, true}); }
uint8_t SM83::SET_6_H() { return SET(6, {H, true}); }
uint8_t SM83::SET_6_L() { return SET(6, {L, true}); }
uint8_t SM83::SET_6_aHL() { return SET(6, {HL, false}); }
uint8_t SM83::SET_6_A() { return SET(6, {A, true}); }
uint8_t SM83::SET_7_B() { return SET(7, {B, true}); }
uint8_t SM83::SET_7_C() { return SET(7, {C, true}); }
uint8_t SM83::SET_7_D() { return SET(7, {D, true}); }
uint8_t SM83::SET_7_E() { return SET(7, {E, true}); }
uint8_t SM83::SET_7_H() { return SET(7, {H, true}); }
uint8_t SM83::SET_7_L() { return SET(7, {L, true}); }
uint8_t SM83::SET_7_aHL() { return SET(7, {HL, false}); }
uint8_t SM83::SET_7_A() { return SET(7, {A, true}); }
