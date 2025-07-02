# to create the function name definitions from the op codes json
import json

with open('opcodes.json') as f:
    opcodes_json = f.read()

opcodes_dict = json.loads(opcodes_json)
output = ""
count = 0
function_names = []
unpre = opcodes_dict['unprefixed']
pre = opcodes_dict['cbprefixed']
operands = set()

ifile = open("SM83.instr.cpp", "w")

ifile.write("#include <SM83rewrite.h>\n#include<iostream>\n\n")


def printitems(list_of_to_print, perline):
    c = perline
    for to_print in list_of_to_print:
        if c <= 1: 
            print(to_print)
            c = perline
        else:
            print(to_print, end=' ')
            c -= 1

def insertbody(functiondefinition: str, bodytoinsert: str):
    return functiondefinition.replace("{ return 0; }", bodytoinsert)

def makedefinition(functionname: str, body: str):
    return "uint8_t SM83::" + functionname + "() " + body


# this is my least favourite part of working on this project


for i in unpre:

    # for operand in unpre[i]['operands']:
    #     operands.add(operand['name'])
#    if unpre['mnemonic'] == 'LD':
#        count += 1
#        print(len(unpre['operands']), i)

    # generating function name
    function_name = unpre[i]['mnemonic']
    for operand in unpre[i]['operands']:

        name = operand['name']
        if not operand['immediate']:
            name = 'a' + name
        if 'increment' in operand:
            name = name + "I"
        if 'decrement' in operand:
            name = name + 'D'
        function_name = function_name + "_" + name
        function_name = function_name.replace("$", "")
        
    # instruction_init_list = '{\"' + unpre[i]['mnemonic'] + '\", &' + function_name + ", " + str(unpre[i]['cycles'][0]) + '},'

    # function_definition: str = "uint8_t SM83::" + function_name + "() {}"
    # function_definition = function_definition.format('{ return 0; }')

    # now generating function body
    function_definition = makedefinition(function_name, '{ return 0; }\n\n')

    # 16 bit Loads
    if function_name.startswith("LD_"):
        args = unpre[i]['operands']
        target = args[0]
        source = args[1]
        t_str = str(target['name']).lower()
        s_str = str(source['name']).lower()
        if len(s_str) == 2 and s_str != 'n8': 
            s_str = f"({s_str[0]} << 8) | {s_str[1]}"
        if not source['immediate']: 
            s_str = f"read({s_str})"
        if not target['immediate']:
            cmd = f"\n    write(({t_str[0]} << 8) | {t_str[1]}, {s_str});"
        else:
            if (len(t_str) == 2):
                cmd = f"\n    {t_str[0]} = {s_str} >> 8;"
                cmd = cmd + f"\n    {t_str[0]} = ({s_str}) & 0xff;"
            else:
                cmd = f"\n    {t_str} = {s_str};"
        
        end = ""
        if 'increment' in target:
            end = f"\n    {target['name'].lower()}++;"
        if 'decrement' in target:
            end = f"\n    {target['name'].lower()}--;"
        if 'increment' in source:
            end = f"\n    {source['name'].lower()}++;"
        if 'decrement' in source:
            end = f"\n    {source['name'].lower()}--;"

        if end != "":
            d = end.split()
            if len(d[0]) == 2+3:
                if (d[-1][-3:] == "++;"):
                    t = "1"
                else:
                    t = "-1"
                end = f"\n    addToHiLo({d[0][0]}, {d[0][1]}, {t});"

            cmd = cmd + end
        
        if 'bytes' in source:
            if source['bytes'] == 1:
                cmd = f"\n    {source['name']} = read(pc++);" + cmd
            if source['bytes'] == 2:
                cmd = f"\n    {source['name']} |= read(pc++) << 8;" + cmd
                cmd = f"\n    {source['name']} = read(pc++);" + cmd

        function_definition = makedefinition(function_name, '{'+cmd+'\n    return 0;\n}\n\n') 

    if function_name == "LD_aa16_SP":
        function_definition = """uint8_t SM83::LD_aa16_SP() {
    a16 = read(pc++);
    a16 |= read(pc++) << 8;
    write(a16, sp & 0xFF);
    write(a16 + 1, (sp >> 8) & 0xFF);
    return 0;
}


"""
    if function_name == "LD_SP_n16":
        function_definition = '''uint8_t SM83::LD_SP_n16() {
    n16 = read(pc++);
    n16 |= read(pc++) << 8;
    sp = n16;
    return 0;
}

'''

    if function_name == "LD_HL_SPI_e8":
        function_definition = '''uint8_t SM83::LD_HL_SPI_e8() {

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

'''

    if function_name == "LD_SP_HL":
        function_definition = '''uint8_t SM83::LD_SP_HL() {
    sp = l;
    sp |= h << 8; 
    return 0;
}

'''

    # POP and PUSH
    if function_name.startswith("PUSH"):
        arg = unpre[i]['operands'][0]['name'].lower()

        cmd = f"\n    write(--sp, {arg[0]});"
        if (arg[1] == 'f'):
            cmd = cmd + f"\n    write(--sp, {arg[1]} & 0xf0);"
        else:
            cmd = cmd + f"\n    write(--sp, {arg[1]});"

        cmd = cmd + "\n    return 0;"
        function_definition = makedefinition(function_name, '{'+cmd+'\n    return 0;\n}\n\n') 
    if function_name.startswith("POP"):
        arg = unpre[i]['operands'][0]['name'].lower()

        cmd = f"\n    {arg[1]} = read(sp++);"
        cmd = f"\n    {arg[0]} = read(sp++);"
        cmd = cmd + "\n    return 0;"
        function_definition = makedefinition(function_name, '{'+cmd+'\n    return 0;\n}\n\n') 

    # LDH
    if function_name.startswith("LDH_"):
        args = unpre[i]['operands']
        target = args[0]
        source = args[1]
        t_str = str(target['name']).lower()
        s_str = str(source['name']).lower()
        if not source['immediate']: 
            s_str = f"read(0xff00 | {s_str})"
        if not target['immediate']:
            cmd = f"\n    write(0xff00 | {t_str}, {s_str});"
        else:
            cmd = f"\n    {t_str} = {s_str};"
        
        if 'bytes' in source:
            if source['bytes'] == 1:
                cmd = f"\n    {source['name']} = read(pc++);" + cmd

        function_definition = makedefinition(function_name, '{'+cmd+'\n    return 0;\n}\n\n') 

    # JP
    if function_name.startswith("JP_"):
        args = unpre[i]['operands']
        if len(args) == 2:
            fargs = args.copy()
            fargs[0] = args[0]['name'].lower()
            fargs[1] = args[1]['name'].lower()

            condition = f"getFlag(f{fargs[0][-1]})"
            if len(fargs[0]) == 2:
                condition = "!" + condition
            
            cmd = """
    if (not condition) {
        pc += 2;
        return 0;
    }
    a16 = read(pc++);
    a16 |= read(pc++) << 8;
    pc = a16;
    return 1;""".replace("condition", condition)
                            
            function_definition = makedefinition(function_name, '{'+cmd+'\n    return 0;\n}\n\n') 
        elif len(args) == 1:
            cmd = """
    pc = l;
    pc |= h << 8;
    return 0;"""
            if args[0]['name'].lower() == "a16":
                cmd = """
    a16 = read(pc++);
    a16 |= read(pc++) << 8;
    pc = a16;
    return 0;"""


            function_definition = makedefinition(function_name, '{'+cmd+'\n    return 0;\n}\n\n') 


    # JR
    if function_name.startswith("JR_"):
        args = unpre[i]['operands']
        if len(args) == 2:
            fargs = args.copy()
            fargs[0] = args[0]['name'].lower()
            fargs[1] = args[1]['name'].lower()

            condition = f"getFlag(f{fargs[0][-1]})"
            if len(fargs[0]) == 2:
                condition = "!" + condition
            
            cmd = """
    if (not condition) {
        pc += 1;
        return 0;
    }
    e8 = toSigned(read(pc++));
    pc += e8;
    return 1;""".replace("condition", condition)
                            
            function_definition = makedefinition(function_name, '{'+cmd+'\n}\n\n') 
        elif len(args) == 1:
            cmd = """
    e8 = toSigned(read(pc++));
    pc += e8;
    return 0;"""
            function_definition = makedefinition(function_name, '{'+cmd+'\n}\n\n') 

    # CALL
    if function_name.startswith("CALL_"):
        args = unpre[i]['operands']
        if len(args) == 2:
            fargs = args.copy()
            fargs[0] = args[0]['name'].lower()
            fargs[1] = args[1]['name'].lower()

            condition = f"getFlag(f{fargs[0][-1]})"
            if len(fargs[0]) == 2:
                condition = "!" + condition
            
            cmd = """
    if (not condition) {
        pc += 2;
        return 0;
    }
    write(--sp, pc >> 8);
    write(--sp, pc & 0xff);
    a16 = read(pc++);
    a16 |= read(pc++) << 8;
    pc = a16;
    return 3;""".replace("condition", condition)
                            
            function_definition = makedefinition(function_name, '{'+cmd+'\n}\n\n') 
        elif len(args) == 1:
            cmd = """
    a16 = read(pc++);
    a16 |= read(pc++) << 8;
    pc = a16;
    return 0;"""
            function_definition = makedefinition(function_name, '{'+cmd+'\n}\n\n') 


    # RET
    if function_name.startswith("RET_"):
        args = unpre[i]['operands']
        if len(args) == 1:
            fargs = args.copy()
            fargs[0] = args[0]['name'].lower()
            condition = f"getFlag(f{fargs[0][-1]})"
            if len(fargs[0]) == 2:
                condition = "!" + condition
            
            cmd = """
    if (not condition) {
        return 0;
    }
    pc = read(sp++);
    pc |= read(sp++) << 8;
    return 3;""".replace("condition", condition)
                            
            function_definition = makedefinition(function_name, '{'+cmd+'\n}\n\n') 
        elif len(args) == 0:
            cmd = """
    pc = read(sp++);
    pc |= read(sp++) << 8;
    return 0;"""
            function_definition = makedefinition(function_name, '{'+cmd+'\n}\n\n') 

    # RETI
    if function_name.startswith("RETI"):
        cmd = """
    ime = 2;
    pc = read(sp++);
    pc |= read(sp++) << 8;
    return 0;
"""
        function_definition = makedefinition(function_name, '{'+cmd+'\n}\n\n') 
    
    
    # RST
    if function_name.startswith("RST_"):
        args = unpre[i]['operands']
        fargs = args.copy()
        fargs[0] = args[0]['name'].upper().replace("$", "0x")
        cmd = """
    write(--sp, pc >> 8);
    write(--sp, pc & 0xff);
    pc = address_lo;
    return 0;
""".replace("address_lo", fargs[0])

        function_definition = makedefinition(function_name, '{'+cmd+'\n}\n\n') 

    # STOP
    if function_name.startswith("STOP_"):

        function_definition = makedefinition(function_name, '{ pc++; return 0; }\n\n')

    # HALT
    if function_name.startswith("HALT"):
        # args = unpre[i]['operands']
        # fargs = args.copy()
        # fargs[0] = args[0]['name'].upper().replace("$", "0x")

        function_definition = makedefinition(function_name, '{ std::cout << "SYSTEM HALT REQUESTED!"; return 0; }\n\n')
    
    # EI AND DI
    if function_name.startswith("EI"):
        function_definition = makedefinition(function_name, '{ ime = 2; return 0; }\n\n')
    if function_name.startswith("DI"):
        function_definition = makedefinition(function_name, '{ ime = 0; return 0; }\n\n')

    #################################################################################################################
    ifile.write(function_definition) 
    #region PROGRESS ################################################################################################

    # ALU STUFF
    # INC
    if (function_name.startswith("INC_") or function_name.startswith("DEC_")):
        fargs = unpre[i]['operands'].copy()
        fargs[0] = '{' + fargs[0]['name'].upper() + ', ' + str(fargs[0]['immediate']).lower() + '}'
        if i[-1] == '3' or i[-1] == 'B': # 16 bit columns
            function_definition = makedefinition(function_name, '{ return PROCESS_ALU16(' + fargs[0] + ', ' + function_name[:3] + '); }')
        else: # 8 bit
            function_definition = makedefinition(function_name, '{ return PROCESS_ALU(' + fargs[0] + ', ' + function_name[:3] + '); }')


    # all INC and DEC
    if (function_name.startswith("INC_") or function_name.startswith("DEC_")):
        fargs = unpre[i]['operands'].copy()
        fargs[0] = '{' + fargs[0]['name'].upper() + ', ' + str(fargs[0]['immediate']).lower() + '}'
        if i[-1] == '3' or i[-1] == 'B': # 16 bit columns
            function_definition = makedefinition(function_name, '{ return PROCESS_ALU16(' + fargs[0] + ', ' + function_name[:3] + '); }')
        else: # 8 bit
            function_definition = makedefinition(function_name, '{ return PROCESS_ALU(' + fargs[0] + ', ' + function_name[:3] + '); }')

    # Remaining operand relying operations of ALU
    if function_name[:3] in ("ADD", "ADC", "SUB", "SBC", "AND", "OR_", "XOR", "CP_", ):
        args = unpre[i]['operands']
        fargs = args.copy()
        fargs[0] = '{' + args[0]['name'].upper() + ', ' + str(args[0]['immediate']).lower() + '}'
        fargs[1] = '{' + args[1]['name'].upper() + ', ' + str(args[1]['immediate']).lower() + '}'

        # 16 bit ADD
        if i[-1] in ('8', '9') and function_name.startswith("ADD"):
            function_definition = makedefinition(function_name, '{ return PROCESS_ALU16(' + fargs[0] + ', ' + fargs[1] + ', ADD); }' )

        else:
            function_definition = makedefinition(function_name, '{ return PROCESS_ALU(' + fargs[0] + ', ' + fargs[1] + ', ' + function_name[:3].replace("_", "") + '); }' )
            
    # the four quirky ones
    if function_name in ("DAA", "SCF", "CPL", "CCF"):
        function_definition = makedefinition(function_name, '{ return PROCESS_ALU(enum' + function_name + '); }')

    # a rotates
    if unpre[i]["mnemonic"] in ("RLCA", "RLA", "RRCA", "RRA"):
        mnemonic = unpre[i]["mnemonic"]
        args = unpre[i]['operands'].copy()
        args = ''
        if mnemonic[1] == 'R':
            args = args + "true"
        else:
            args = args + "false"
        if mnemonic[2] == 'C':
            args = args + ", false"
        else:
            args = args + ", true"

        function_definition = makedefinition(function_name, '{ return ROTATEREGALEFT(' + args + '); }')


    count += 1
    if count == 1:
        # print("uint8_t " + function_name + "();")
        # print(instruction_init_list)
        # output = output + instruction_init_list + "\n"
        print(function_definition)
        output = output + function_definition + "\n"
        count = 0
    else:
        pass
        # print("uint8_t " + function_name + "();", end=' ')
        # print(instruction_init_list, end=' ')
        # output = output + instruction_init_list + ' '

