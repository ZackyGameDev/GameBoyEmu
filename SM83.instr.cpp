#include <SM83rewrite.h>
#include<iostream>

uint8_t SM83::NOP() { return 0; }

uint8_t SM83::LD_BC_n16() {
    n16 = read(pc++);
    n16 |= read(pc++) << 8;
    b = n16 >> 8;
    b = (n16) & 0xff;
    return 0;
}

uint8_t SM83::LD_aBC_A() {
    write((b << 8) | c, a);
    return 0;
}

uint8_t SM83::INC_BC() { return 0; }

uint8_t SM83::INC_B() { return 0; }

uint8_t SM83::DEC_B() { return 0; }

uint8_t SM83::LD_B_n8() {
    n8 = read(pc++);
    b = n8;
    return 0;
}

uint8_t SM83::RLCA() { return 0; }

uint8_t SM83::LD_aa16_SP() {
    a16 = read(pc++);
    a16 |= read(pc++) << 8;
    write(a16, sp & 0xFF);
    write(a16 + 1, (sp >> 8) & 0xFF);
    return 0;
}


uint8_t SM83::ADD_HL_BC() { return 0; }

uint8_t SM83::LD_A_aBC() {
    a = read((b << 8) | c);
    return 0;
}

uint8_t SM83::DEC_BC() { return 0; }

uint8_t SM83::INC_C() { return 0; }

uint8_t SM83::DEC_C() { return 0; }

uint8_t SM83::LD_C_n8() {
    n8 = read(pc++);
    c = n8;
    return 0;
}

uint8_t SM83::RRCA() { return 0; }

uint8_t SM83::STOP_n8() { pc++; return 0; }

uint8_t SM83::LD_DE_n16() {
    n16 = read(pc++);
    n16 |= read(pc++) << 8;
    d = n16 >> 8;
    d = (n16) & 0xff;
    return 0;
}

uint8_t SM83::LD_aDE_A() {
    write((d << 8) | e, a);
    return 0;
}

uint8_t SM83::INC_DE() { return 0; }

uint8_t SM83::INC_D() { return 0; }

uint8_t SM83::DEC_D() { return 0; }

uint8_t SM83::LD_D_n8() {
    n8 = read(pc++);
    d = n8;
    return 0;
}

uint8_t SM83::RLA() { return 0; }

uint8_t SM83::JR_e8() {
    e8 = toSigned(read(pc++));
    pc += e8;
    return 0;
}

uint8_t SM83::ADD_HL_DE() { return 0; }

uint8_t SM83::LD_A_aDE() {
    a = read((d << 8) | e);
    return 0;
}

uint8_t SM83::DEC_DE() { return 0; }

uint8_t SM83::INC_E() { return 0; }

uint8_t SM83::DEC_E() { return 0; }

uint8_t SM83::LD_E_n8() {
    n8 = read(pc++);
    e = n8;
    return 0;
}

uint8_t SM83::RRA() { return 0; }

uint8_t SM83::JR_NZ_e8() {
    if (not !getFlag(fz)) {
        pc += 1;
        return 0;
    }
    e8 = toSigned(read(pc++));
    pc += e8;
    return 1;
}

uint8_t SM83::LD_HL_n16() {
    n16 = read(pc++);
    n16 |= read(pc++) << 8;
    h = n16 >> 8;
    h = (n16) & 0xff;
    return 0;
}

uint8_t SM83::LD_aHLI_A() {
    write((h << 8) | l, a);
    addToHiLo(h, l, 1);
    return 0;
}

uint8_t SM83::INC_HL() { return 0; }

uint8_t SM83::INC_H() { return 0; }

uint8_t SM83::DEC_H() { return 0; }

uint8_t SM83::LD_H_n8() {
    n8 = read(pc++);
    h = n8;
    return 0;
}

uint8_t SM83::DAA() { return 0; }

uint8_t SM83::JR_Z_e8() {
    if (not getFlag(fz)) {
        pc += 1;
        return 0;
    }
    e8 = toSigned(read(pc++));
    pc += e8;
    return 1;
}

