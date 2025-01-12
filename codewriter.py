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
    function_definition = makedefinition(function_name, '{ return 0; }')

    # 16 bit Loads
    if function_name.startswith("LD_"):
        args = unpre[i]['operands']
        fargs = args.copy()
        fargs[0] = '{' + args[0]['name'].upper() + ', ' + str(args[0]['immediate']).lower() + '}'
        fargs[1] = '{' + args[1]['name'].upper() + ', ' + str(args[1]['immediate']).lower() + '}'

        if 'increment' in args[0]:
            fargs[0] = fargs[0][:-1] + ', .increment=true}'
        if 'decrement' in args[0]:
            fargs[0] = fargs[0][:-1] + ', .decrement=true}'
        if 'increment' in args[1]:
            fargs[1] = fargs[1][:-1] + ', .increment=true}'
        if 'decrement' in args[1]:
            fargs[1] = fargs[1][:-1] + ', .decrement=true}'

        function_definition = makedefinition(function_name, '{ return LD16(' + fargs[0] + ', ' + fargs[1] + '); }') 

    # POP and PUSH
    if function_name.startswith("POP") or function_name.startswith("PUSH"):
        args = unpre[i]['operands']
        fargs = args.copy()
        fargs[0] = '{' + args[0]['name'].upper() + ', ' + str(args[0]['immediate']).lower() + '}'

        function_definition = makedefinition(function_name, '{ return ' + function_name[:-3] + '(' + fargs[0] + '); }')

    # 8 bit lads
    if function_name.startswith("LD_") and ((i[-2] in [str(p) for p in range(4, 8)]) or (i[-2] in [str(p) for p in range(0, 5)] and i[-1] not in ('1', '8')) or (i[-1] == 'A' and (i[-2] in ('E', 'F')))):
        args = unpre[i]['operands']
        fargs = args.copy()
        fargs[0] = '{' + args[0]['name'].upper() + ', ' + str(args[0]['immediate']).lower() + '}'
        fargs[1] = '{' + args[1]['name'].upper() + ', ' + str(args[1]['immediate']).lower() + '}'

        if 'increment' in args[0]:
            fargs[0] = fargs[0][:-1] + ', .increment=true}'
        if 'decrement' in args[0]:
            fargs[0] = fargs[0][:-1] + ', .decrement=true}'
        if 'increment' in args[1]:
            fargs[1] = fargs[1][:-1] + ', .increment=true}'
        if 'decrement' in args[1]:
            fargs[1] = fargs[1][:-1] + ', .decrement=true}'

        # 8 bit loads
        function_definition = makedefinition(function_name, '{ return LD(' + fargs[0] + ', ' + fargs[1] + '); }')

    # LD HL, SP + e8
    if function_name.startswith("LD_HL_SPI_e8"):
        function_definition = makedefinition(function_name, '{ return LD_HL_SPDD(); }')

    # LDH
    if function_name.startswith("LDH_"):
        args = unpre[i]['operands']
        fargs = args.copy()
        fargs[0] = '{' + args[0]['name'].upper() + ', ' + str(args[0]['immediate']).lower() + '}'
        fargs[1] = '{' + args[1]['name'].upper() + ', ' + str(args[1]['immediate']).lower() + '}'

        # LDH
        function_definition =  makedefinition(function_name, '{ return LDH(' + fargs[0] + ', ' + fargs[1] + '); }')


    # JP
    if function_name.startswith("JP_"):
        args = unpre[i]['operands']
        if len(args) == 2:
            fargs = args.copy()
            fargs[0] = args[0]['name'].upper()
            fargs[1] = '{' + args[1]['name'].upper() + ', ' + str(args[1]['immediate']).lower() + '}'

            function_definition = makedefinition(function_name, '{ return JUMPTO(' + fargs[0] + ', ' + fargs[1] + '); }')
        elif len(args) == 1:
            fargs = args.copy()
            fargs[0] = '{' + args[0]['name'].upper() + ', ' + str(args[0]['immediate']).lower() + '}'

            function_definition = makedefinition(function_name, '{ return JUMPTO(' + fargs[0] + '); }')

    # JR
    if function_name.startswith("JR_"):
        args = unpre[i]['operands']
        if len(args) == 2:
            fargs = args.copy()
            fargs[0] = args[0]['name'].upper()
            fargs[1] = '{' + args[1]['name'].upper() + ', ' + str(args[1]['immediate']).lower() + '}'

            function_definition = makedefinition(function_name, '{ return JUMPTO(' + fargs[0] + ', ' + fargs[1] + '); }')
        elif len(args) == 1:
            fargs = args.copy()
            fargs[0] = '{' + args[0]['name'].upper() + ', ' + str(args[0]['immediate']).lower() + '}'

            function_definition = makedefinition(function_name, '{ return JUMPTO(' + fargs[0] + '); }')

    # CALL
    if function_name.startswith("CALL_"):
        args = unpre[i]['operands']
        if len(args) == 2:
            fargs = args.copy()
            fargs[0] = args[0]['name'].upper()
            fargs[1] = '{' + args[1]['name'].upper() + ', ' + str(args[1]['immediate']).lower() + '}'

            function_definition = makedefinition(function_name, '{ return CALL(' + fargs[0] + ', ' + fargs[1] + '); }')
        elif len(args) == 1:
            fargs = args.copy()
            fargs[0] = '{' + args[0]['name'].upper() + ', ' + str(args[0]['immediate']).lower() + '}'

            function_definition = makedefinition(function_name, '{ return CALL(' + fargs[0] + '); }') 

    # RET
    if function_name.startswith("RET_"):
        args = unpre[i]['operands']
        if len(args) == 1:
            fargs = args.copy()
            fargs[0] = args[0]['name'].upper()

            function_definition = makedefinition(function_name, '{ return RETURNFROMFUNCTION(' + fargs[0] + '); }')
        elif len(args) == 0:
            function_definition = makedefinition(function_name, '{ return RETURNFROMFUNCTION(); }')

    # RETI
    if function_name.startswith("RETI"):
        function_definition = makedefinition(function_name, '{ return RETURNANDEI(); }')
    
    # RST
    if function_definition.startswith("RST_"):
        args = unpre[i]['operands']
        fargs = args.copy()
        fargs[0] = args[0]['name'].upper().replace("$", "0x")

        function_definition = makedefinition(function_name, '{ return RST(' + fargs[0] + '); }')
    
    # EI AND DI
    if function_name.startswith("EI"):
        function_definition = makedefinition(function_name, '{ return ENABLEINTERRUPTS(); }')
    if function_name.startswith("DI"):
        function_definition = makedefinition(function_name, '{ return DISABLEINTERRUPTS(); }')

    # ALU STUFF
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
            args = args + ", true"
        else:
            args = args + ", false"

        function_definition = makedefinition(function_name, '{ return ROTATELEFT(' + args + '); }')

    # SHIFTS and SWAPS
    elif i[-2] in ("2", "3"):
        mnemonic = pre[i]["mnemonic"]
        args = pre[i]['operands'].copy()
        args = '{' + args[0]['name'].upper() + ', ' + str(args[0]['immediate']).lower() + '}'

        # SHIFTS
        if len(mnemonic) == 2:
            if mnemonic[1] == 'R':
                args = args + ", .reversed=true"
            if mnemonic[2] == 'L':
                args = args + ", .is_logical=true"
            function_definition = makedefinition(function_name, '{ return SHIFTLEFT(' + args + '); }')

        # SWAPS
        if len(mnemonic) == 3:
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