# print ("FINAL COUNT", count)

#print(opcodes_dict['unprefixed']['0x'+'{:X}'.format(0).rjust(2, '0')])

# [print(operand) for operand in operands if not operand.startswith("$")]

with open("codewriteroutput/unprefixeddefinitions.txt", 'w') as f:
    f.write(output)



output = ""
print('\n\n')
for i in pre:
    
    # generating function name
    function_name = pre[i]['mnemonic']
    for operand in pre[i]['operands']:

        name = operand['name']
        if not operand['immediate']:
            name = 'a' + name
        if 'increment' in operand:
            name = name + "I"
        if 'decrement' in operand:
            name = name + 'D'
        function_name = function_name + "_" + name
        function_name = function_name.replace("$", "")

    # now generating function body
    function_definition = makedefinition(function_name, '{ return 0; }')

    instruction_init_list = '{\"' + pre[i]['mnemonic'] + '\", &x::' + function_name + ", " + str(pre[i]['cycles'][0]) + '},'

    # function_definition: str = "uint8_t SM83::" + function_name + "() {}"
    # function_definition = function_definition.format('{ return 0; }')



    # ROTATES
    if i[-2] in ("0", "1"):
        mnemonic = pre[i]["mnemonic"]
        args = pre[i]['operands'].copy()
        args = '{' + args[0]['name'].upper() + ', ' + str(args[0]['immediate']).lower() + '}'
        if mnemonic[1] == 'R':
            args = args + ", true"
        else:
            args = args + ", false"
        if len(mnemonic) == 3:
            args = args + ", false"
        else:
            args = args + ", true"

        function_definition = makedefinition(function_name, '{ return ROTATELEFT(' + args + '); }')

    # SHIFTS and SWAPS
    elif i[-2] in ("2", "3"):
        mnemonic = pre[i]["mnemonic"]
        args = pre[i]['operands'].copy()
        args = '{' + args[0]['name'].upper() + ', ' + str(args[0]['immediate']).lower() + '}'

        # SHIFTS
        if len(mnemonic) == 3:
            if mnemonic[1] == 'R':
                args = args + ", true"
            else:
                args = args + ", false"
            if mnemonic[2] == 'L':
                args = args + ", true"
            else:
                args = args + ", false"
            function_definition = makedefinition(function_name, '{ return SHIFTLEFT(' + args + '); }')

        # SWAPS
        if len(mnemonic) == 4:
            function_definition = makedefinition(function_name, '{ return SWAP(' + args + '); }')

    # the rest of them BITS, SETS, and RESETS
    else:
        mnemonic = pre[i]['mnemonic']
        args = pre[i]['operands'].copy()
        args = args[0]['name'] + ', {' + args[1]['name'].upper() + ', ' + str(args[1]['immediate']).lower() + '}'
        function_definition = makedefinition(function_name, '{ return ' + mnemonic + '(' + args + '); }')


    count += 1
    if count == 1:
        # print("uint8_t " + function_name + "();")
        # print(instruction_init_list)
        # output = output + instruction_init_list + "\n"
        print(function_definition)
        output = output + function_definition + "\n"
        count = 0
    else:
        pass
        print(function_definition)
        output = output + function_definition + "\n"
        # print("uint8_t " + function_name + "();", end=' ')
        # print(instruction_init_list, end=' ')
        # output = output + instruction_init_list + ' '
    
    
    with open("codewriteroutput/cbprefixeddefinitions.txt", 'w') as f:
        f.write(output)