uint8_t SM83::ADD_HL_HL() { return 0; }

uint8_t SM83::LD_A_aHLI() {
    a = read((h << 8) | l);
    addToHiLo(h, l, 1);
    return 0;
}

uint8_t SM83::DEC_HL() { return 0; }

uint8_t SM83::INC_L() { return 0; }

uint8_t SM83::DEC_L() { return 0; }

uint8_t SM83::LD_L_n8() {
    n8 = read(pc++);
    l = n8;
    return 0;
}

uint8_t SM83::CPL() { return 0; }

uint8_t SM83::JR_NC_e8() {
    if (not !getFlag(fc)) {
        pc += 1;
        return 0;
    }
    e8 = toSigned(read(pc++));
    pc += e8;
    return 1;
}

uint8_t SM83::LD_SP_n16() {
    n16 = read(pc++);
    n16 |= read(pc++) << 8;
    sp = n16;
    return 0;
}

uint8_t SM83::LD_aHLD_A() {
    write((h << 8) | l, a);
    addToHiLo(h, l, -1);
    return 0;
}

uint8_t SM83::INC_SP() { return 0; }

uint8_t SM83::INC_aHL() { return 0; }

uint8_t SM83::DEC_aHL() { return 0; }

uint8_t SM83::LD_aHL_n8() {
    n8 = read(pc++);
    write((h << 8) | l, n8);
    return 0;
}

uint8_t SM83::SCF() { return 0; }

uint8_t SM83::JR_C_e8() {
    if (not getFlag(fc)) {
        pc += 1;
        return 0;
    }
    e8 = toSigned(read(pc++));
    pc += e8;
    return 1;
}

uint8_t SM83::ADD_HL_SP() { return 0; }

uint8_t SM83::LD_A_aHLD() {
    a = read((h << 8) | l);
    addToHiLo(h, l, -1);
    return 0;
}

uint8_t SM83::DEC_SP() { return 0; }

uint8_t SM83::INC_A() { return 0; }

uint8_t SM83::DEC_A() { return 0; }

uint8_t SM83::LD_A_n8() {
    n8 = read(pc++);
    a = n8;
    return 0;
}

uint8_t SM83::CCF() { return 0; }

uint8_t SM83::LD_B_B() {
    b = b;
    return 0;
}

uint8_t SM83::LD_B_C() {
    b = c;
    return 0;
}

uint8_t SM83::LD_B_D() {
    b = d;
    return 0;
}

uint8_t SM83::LD_B_E() {
    b = e;
    return 0;
}

uint8_t SM83::LD_B_H() {
    b = h;
    return 0;
}

uint8_t SM83::LD_B_L() {
    b = l;
    return 0;
}

uint8_t SM83::LD_B_aHL() {
    b = read((h << 8) | l);
    return 0;
}

uint8_t SM83::LD_B_A() {
    b = a;
    return 0;
}

uint8_t SM83::LD_C_B() {
    c = b;
    return 0;
}

uint8_t SM83::LD_C_C() {
    c = c;
    return 0;
}

uint8_t SM83::LD_C_D() {
    c = d;
    return 0;
}

uint8_t SM83::LD_C_E() {
    c = e;
    return 0;
}

uint8_t SM83::LD_C_H() {
    c = h;
    return 0;
}

uint8_t SM83::LD_C_L() {
    c = l;
    return 0;
}

uint8_t SM83::LD_C_aHL() {
    c = read((h << 8) | l);
    return 0;
}

uint8_t SM83::LD_C_A() {
    c = a;
    return 0;
}

uint8_t SM83::LD_D_B() {
    d = b;
    return 0;
}

uint8_t SM83::LD_D_C() {
    d = c;
    return 0;
}

uint8_t SM83::LD_D_D() {
    d = d;
    return 0;
}

uint8_t SM83::LD_D_E() {
    d = e;
    return 0;
}

uint8_t SM83::LD_D_H() {
    d = h;
    return 0;
}

uint8_t SM83::LD_D_L() {
    d = l;
    return 0;
}

uint8_t SM83::LD_D_aHL() {
    d = read((h << 8) | l);
    return 0;
}

