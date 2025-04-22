#include "SM83.h"
#include "Bus.h"
#include <iostream>
#include <iomanip>

#ifdef DEBUGMODE_
#include<algorithm>
#endif

SM83::SM83() {
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
        {"RST", &x::RST_38, 4}
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
        {"SET", &x::SET_7_H, 2}, {"SET", &x::SET_7_L, 2}, {"SET", &x::SET_7_aHL, 4}, {"SET", &x::SET_7_A, 2}
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
        "LDH A a8", "POP AF", "LDH A C", "DI", "ILLEGAL_F4", "PUSH AF", "OR A n8", "RST $30", "LD HL SP e8", "LD SP HL", "LD A a16", "EI", "ILLEGAL_FC", "ILLEGAL_FD", "CP A n8", "RST $38"
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

#pragma region INTERRUPTS

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

#pragma region TIMER
void SM83::clockTimer() {
    timer_clock += 0x0001;
    div = (timer_clock >> 6) & 0xFF;

    uint8_t clock_select = tac & 0x3;
    uint8_t enabled = (tac >> 2) & 0x1;
    uint8_t div_factor;
    if (!enabled) 
        return;
    
    switch (clock_select) {
        case 0: div_factor = 256-1; break;
        case 1: div_factor = 4-1; break;
        case 2: div_factor = 16-1; break;
        case 3: div_factor = 64-1; break;
    }

    if ((timer_clock & div_factor) == 0) {
        ++tima;
    }

    if (tima == 0x00) { // if overflowed
        requestInterrupt(SM83::InterruptFlags::Timer);
        tima = tma;
    }
    
}


#pragma region CLOCK
void SM83::clock() {


    if (halted) {
        if (ie & if_) { // if interrupt pending
            halted = false;
        }
    }

    if (cycles == 0 && !halted) {
        
        ++debug_cycles_count;
        
        if (ime > 1) ime--; // this is part of IE instruction implementation. because it acts one instruction after the IE instruct.

        // if boot rom finished, unload it.
        if (0x100 <= pc and pc <= 0x102) {
            bus->bootrom = bus->cart; // lazy fix
            std::cout << "Boot rom unloaded" << std::endl;
            write(0xff50, 0x01);
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

        // if (last_executed_pc == 0x0040) {
        //     std::cout << "Breakpoint!\n";
        // }
        // if (last_executed_pc == 0x0064 && read(0xff44) == 0x0090) {
        //     std::cout << "Breakpoint!\n";
        // }

        uint16_t breakpoint = 0xc31d;

        #ifdef BREAKPOINT_PC_FILE
        std::ifstream infile("ROMS/breakpoint.pc");
        std::string hexStr;
        
        if (infile.is_open()) {
            // Read the hexadecimal number from the file
            infile >> hexStr;
            infile.close();

            // Convert the string to a uint16_t
            breakpoint = static_cast<uint16_t>(std::stoul(hexStr, nullptr, 16));
        }
        #endif

        // if (last_executed_pc == breakpoint) {
            // std::cout << "Breakpoint!\n";
        // }

        if (opcode == 0xcb) {
            opcode = read(pc++);
            additional_clock_cycles = (this->*prefixed_opcode_lookup[opcode].operate)();
            cycles = prefixed_opcode_lookup[opcode].cycles + additional_clock_cycles + prefixed_opcode_lookup[0xcb].cycles;
        } else {
            additional_clock_cycles = (this->*unprefixed_opcode_lookup[opcode].operate)();
            cycles = unprefixed_opcode_lookup[opcode].cycles + additional_clock_cycles;
        }
        
        #ifdef LOG_PC
        if (last_executed_pc >= 0xF1)
        logLastPC();
        #endif
        
        bus->cart.rectifyPttrWrites();
        handleInterrupts();
    }
    
    if ((timer_clock & (128-1)) == 0)
        bus->clockSerialTransfer();
    bus->joypad.update();
    clockTimer();
    if (!halted) 
        --cycles;
    // std::cout << "CPU CLOCKED" << std::endl;

}
#pragma endregion

#ifdef DEBUGMODE_

void SM83::opDebug() {
    pc = 0x160;
    for (uint8_t opcode = 0; opcode < 0x100; opcode++) {
        if (opcode == 0xcb) continue;
        (this->*unprefixed_opcode_lookup[opcode].operate)();
    }

}

#endif

#pragma region COMMS
uint8_t SM83::read(uint16_t addr) {
    return bus->cpuRead(addr);
}

void SM83::write(uint16_t addr, uint8_t data) {
    bus->cpuWrite(addr, data);
}


#pragma region INSTRUCTIONS







#include <SM83.instr.cpp>
#pragma endregion