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

uint8_t SM83::INC_BC() {
    addToHiLo(b, c, 1);
    return 0;
}

uint8_t SM83::INC_B() {
    setFlag(fh, ((b & 0x0f) + 0x01) & 0x10);
    setFlag(fn, 0);
    b += 1; 
    setFlag(fz, b == 0);
    return 0;
}

uint8_t SM83::DEC_B() {
    b -= 1;
    setFlag(fn, 1);
    setFlag(fh, (b & 0x0F) == 0x0F);
    setFlag(fz, b == 0);
    return 0;
}

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


uint8_t SM83::ADD_HL_BC() {
    n32 = ((h << 8) | l) + ((b << 8) | c);
    setFlag(fh, ((((h << 8) | l) & 0xFFF) + (((b << 8) | c) & 0xFFF)) & 0x1000);
    setFlag(fc, n32 > 0xFFFF);
    setFlag(fn, 0);
    n32 = n32 & 0xFFFF;
    h = (n32 >> 8) & 0xFF;
    l = n32 & 0xFF;
}

uint8_t SM83::LD_A_aBC() {
    a = read((b << 8) | c);
    return 0;
}

uint8_t SM83::DEC_BC() {
    addToHiLo(b, c, -1);
    return 0;
}

uint8_t SM83::INC_C() {
    setFlag(fh, ((c & 0x0f) + 0x01) & 0x10);
    setFlag(fn, 0);
    c += 1; 
    setFlag(fz, c == 0);
    return 0;
}

uint8_t SM83::DEC_C() {
    c -= 1;
    setFlag(fn, 1);
    setFlag(fh, (c & 0x0F) == 0x0F);
    setFlag(fz, c == 0);
    return 0;
}

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

uint8_t SM83::INC_DE() {
    addToHiLo(d, e, 1);
    return 0;
}

uint8_t SM83::INC_D() {
    setFlag(fh, ((d & 0x0f) + 0x01) & 0x10);
    setFlag(fn, 0);
    d += 1; 
    setFlag(fz, d == 0);
    return 0;
}

uint8_t SM83::DEC_D() {
    d -= 1;
    setFlag(fn, 1);
    setFlag(fh, (d & 0x0F) == 0x0F);
    setFlag(fz, d == 0);
    return 0;
}

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

uint8_t SM83::ADD_HL_DE() {
    n32 = ((h << 8) | l) + ((d << 8) | e);
    setFlag(fh, ((((h << 8) | l) & 0xFFF) + (((d << 8) | e) & 0xFFF)) & 0x1000);
    setFlag(fc, n32 > 0xFFFF);
    setFlag(fn, 0);
    n32 = n32 & 0xFFFF;
    h = (n32 >> 8) & 0xFF;
    l = n32 & 0xFF;
}

uint8_t SM83::LD_A_aDE() {
    a = read((d << 8) | e);
    return 0;
}

uint8_t SM83::DEC_DE() {
    addToHiLo(d, e, -1);
    return 0;
}

uint8_t SM83::INC_E() {
    setFlag(fh, ((e & 0x0f) + 0x01) & 0x10);
    setFlag(fn, 0);
    e += 1; 
    setFlag(fz, e == 0);
    return 0;
}

uint8_t SM83::DEC_E() {
    e -= 1;
    setFlag(fn, 1);
    setFlag(fh, (e & 0x0F) == 0x0F);
    setFlag(fz, e == 0);
    return 0;
}

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

uint8_t SM83::INC_HL() {
    addToHiLo(h, l, 1);
    return 0;
}

uint8_t SM83::INC_H() {
    setFlag(fh, ((h & 0x0f) + 0x01) & 0x10);
    setFlag(fn, 0);
    h += 1; 
    setFlag(fz, h == 0);
    return 0;
}

uint8_t SM83::DEC_H() {
    h -= 1;
    setFlag(fn, 1);
    setFlag(fh, (h & 0x0F) == 0x0F);
    setFlag(fz, h == 0);
    return 0;
}

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

uint8_t SM83::ADD_HL_HL() {
    n32 = ((h << 8) | l) + ((h << 8) | l);
    setFlag(fh, ((((h << 8) | l) & 0xFFF) + (((h << 8) | l) & 0xFFF)) & 0x1000);
    setFlag(fc, n32 > 0xFFFF);
    setFlag(fn, 0);
    n32 = n32 & 0xFFFF;
    h = (n32 >> 8) & 0xFF;
    l = n32 & 0xFF;
}