uint8_t SM83::LD_D_A() {
    d = a;
    return 0;
}

uint8_t SM83::LD_E_B() {
    e = b;
    return 0;
}

uint8_t SM83::LD_E_C() {
    e = c;
    return 0;
}

uint8_t SM83::LD_E_D() {
    e = d;
    return 0;
}

uint8_t SM83::LD_E_E() {
    e = e;
    return 0;
}

uint8_t SM83::LD_E_H() {
    e = h;
    return 0;
}

uint8_t SM83::LD_E_L() {
    e = l;
    return 0;
}

uint8_t SM83::LD_E_aHL() {
    e = read((h << 8) | l);
    return 0;
}

uint8_t SM83::LD_E_A() {
    e = a;
    return 0;
}

uint8_t SM83::LD_H_B() {
    h = b;
    return 0;
}

uint8_t SM83::LD_H_C() {
    h = c;
    return 0;
}

uint8_t SM83::LD_H_D() {
    h = d;
    return 0;
}

uint8_t SM83::LD_H_E() {
    h = e;
    return 0;
}

uint8_t SM83::LD_H_H() {
    h = h;
    return 0;
}

uint8_t SM83::LD_H_L() {
    h = l;
    return 0;
}

uint8_t SM83::LD_H_aHL() {
    h = read((h << 8) | l);
    return 0;
}

uint8_t SM83::LD_H_A() {
    h = a;
    return 0;
}

uint8_t SM83::LD_L_B() {
    l = b;
    return 0;
}

uint8_t SM83::LD_L_C() {
    l = c;
    return 0;
}

uint8_t SM83::LD_L_D() {
    l = d;
    return 0;
}

uint8_t SM83::LD_L_E() {
    l = e;
    return 0;
}

uint8_t SM83::LD_L_H() {
    l = h;
    return 0;
}

uint8_t SM83::LD_L_L() {
    l = l;
    return 0;
}

uint8_t SM83::LD_L_aHL() {
    l = read((h << 8) | l);
    return 0;
}

uint8_t SM83::LD_L_A() {
    l = a;
    return 0;
}

uint8_t SM83::LD_aHL_B() {
    write((h << 8) | l, b);
    return 0;
}

uint8_t SM83::LD_aHL_C() {
    write((h << 8) | l, c);
    return 0;
}

uint8_t SM83::LD_aHL_D() {
    write((h << 8) | l, d);
    return 0;
}

uint8_t SM83::LD_aHL_E() {
    write((h << 8) | l, e);
    return 0;
}

uint8_t SM83::LD_aHL_H() {
    write((h << 8) | l, h);
    return 0;
}

uint8_t SM83::LD_aHL_L() {
    write((h << 8) | l, l);
    return 0;
}

uint8_t SM83::HALT() { std::cout << "SYSTEM HALT REQUESTED!"; return 0; }

uint8_t SM83::LD_aHL_A() {
    write((h << 8) | l, a);
    return 0;
}

uint8_t SM83::LD_A_B() {
    a = b;
    return 0;
}

uint8_t SM83::LD_A_C() {
    a = c;
    return 0;
}

uint8_t SM83::LD_A_D() {
    a = d;
    return 0;
}

uint8_t SM83::LD_A_E() {
    a = e;
    return 0;
}

uint8_t SM83::LD_A_H() {
    a = h;
    return 0;
}

uint8_t SM83::LD_A_L() {
    a = l;
    return 0;
}

uint8_t SM83::LD_A_aHL() {
    a = read((h << 8) | l);
    return 0;
}

uint8_t SM83::LD_A_A() {
    a = a;
    return 0;
}

uint8_t SM83::ADD_A_B() { return 0; }

uint8_t SM83::ADD_A_C() { return 0; }

uint8_t SM83::ADD_A_D() { return 0; }

uint8_t SM83::ADD_A_E() { return 0; }

uint8_t SM83::ADD_A_H() { return 0; }

uint8_t SM83::ADD_A_L() { return 0; }

uint8_t SM83::ADD_A_aHL() { return 0; }

uint8_t SM83::ADD_A_A() { return 0; }