# test rom
from random import randrange

rom = [] # list of chr
for addr in range(0, 0x8000):
    rom.append(0x00)

addr = 0x0150
for i in unpre:
    code = int(i[2:], base=16)
    if code == 0xcb:
        continue
    rom[addr] = code
    addr += 1
    if unpre[i]['bytes'] == 2:
        rom[addr] = randrange(0x00, 0xd0)
        addr += 1
    if unpre[i]['bytes'] == 3:
        if unpre[i]['mnemonic'] in ('CALL', 'JP', 'JR'):
            rom[addr] = randrange(0x00, 0x23)
            addr += 1
            rom[addr] = randrange(0x00, 0x80)
            addr += 1
        else :
            rom[addr] = randrange(0x00, 0xff + 1)
            addr += 1
            rom[addr] = randrange(0xc0, 0xe0)
            addr += 1

# JP $150
rom[0x0101] = 0xc3 
rom[0x0102] = 0x50
rom[0x0103] = 0x01

with open("ROMS/mytestrom.gb", "wb") as f:
    bytecode = bytes(rom)
    f.write(bytecode)

print("bytes written")

# instructions names
perline = 15
c = perline
for i in unpre:
    l = [unpre[i]['mnemonic']]
    for operand in unpre[i]['operands']:
        l.append(operand['name'])
    
    if c == 0: 
        print('"{}",'.format(" ".join(l)))
        c = perline
    else:
        print('"{}",'.format(" ".join(l)), end=' ')
        c -= 1