uint8_t SM83::LD_A_aHLI() {
    a = read((h << 8) | l);
    addToHiLo(h, l, 1);
    return 0;
}

uint8_t SM83::DEC_HL() {
    addToHiLo(h, l, -1);
    return 0;
}

uint8_t SM83::INC_L() {
    setFlag(fh, ((l & 0x0f) + 0x01) & 0x10);
    setFlag(fn, 0);
    l += 1; 
    setFlag(fz, l == 0);
    return 0;
}

uint8_t SM83::DEC_L() {
    l -= 1;
    setFlag(fn, 1);
    setFlag(fh, (l & 0x0F) == 0x0F);
    setFlag(fz, l == 0);
    return 0;
}

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

uint8_t SM83::INC_SP() {
    sp += 1;
    return 0;
}

uint8_t SM83::INC_aHL() {
    n8 = read((h << 8) | l);
    setFlag(fh, ((n8 & 0x0f) + 0x01) & 0x10);
    setFlag(fn, 0);
    n8 += 1;
    write((h << 8) | l, n8); 
    setFlag(fz, n8 == 0);
    return 0;
}

uint8_t SM83::DEC_aHL() {
    n8 = read((h << 8) | l);
    n8 -= 1;
    write((h << 8) | l, n8);
    setFlag(fn, 1);
    setFlag(fh, (n8 & 0x0F) == 0x0F);
    setFlag(fz, n8 == 0);
    return 0;
}

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

uint8_t SM83::ADD_HL_SP() {
    n32 = ((h << 8) | l) + sp;
    setFlag(fh, ((((h << 8) | l) & 0xFFF) + (sp & 0xFFF)) & 0x1000);
    setFlag(fc, n32 > 0xFFFF);
    setFlag(fn, 0);
    n32 = n32 & 0xFFFF;
    h = (n32 >> 8) & 0xFF;
    l = n32 & 0xFF;
}

uint8_t SM83::LD_A_aHLD() {
    a = read((h << 8) | l);
    addToHiLo(h, l, -1);
    return 0;
}

uint8_t SM83::DEC_SP() {
    sp -= 1;
    return 0;
}

uint8_t SM83::INC_A() {
    setFlag(fh, ((a & 0x0f) + 0x01) & 0x10);
    setFlag(fn, 0);
    a += 1; 
    setFlag(fz, a == 0);
    return 0;
}

uint8_t SM83::DEC_A() {
    a -= 1;
    setFlag(fn, 1);
    setFlag(fh, (a & 0x0F) == 0x0F);
    setFlag(fz, a == 0);
    return 0;
}

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

uint8_t SM83::ADD_A_B() {
    n32 = a + b;
    setFlag(fn, 0);
    setFlag(fh, ((a & 0x0F) + (b & 0x0F)) & 0x10);
    setFlag(fc, n32 & 0x0100);
    setFlag(fz, (a & 0xff) == 0);
    a = n32 & 0xff;
    return 0;
}

uint8_t SM83::ADD_A_C() {
    n32 = a + c;
    setFlag(fn, 0);
    setFlag(fh, ((a & 0x0F) + (c & 0x0F)) & 0x10);
    setFlag(fc, n32 & 0x0100);
    setFlag(fz, (a & 0xff) == 0);
    a = n32 & 0xff;
    return 0;
}

uint8_t SM83::ADD_A_D() {
    n32 = a + d;
    setFlag(fn, 0);
    setFlag(fh, ((a & 0x0F) + (d & 0x0F)) & 0x10);
    setFlag(fc, n32 & 0x0100);
    setFlag(fz, (a & 0xff) == 0);
    a = n32 & 0xff;
    return 0;
}

uint8_t SM83::ADD_A_E() {
    n32 = a + e;
    setFlag(fn, 0);
    setFlag(fh, ((a & 0x0F) + (e & 0x0F)) & 0x10);
    setFlag(fc, n32 & 0x0100);
    setFlag(fz, (a & 0xff) == 0);
    a = n32 & 0xff;
    return 0;
}

uint8_t SM83::ADD_A_H() {
    n32 = a + h;
    setFlag(fn, 0);
    setFlag(fh, ((a & 0x0F) + (h & 0x0F)) & 0x10);
    setFlag(fc, n32 & 0x0100);
    setFlag(fz, (a & 0xff) == 0);
    a = n32 & 0xff;
    return 0;
}

