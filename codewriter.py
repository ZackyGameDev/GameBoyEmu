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
for i in unpre:



    # for operand in unpre[i]['operands']:
    #     operands.add(operand['name'])
#    if unpre['mnemonic'] == 'LD':
#        count += 1
#        print(len(unpre['operands']), i)
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

    function_definition: str = "uint8_t SM83::" + function_name + "() {}"

    if function_name.startswith("LD_") and ((i[-2] in [str(p) for p in range(4, 8)]) or (i[-2] in [str(p) for p in range(0, 5)] and i[-1] not in ('1', '8')) or (i[-1] == 'A' and (i[2] in ('E', 'F')))):
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

        function_definition = function_definition.format('{ return LD(' + fargs[0] + ', ' + fargs[1] + '); }')
    else:
        function_definition = function_definition.format('{ return 0; }')


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