# all valid addresses of instructions in a rom
romname = "ROMS/Tetris (Japan) (En).gb"
with open(romname, "rb") as f:
    rom: bytes = f.read()

bytecode = rom.hex(":")
addr = 0x150
bytelist = bytecode.split(":")
valid_address: list = []

while addr < 0x8000:
    opcode = '0x'+bytelist[addr].upper()
    instructsize = unpre[opcode]['bytes']
    if opcode == "0xCB":
        instructsize = 2
    valid_address.append(hex(addr).upper())
    # this is for disassembly
    # to_print = hex(addr).upper().rjust(4, '0') + ": " + unpre['0x'+bytelist[addr].upper()]['mnemonic'].upper().rjust(4, ' ') + " "
    # list of all addresses
    
    addr += instructsize

with open("ROMS/validaddr.out", "w") as f:
    
    perline = 1
    c = perline
    for i in valid_address:
        i = int(i[2:], base=16)
        if c <= 1:
            f.write(str(i) + "\n")
            c = perline
        else:
            f.write(str(i) + " ")
            c -= 1

romname = "ROMS/dmg_boot.bin"
with open(romname, "rb") as f:
    rom: bytes = f.read()

bytecode = rom.hex(":")
addr = 0x00
bytelist = bytecode.split(":")
asm_lines: list = []