uint8_t SM83::ADD_A_L() {
    n32 = a + l;
    setFlag(fn, 0);
    setFlag(fh, ((a & 0x0F) + (l & 0x0F)) & 0x10);
    setFlag(fc, n32 & 0x0100);
    setFlag(fz, (a & 0xff) == 0);
    a = n32 & 0xff;
    return 0;
}

uint8_t SM83::ADD_A_aHL() {
    n32 = a + read((h << 8) | l);
    setFlag(fn, 0);
    setFlag(fh, ((a & 0x0F) + (read((h << 8) | l) & 0x0F)) & 0x10);
    setFlag(fc, n32 & 0x0100);
    setFlag(fz, (a & 0xff) == 0);
    a = n32 & 0xff;
    return 0;
}

uint8_t SM83::ADD_A_A() {
    n32 = a + a;
    setFlag(fn, 0);
    setFlag(fh, ((a & 0x0F) + (a & 0x0F)) & 0x10);
    setFlag(fc, n32 & 0x0100);
    setFlag(fz, (a & 0xff) == 0);
    a = n32 & 0xff;
    return 0;
}

uint8_t SM83::ADC_A_B() {
    n32 = a + b + getFlag(fc);
    setFlag(fn, 0);
    setFlag(fh, ((a & 0x0F) + (b & 0x0F) + getFlag(fc)) & 0x10);
    setFlag(fc, n32 & 0x0100);
    setFlag(fz, (a & 0xff) == 0);
    a = n32 & 0xff;
    return 0;
}

uint8_t SM83::ADC_A_C() {
    n32 = a + c + getFlag(fc);
    setFlag(fn, 0);
    setFlag(fh, ((a & 0x0F) + (c & 0x0F) + getFlag(fc)) & 0x10);
    setFlag(fc, n32 & 0x0100);
    setFlag(fz, (a & 0xff) == 0);
    a = n32 & 0xff;
    return 0;
}

uint8_t SM83::ADC_A_D() {
    n32 = a + d + getFlag(fc);
    setFlag(fn, 0);
    setFlag(fh, ((a & 0x0F) + (d & 0x0F) + getFlag(fc)) & 0x10);
    setFlag(fc, n32 & 0x0100);
    setFlag(fz, (a & 0xff) == 0);
    a = n32 & 0xff;
    return 0;
}

uint8_t SM83::ADC_A_E() {
    n32 = a + e + getFlag(fc);
    setFlag(fn, 0);
    setFlag(fh, ((a & 0x0F) + (e & 0x0F) + getFlag(fc)) & 0x10);
    setFlag(fc, n32 & 0x0100);
    setFlag(fz, (a & 0xff) == 0);
    a = n32 & 0xff;
    return 0;
}

uint8_t SM83::ADC_A_H() {
    n32 = a + h + getFlag(fc);
    setFlag(fn, 0);
    setFlag(fh, ((a & 0x0F) + (h & 0x0F) + getFlag(fc)) & 0x10);
    setFlag(fc, n32 & 0x0100);
    setFlag(fz, (a & 0xff) == 0);
    a = n32 & 0xff;
    return 0;
}

uint8_t SM83::ADC_A_L() {
    n32 = a + l + getFlag(fc);
    setFlag(fn, 0);
    setFlag(fh, ((a & 0x0F) + (l & 0x0F) + getFlag(fc)) & 0x10);
    setFlag(fc, n32 & 0x0100);
    setFlag(fz, (a & 0xff) == 0);
    a = n32 & 0xff;
    return 0;
}

uint8_t SM83::ADC_A_aHL() {
    n32 = a + read((h << 8) | l) + getFlag(fc);
    setFlag(fn, 0);
    setFlag(fh, ((a & 0x0F) + (read((h << 8) | l) & 0x0F) + getFlag(fc)) & 0x10);
    setFlag(fc, n32 & 0x0100);
    setFlag(fz, (a & 0xff) == 0);
    a = n32 & 0xff;
    return 0;
}

uint8_t SM83::ADC_A_A() {
    n32 = a + a + getFlag(fc);
    setFlag(fn, 0);
    setFlag(fh, ((a & 0x0F) + (a & 0x0F) + getFlag(fc)) & 0x10);
    setFlag(fc, n32 & 0x0100);
    setFlag(fz, (a & 0xff) == 0);
    a = n32 & 0xff;
    return 0;
}