uint8_t SM83::ADC_A_B() { return 0; }

uint8_t SM83::ADC_A_C() { return 0; }

uint8_t SM83::ADC_A_D() { return 0; }

uint8_t SM83::ADC_A_E() { return 0; }

uint8_t SM83::ADC_A_H() { return 0; }

uint8_t SM83::ADC_A_L() { return 0; }

uint8_t SM83::ADC_A_aHL() { return 0; }

uint8_t SM83::ADC_A_A() { return 0; }

uint8_t SM83::SUB_A_B() { return 0; }

uint8_t SM83::SUB_A_C() { return 0; }

uint8_t SM83::SUB_A_D() { return 0; }

uint8_t SM83::SUB_A_E() { return 0; }

uint8_t SM83::SUB_A_H() { return 0; }

uint8_t SM83::SUB_A_L() { return 0; }

uint8_t SM83::SUB_A_aHL() { return 0; }

uint8_t SM83::SUB_A_A() { return 0; }

uint8_t SM83::SBC_A_B() { return 0; }

uint8_t SM83::SBC_A_C() { return 0; }

uint8_t SM83::SBC_A_D() { return 0; }

uint8_t SM83::SBC_A_E() { return 0; }

uint8_t SM83::SBC_A_H() { return 0; }

uint8_t SM83::SBC_A_L() { return 0; }

uint8_t SM83::SBC_A_aHL() { return 0; }

uint8_t SM83::SBC_A_A() { return 0; }

uint8_t SM83::AND_A_B() { return 0; }

uint8_t SM83::AND_A_C() { return 0; }

uint8_t SM83::AND_A_D() { return 0; }

uint8_t SM83::AND_A_E() { return 0; }

uint8_t SM83::AND_A_H() { return 0; }

uint8_t SM83::AND_A_L() { return 0; }

uint8_t SM83::AND_A_aHL() { return 0; }

uint8_t SM83::AND_A_A() { return 0; }

uint8_t SM83::XOR_A_B() { return 0; }

uint8_t SM83::XOR_A_C() { return 0; }

uint8_t SM83::XOR_A_D() { return 0; }

uint8_t SM83::XOR_A_E() { return 0; }

uint8_t SM83::XOR_A_H() { return 0; }

uint8_t SM83::XOR_A_L() { return 0; }

uint8_t SM83::XOR_A_aHL() { return 0; }

uint8_t SM83::XOR_A_A() { return 0; }

uint8_t SM83::OR_A_B() { return 0; }

uint8_t SM83::OR_A_C() { return 0; }

uint8_t SM83::OR_A_D() { return 0; }

uint8_t SM83::OR_A_E() { return 0; }

uint8_t SM83::OR_A_H() { return 0; }

uint8_t SM83::OR_A_L() { return 0; }

uint8_t SM83::OR_A_aHL() { return 0; }

uint8_t SM83::OR_A_A() { return 0; }

uint8_t SM83::CP_A_B() { return 0; }

uint8_t SM83::CP_A_C() { return 0; }

uint8_t SM83::CP_A_D() { return 0; }

uint8_t SM83::CP_A_E() { return 0; }

uint8_t SM83::CP_A_H() { return 0; }

uint8_t SM83::CP_A_L() { return 0; }

uint8_t SM83::CP_A_aHL() { return 0; }

uint8_t SM83::CP_A_A() { return 0; }

uint8_t SM83::RET_NZ() {
    if (not !getFlag(fz)) {
        return 0;
    }
    pc = read(sp++);
    pc |= read(sp++) << 8;
    return 3;
}

uint8_t SM83::POP_BC() {
    b = read(sp++);
    return 0;
    return 0;
}

uint8_t SM83::JP_NZ_a16() {
    if (not !getFlag(fz)) {
        pc += 2;
        return 0;
    }
    a16 = read(pc++);
    a16 |= read(pc++) << 8;
    pc = a16;
    return 1;
    return 0;
}

uint8_t SM83::JP_a16() {
    a16 = read(pc++);
    a16 |= read(pc++) << 8;
    pc = a16;
    return 0;
    return 0;
}

