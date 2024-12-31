# to create the function name definitions from the op codes json
import json

with open('opcodes.json') as f:
    opcodes_json = f.read()

opcodes_dict = json.loads(opcodes_json)
output = ""
count = 0
function_names = []
unpre = opcodes_dict['unprefixed']
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

with open("hackoutputs.txt", 'w') as f:
    f.write(output)