uint8_t SM83::SUB_A_B() {
    n16 = a - b;
    setFlag(fn, 1);
    setFlag(fh, (a & 0x0F) < (b & 0x0F));
    setFlag(fc, a < b);
    setFlag(fz, (a & 0xff) == 0);
    a = n16 & 0xff;
    return 0;
}

uint8_t SM83::SUB_A_C() {
    n16 = a - c;
    setFlag(fn, 1);
    setFlag(fh, (a & 0x0F) < (c & 0x0F));
    setFlag(fc, a < c);
    setFlag(fz, (a & 0xff) == 0);
    a = n16 & 0xff;
    return 0;
}

uint8_t SM83::SUB_A_D() {
    n16 = a - d;
    setFlag(fn, 1);
    setFlag(fh, (a & 0x0F) < (d & 0x0F));
    setFlag(fc, a < d);
    setFlag(fz, (a & 0xff) == 0);
    a = n16 & 0xff;
    return 0;
}

uint8_t SM83::SUB_A_E() {
    n16 = a - e;
    setFlag(fn, 1);
    setFlag(fh, (a & 0x0F) < (e & 0x0F));
    setFlag(fc, a < e);
    setFlag(fz, (a & 0xff) == 0);
    a = n16 & 0xff;
    return 0;
}

uint8_t SM83::SUB_A_H() {
    n16 = a - h;
    setFlag(fn, 1);
    setFlag(fh, (a & 0x0F) < (h & 0x0F));
    setFlag(fc, a < h);
    setFlag(fz, (a & 0xff) == 0);
    a = n16 & 0xff;
    return 0;
}

uint8_t SM83::SUB_A_L() {
    n16 = a - l;
    setFlag(fn, 1);
    setFlag(fh, (a & 0x0F) < (l & 0x0F));
    setFlag(fc, a < l);
    setFlag(fz, (a & 0xff) == 0);
    a = n16 & 0xff;
    return 0;
}

uint8_t SM83::SUB_A_aHL() {
    n16 = a - read((h << 8) | l);
    setFlag(fn, 1);
    setFlag(fh, (a & 0x0F) < (read((h << 8) | l) & 0x0F));
    setFlag(fc, a < read((h << 8) | l));
    setFlag(fz, (a & 0xff) == 0);
    a = n16 & 0xff;
    return 0;
}

uint8_t SM83::SUB_A_A() {
    n16 = a - a;
    setFlag(fn, 1);
    setFlag(fh, (a & 0x0F) < (a & 0x0F));
    setFlag(fc, a < a);
    setFlag(fz, (a & 0xff) == 0);
    a = n16 & 0xff;
    return 0;
}

uint8_t SM83::SBC_A_B() {
    n16 = a - b - getFlag(fc);
    setFlag(fn, 1);
    setFlag(fh, ((a & 0x0F) < ((b & 0x0F) + getFlag(fc))));
    setFlag(fc, (a < (b + getFlag(fc))));
    setFlag(fz, (a & 0xff) == 0);
    a = n16 & 0xff;
    return 0;
}

uint8_t SM83::SBC_A_C() {
    n16 = a - c - getFlag(fc);
    setFlag(fn, 1);
    setFlag(fh, ((a & 0x0F) < ((c & 0x0F) + getFlag(fc))));
    setFlag(fc, (a < (c + getFlag(fc))));
    setFlag(fz, (a & 0xff) == 0);
    a = n16 & 0xff;
    return 0;
}

uint8_t SM83::SBC_A_D() {
    n16 = a - d - getFlag(fc);
    setFlag(fn, 1);
    setFlag(fh, ((a & 0x0F) < ((d & 0x0F) + getFlag(fc))));
    setFlag(fc, (a < (d + getFlag(fc))));
    setFlag(fz, (a & 0xff) == 0);
    a = n16 & 0xff;
    return 0;
}

uint8_t SM83::SBC_A_E() {
    n16 = a - e - getFlag(fc);
    setFlag(fn, 1);
    setFlag(fh, ((a & 0x0F) < ((e & 0x0F) + getFlag(fc))));
    setFlag(fc, (a < (e + getFlag(fc))));
    setFlag(fz, (a & 0xff) == 0);
    a = n16 & 0xff;
    return 0;
}