uint8_t SM83::CALL_NZ_a16() {
    if (not !getFlag(fz)) {
        pc += 2;
        return 0;
    }
    write(--sp, pc >> 8);
    write(--sp, pc & 0xff);
    a16 = read(pc++);
    a16 |= read(pc++) << 8;
    pc = a16;
    return 3;
}

uint8_t SM83::PUSH_BC() {
    write(--sp, b);
    write(--sp, c);
    return 0;
    return 0;
}

uint8_t SM83::ADD_A_n8() { return 0; }

uint8_t SM83::RST_00() {
    write(--sp, pc >> 8);
    write(--sp, pc & 0xff);
    pc = 0x00;
    return 0;

}

uint8_t SM83::RET_Z() {
    if (not getFlag(fz)) {
        return 0;
    }
    pc = read(sp++);
    pc |= read(sp++) << 8;
    return 3;
}

uint8_t SM83::RET() { return 0; }

uint8_t SM83::JP_Z_a16() {
    if (not getFlag(fz)) {
        pc += 2;
        return 0;
    }
    a16 = read(pc++);
    a16 |= read(pc++) << 8;
    pc = a16;
    return 1;
    return 0;
}

uint8_t SM83::PREFIX() { return 0; }

uint8_t SM83::CALL_Z_a16() {
    if (not getFlag(fz)) {
        pc += 2;
        return 0;
    }
    write(--sp, pc >> 8);
    write(--sp, pc & 0xff);
    a16 = read(pc++);
    a16 |= read(pc++) << 8;
    pc = a16;
    return 3;
}

uint8_t SM83::CALL_a16() {
    a16 = read(pc++);
    a16 |= read(pc++) << 8;
    pc = a16;
    return 0;
}

uint8_t SM83::ADC_A_n8() { return 0; }

uint8_t SM83::RST_08() {
    write(--sp, pc >> 8);
    write(--sp, pc & 0xff);
    pc = 0x08;
    return 0;

}

uint8_t SM83::RET_NC() {
    if (not !getFlag(fc)) {
        return 0;
    }
    pc = read(sp++);
    pc |= read(sp++) << 8;
    return 3;
}

uint8_t SM83::POP_DE() {
    d = read(sp++);
    return 0;
    return 0;
}

uint8_t SM83::JP_NC_a16() {
    if (not !getFlag(fc)) {
        pc += 2;
        return 0;
    }
    a16 = read(pc++);
    a16 |= read(pc++) << 8;
    pc = a16;
    return 1;
    return 0;
}

uint8_t SM83::ILLEGAL_D3() { return 0; }

uint8_t SM83::CALL_NC_a16() {
    if (not !getFlag(fc)) {
        pc += 2;
        return 0;
    }
    write(--sp, pc >> 8);
    write(--sp, pc & 0xff);
    a16 = read(pc++);
    a16 |= read(pc++) << 8;
    pc = a16;
    return 3;
}

uint8_t SM83::PUSH_DE() {
    write(--sp, d);
    write(--sp, e);
    return 0;
    return 0;
}

uint8_t SM83::SUB_A_n8() { return 0; }

uint8_t SM83::RST_10() {
    write(--sp, pc >> 8);
    write(--sp, pc & 0xff);
    pc = 0x10;
    return 0;

}

uint8_t SM83::RET_C() {
    if (not getFlag(fc)) {
        return 0;
    }
    pc = read(sp++);
    pc |= read(sp++) << 8;
    return 3;
}

uint8_t SM83::RETI() {
    ime = 2;
    pc = read(sp++);
    pc |= read(sp++) << 8;
    return 0;

}

uint8_t SM83::JP_C_a16() {
    if (not getFlag(fc)) {
        pc += 2;
        return 0;
    }
    a16 = read(pc++);
    a16 |= read(pc++) << 8;
    pc = a16;
    return 1;
    return 0;
}

uint8_t SM83::ILLEGAL_DB() { return 0; }