while addr < 0xff:
    opcode = '0x'+bytelist[addr].upper()
    instructsize = unpre[opcode]['bytes']
    ins_memo = unpre[opcode]['mnemonic']
    

    if opcode == "0xCB":
        preopcode = '0x' + bytelist[addr+1].upper()
        ins_memo = ins_memo + ' ' + pre[preopcode]['mnemonic']
        for operand in pre[preopcode]['operands']:
            if operand['immediate']:
                ins_memo = ins_memo + " " + operand['name']
            else:
                ins_memo = ins_memo + " a" + operand['name']
        instructsize = 2
    else:
        for operand in unpre[opcode]['operands']:
            if operand['immediate']:
                ins_memo = ins_memo + " " + operand['name']
            else:
                ins_memo = ins_memo + " a" + operand['name']
        if instructsize == 3:
            ins_memo = ins_memo + ' ' + bytelist[addr+2].upper() + bytelist[addr+1].upper()
        if instructsize == 2:
            ins_memo = ins_memo + ' ' + bytelist[addr+1].upper()
    asm_lines.append(hex(addr).upper() + ': ' + ins_memo)
    # this is for disassembly
    # to_print = hex(addr).upper().rjust(4, '0') + ": " + unpre['0x'+bytelist[addr].upper()]['mnemonic'].upper().rjust(4, ' ') + " "
    # list of all addresses
    
    addr += instructsize

with open("ROMS/dmg_boot.txt", "w") as f:

    perline = 1
    c = perline
    for i in asm_lines:
        if c <= 1:
            f.write(i + "\n")
            c = perline
        else:
            f.write(i + " ")
            c -= 1


ifile.close()