uint8_t SM83::SBC_A_H() {
    n16 = a - h - getFlag(fc);
    setFlag(fn, 1);
    setFlag(fh, ((a & 0x0F) < ((h & 0x0F) + getFlag(fc))));
    setFlag(fc, (a < (h + getFlag(fc))));
    setFlag(fz, (a & 0xff) == 0);
    a = n16 & 0xff;
    return 0;
}

uint8_t SM83::SBC_A_L() {
    n16 = a - l - getFlag(fc);
    setFlag(fn, 1);
    setFlag(fh, ((a & 0x0F) < ((l & 0x0F) + getFlag(fc))));
    setFlag(fc, (a < (l + getFlag(fc))));
    setFlag(fz, (a & 0xff) == 0);
    a = n16 & 0xff;
    return 0;
}

uint8_t SM83::SBC_A_aHL() {
    n16 = a - read((h << 8) | l) - getFlag(fc);
    setFlag(fn, 1);
    setFlag(fh, ((a & 0x0F) < ((read((h << 8) | l) & 0x0F) + getFlag(fc))));
    setFlag(fc, (a < (read((h << 8) | l) + getFlag(fc))));
    setFlag(fz, (a & 0xff) == 0);
    a = n16 & 0xff;
    return 0;
}

uint8_t SM83::SBC_A_A() {
    n16 = a - a - getFlag(fc);
    setFlag(fn, 1);
    setFlag(fh, ((a & 0x0F) < ((a & 0x0F) + getFlag(fc))));
    setFlag(fc, (a < (a + getFlag(fc))));
    setFlag(fz, (a & 0xff) == 0);
    a = n16 & 0xff;
    return 0;
}

uint8_t SM83::AND_A_B() {
    a &= b;
    setFlag(fz, a == 0);
    setFlag(fn, 0);
    setFlag(fh, 1);
    setFlag(fc, 0);
    return 0;
}

uint8_t SM83::AND_A_C() {
    a &= c;
    setFlag(fz, a == 0);
    setFlag(fn, 0);
    setFlag(fh, 1);
    setFlag(fc, 0);
    return 0;
}

uint8_t SM83::AND_A_D() {
    a &= d;
    setFlag(fz, a == 0);
    setFlag(fn, 0);
    setFlag(fh, 1);
    setFlag(fc, 0);
    return 0;
}

uint8_t SM83::AND_A_E() {
    a &= e;
    setFlag(fz, a == 0);
    setFlag(fn, 0);
    setFlag(fh, 1);
    setFlag(fc, 0);
    return 0;
}

uint8_t SM83::AND_A_H() {
    a &= h;
    setFlag(fz, a == 0);
    setFlag(fn, 0);
    setFlag(fh, 1);
    setFlag(fc, 0);
    return 0;
}

uint8_t SM83::AND_A_L() {
    a &= l;
    setFlag(fz, a == 0);
    setFlag(fn, 0);
    setFlag(fh, 1);
    setFlag(fc, 0);
    return 0;
}

uint8_t SM83::AND_A_aHL() {
    a &= read((h << 8) | l);
    setFlag(fz, a == 0);
    setFlag(fn, 0);
    setFlag(fh, 1);
    setFlag(fc, 0);
    return 0;
}

uint8_t SM83::AND_A_A() {
    a &= a;
    setFlag(fz, a == 0);
    setFlag(fn, 0);
    setFlag(fh, 1);
    setFlag(fc, 0);
    return 0;
}

uint8_t SM83::XOR_A_B() {
    a ^= b;
    setFlag(fz, a == 0);
    setFlag(fn, 0);
    setFlag(fh, 0);
    setFlag(fc, 0);
    return 0;
}

uint8_t SM83::XOR_A_C() {
    a ^= c;
    setFlag(fz, a == 0);
    setFlag(fn, 0);
    setFlag(fh, 0);
    setFlag(fc, 0);
    return 0;
}

uint8_t SM83::XOR_A_D() {
    a ^= d;
    setFlag(fz, a == 0);
    setFlag(fn, 0);
    setFlag(fh, 0);
    setFlag(fc, 0);
    return 0;
}

uint8_t SM83::XOR_A_E() {
    a ^= e;
    setFlag(fz, a == 0);
    setFlag(fn, 0);
    setFlag(fh, 0);
    setFlag(fc, 0);
    return 0;
}