uint8_t SM83::CALL_C_a16() {
    if (not getFlag(fc)) {
        pc += 2;
        return 0;
    }
    write(--sp, pc >> 8);
    write(--sp, pc & 0xff);
    a16 = read(pc++);
    a16 |= read(pc++) << 8;
    pc = a16;
    return 3;
}

uint8_t SM83::ILLEGAL_DD() { return 0; }

uint8_t SM83::SBC_A_n8() { return 0; }

uint8_t SM83::RST_18() {
    write(--sp, pc >> 8);
    write(--sp, pc & 0xff);
    pc = 0x18;
    return 0;

}

uint8_t SM83::LDH_aa8_A() {
    write(0xff00 | a8, a);
    return 0;
}

uint8_t SM83::POP_HL() {
    h = read(sp++);
    return 0;
    return 0;
}

uint8_t SM83::LDH_aC_A() {
    write(0xff00 | c, a);
    return 0;
}

uint8_t SM83::ILLEGAL_E3() { return 0; }

uint8_t SM83::ILLEGAL_E4() { return 0; }

uint8_t SM83::PUSH_HL() {
    write(--sp, h);
    write(--sp, l);
    return 0;
    return 0;
}

uint8_t SM83::AND_A_n8() { return 0; }

uint8_t SM83::RST_20() {
    write(--sp, pc >> 8);
    write(--sp, pc & 0xff);
    pc = 0x20;
    return 0;

}

uint8_t SM83::ADD_SP_e8() { return 0; }

uint8_t SM83::JP_HL() {
    pc = l;
    pc |= h << 8;
    return 0;
    return 0;
}

uint8_t SM83::LD_aa16_A() {
    write((a << 8) | 1, a);
    return 0;
}

uint8_t SM83::ILLEGAL_EB() { return 0; }

uint8_t SM83::ILLEGAL_EC() { return 0; }

uint8_t SM83::ILLEGAL_ED() { return 0; }

uint8_t SM83::XOR_A_n8() { return 0; }

uint8_t SM83::RST_28() {
    write(--sp, pc >> 8);
    write(--sp, pc & 0xff);
    pc = 0x28;
    return 0;

}

uint8_t SM83::LDH_A_aa8() {
    a8 = read(pc++);
    a = read(0xff00 | a8);
    return 0;
}

uint8_t SM83::POP_AF() {
    a = read(sp++);
    return 0;
    return 0;
}

uint8_t SM83::LDH_A_aC() {
    a = read(0xff00 | c);
    return 0;
}

uint8_t SM83::DI() { ime = 0; return 0; }

uint8_t SM83::ILLEGAL_F4() { return 0; }

uint8_t SM83::PUSH_AF() {
    write(--sp, a);
    write(--sp, f & 0xf0);
    return 0;
    return 0;
}

uint8_t SM83::OR_A_n8() { return 0; }

uint8_t SM83::RST_30() {
    write(--sp, pc >> 8);
    write(--sp, pc & 0xff);
    pc = 0x30;
    return 0;

}

uint8_t SM83::LD_HL_SPI_e8() {

    e8 = toSigned(read(pc++));
    
    // flags
    setFlag(fz, 0);
    setFlag(fn, 0);
    setFlag( fh, ((sp & 0x0F) + (e8 & 0x0F)) & 0x10 );
    setFlag( fc, ((sp & 0xFF) + (e8 & 0xFF)) & 0x100 );

    h = (sp + e8) >> 8;
    l = (sp + e8) & 0xff; 
    return 0;
}

uint8_t SM83::LD_SP_HL() {
    sp = l;
    sp |= h << 8; 
    return 0;
}

uint8_t SM83::LD_A_aa16() {
    a16 = read(pc++);
    a16 |= read(pc++) << 8;
    a = read(a16);
    return 0;
}

uint8_t SM83::EI() { ime = 2; return 0; }

uint8_t SM83::ILLEGAL_FC() { return 0; }

uint8_t SM83::ILLEGAL_FD() { return 0; }

uint8_t SM83::CP_A_n8() { return 0; }

uint8_t SM83::RST_38() {
    write(--sp, pc >> 8);
    write(--sp, pc & 0xff);
    pc = 0x38;
    return 0;

}