uint8_t SM83::XOR_A_H() {
    a ^= h;
    setFlag(fz, a == 0);
    setFlag(fn, 0);
    setFlag(fh, 0);
    setFlag(fc, 0);
    return 0;
}

uint8_t SM83::XOR_A_L() {
    a ^= l;
    setFlag(fz, a == 0);
    setFlag(fn, 0);
    setFlag(fh, 0);
    setFlag(fc, 0);
    return 0;
}

uint8_t SM83::XOR_A_aHL() {
    a ^= read((h << 8) | l);
    setFlag(fz, a == 0);
    setFlag(fn, 0);
    setFlag(fh, 0);
    setFlag(fc, 0);
    return 0;
}

uint8_t SM83::XOR_A_A() {
    a ^= a;
    setFlag(fz, a == 0);
    setFlag(fn, 0);
    setFlag(fh, 0);
    setFlag(fc, 0);
    return 0;
}

uint8_t SM83::OR_A_B() {
    a |= b;
    setFlag(fz, a == 0);
    setFlag(fn, 0);
    setFlag(fh, 0);
    setFlag(fc, 0);
    return 0;
}

uint8_t SM83::OR_A_C() {
    a |= c;
    setFlag(fz, a == 0);
    setFlag(fn, 0);
    setFlag(fh, 0);
    setFlag(fc, 0);
    return 0;
}

uint8_t SM83::OR_A_D() {
    a |= d;
    setFlag(fz, a == 0);
    setFlag(fn, 0);
    setFlag(fh, 0);
    setFlag(fc, 0);
    return 0;
}

uint8_t SM83::OR_A_E() {
    a |= e;
    setFlag(fz, a == 0);
    setFlag(fn, 0);
    setFlag(fh, 0);
    setFlag(fc, 0);
    return 0;
}

uint8_t SM83::OR_A_H() {
    a |= h;
    setFlag(fz, a == 0);
    setFlag(fn, 0);
    setFlag(fh, 0);
    setFlag(fc, 0);
    return 0;
}

uint8_t SM83::OR_A_L() {
    a |= l;
    setFlag(fz, a == 0);
    setFlag(fn, 0);
    setFlag(fh, 0);
    setFlag(fc, 0);
    return 0;
}

uint8_t SM83::OR_A_aHL() {
    a |= read((h << 8) | l);
    setFlag(fz, a == 0);
    setFlag(fn, 0);
    setFlag(fh, 0);
    setFlag(fc, 0);
    return 0;
}

uint8_t SM83::OR_A_A() {
    a |= a;
    setFlag(fz, a == 0);
    setFlag(fn, 0);
    setFlag(fh, 0);
    setFlag(fc, 0);
    return 0;
}

uint8_t SM83::CP_A_B() {
    n16 = a - b;
    setFlag(fn, 1);
    setFlag(fh, ((a & 0x0F) < (b & 0x0F)));
    setFlag(fz, (n16 & 0xff) == 0);
    setFlag(fc, (a < b));
    return 0;
}

uint8_t SM83::CP_A_C() {
    n16 = a - c;
    setFlag(fn, 1);
    setFlag(fh, ((a & 0x0F) < (c & 0x0F)));
    setFlag(fz, (n16 & 0xff) == 0);
    setFlag(fc, (a < c));
    return 0;
}

uint8_t SM83::CP_A_D() {
    n16 = a - d;
    setFlag(fn, 1);
    setFlag(fh, ((a & 0x0F) < (d & 0x0F)));
    setFlag(fz, (n16 & 0xff) == 0);
    setFlag(fc, (a < d));
    return 0;
}

uint8_t SM83::CP_A_E() {
    n16 = a - e;
    setFlag(fn, 1);
    setFlag(fh, ((a & 0x0F) < (e & 0x0F)));
    setFlag(fz, (n16 & 0xff) == 0);
    setFlag(fc, (a < e));
    return 0;
}

uint8_t SM83::CP_A_H() {
    n16 = a - h;
    setFlag(fn, 1);
    setFlag(fh, ((a & 0x0F) < (h & 0x0F)));
    setFlag(fz, (n16 & 0xff) == 0);
    setFlag(fc, (a < h));
    return 0;
}

uint8_t SM83::CP_A_L() {
    n16 = a - l;
    setFlag(fn, 1);
    setFlag(fh, ((a & 0x0F) < (l & 0x0F)));
    setFlag(fz, (n16 & 0xff) == 0);
    setFlag(fc, (a < l));
    return 0;
}

uint8_t SM83::CP_A_aHL() {
    n16 = a - read((h << 8) | l);
    setFlag(fn, 1);
    setFlag(fh, ((a & 0x0F) < (read((h << 8) | l) & 0x0F)));
    setFlag(fz, (n16 & 0xff) == 0);
    setFlag(fc, (a < read((h << 8) | l)));
    return 0;
}

uint8_t SM83::CP_A_A() {
    n16 = a - a;
    setFlag(fn, 1);
    setFlag(fh, ((a & 0x0F) < (a & 0x0F)));
    setFlag(fz, (n16 & 0xff) == 0);
    setFlag(fc, (a < a));
    return 0;
}

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

uint8_t SM83::ADD_A_n8() {
    n8 = read(pc++);
    n32 = a + n8;
    setFlag(fn, 0);
    setFlag(fh, ((a & 0x0F) + (n8 & 0x0F)) & 0x10);
    setFlag(fc, n32 & 0x0100);
    setFlag(fz, (a & 0xff) == 0);
    a = n32 & 0xff;
    return 0;
}

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

uint8_t SM83::ADC_A_n8() {
    n8 = read(pc++);
    n32 = a + n8 + getFlag(fc);
    setFlag(fn, 0);
    setFlag(fh, ((a & 0x0F) + (n8 & 0x0F) + getFlag(fc)) & 0x10);
    setFlag(fc, n32 & 0x0100);
    setFlag(fz, (a & 0xff) == 0);
    a = n32 & 0xff;
    return 0;
}

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

uint8_t SM83::SUB_A_n8() {
    n8 = read(pc++);
    n16 = a - n8;
    setFlag(fn, 1);
    setFlag(fh, (a & 0x0F) < (n8 & 0x0F));
    setFlag(fc, a < n8);
    setFlag(fz, (a & 0xff) == 0);
    a = n16 & 0xff;
    return 0;
}

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

uint8_t SM83::SBC_A_n8() {
    n8 = read(pc++);
    n16 = a - n8 - getFlag(fc);
    setFlag(fn, 1);
    setFlag(fh, ((a & 0x0F) < ((n8 & 0x0F) + getFlag(fc))));
    setFlag(fc, (a < (n8 + getFlag(fc))));
    setFlag(fz, (a & 0xff) == 0);
    a = n16 & 0xff;
    return 0;
}

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

uint8_t SM83::AND_A_n8() {
    n8 = read(pc++);
    a &= n8;
    setFlag(fz, a == 0);
    setFlag(fn, 0);
    setFlag(fh, 1);
    setFlag(fc, 0);
    return 0;
}

uint8_t SM83::RST_20() {
    write(--sp, pc >> 8);
    write(--sp, pc & 0xff);
    pc = 0x20;
    return 0;
}

uint8_t SM83::ADD_SP_e8() {
    e8 = toSigned(read(pc++));
    n32 = sp + e8;
    setFlag(fz, 0);
    setFlag(fn, 0);
    setFlag(fh, ((sp & 0x0F) + (e8 & 0x0F)) & 0x10);
    setFlag(fc, ((sp & 0xFF) + (e8 & 0xFF)) & 0x100);
    sp = n32 & 0xFFFF;
    return 0;
}

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

uint8_t SM83::XOR_A_n8() {
    n8 = read(pc++);
    a ^= n8;
    setFlag(fz, a == 0);
    setFlag(fn, 0);
    setFlag(fh, 0);
    setFlag(fc, 0);
    return 0;
}

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

uint8_t SM83::OR_A_n8() {
    n8 = read(pc++);
    a |= n8;
    setFlag(fz, a == 0);
    setFlag(fn, 0);
    setFlag(fh, 0);
    setFlag(fc, 0);
    return 0;
}

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

uint8_t SM83::CP_A_n8() {
    n8 = read(pc++);
    n16 = a - n8;
    setFlag(fn, 1);
    setFlag(fh, ((a & 0x0F) < (n8 & 0x0F)));
    setFlag(fz, (n16 & 0xff) == 0);
    setFlag(fc, (a < n8));
    return 0;
}

uint8_t SM83::RST_38() {
    write(--sp, pc >> 8);
    write(--sp, pc & 0xff);
    pc = 0x38;
    return 0;
}

