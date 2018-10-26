dict = {
    "lb": 1,
    "lh": 1,
    "lw": 1,
    "lbu": 1,
    "lhu": 1,
    "jalr": 1,
    "addi": 1,
    "slti": 1,
    "xori": 1,
    "ori": 1,
    "andi": 1,
    "slli": 1,
    "srli": 1,
    "srai": 1,
    "lui": 2,
    "auipc": 2,
    "add": 3,
    "sub": 3,
    "sll": 3,
    "slt": 3,
    "sltu": 3,
    "xor": 3,
    "srl": 3,
    "sra": 3,
    "or": 3,
    "and": 3,
    "jal": 4,
    "beq": 5,
    "bne": 5,
    "blt": 5,
    "bge": 5,
    "bltu": 5,
    "bgeu": 5,
    "sb": 6,
    "sh": 6,
    "sw": 6,
}

dict_type = {
    1:  "I",
    2:  "U",
    3:  "R",
    4:  "J",
    5:  "B",
    6:  "S"
}

dict_op = {
    "lb": "0000011",
    "lh": "0000011",
    "lw": "0000011",
    "lbu": "0000011",
    "lhu": "0000011",
    "jalr": "1100111",
    "addi": "0010011",
    "slti": "0010011",
    "xori": "0010011",
    "ori": "0010011",
    "andi": "0010011",
    "slli": "0010011",
    "srli": "0010011",
    "srai": "0010011"
}

dict_funct3 = {
    "lb": "000",
    "lh": "001",
    "lw": "010",
    "lbu": "100",
    "lhu": "101",
    "jalr": "000",
    "addi": "000",
    "slti": "010",
    "xori": "100",
    "ori": "110",
    "andi": "111",
    "slli": "001",
    "srli": "101",
    "srai": "101"
}

dict_funct7 = {
    "slli": "0000000",
    "srli": "0000000",
    "srai": "0100000"
}



def Itype(x):
    funct7 = dict_funct7.get(x[0], -1)
    opcode = dict_op[x[0]]
    funct3 = dict_funct3[x[0]]
    rd = bin(int(x[1]))
    newrd = rd[2:len(rd)]
    while(len(newrd) != 5):
        newrd = '0' + newrd
    rd = newrd
    rs1 = bin(int(x[2]))
    newrs1 = rs1[2:len(rs1)]
    while(len(newrs1) != 5):
        newrs1 = '0' + newrs1
    rs1 = newrs1
    if(funct7 == -1):
        imm = bin(int(x[3]) & 0b111111111111)
        newimm = imm[2:len(imm)]
        while(len(newimm) != 12):
            newimm = '0' + newimm
        imm = newimm
        instruction = imm + rs1 + funct3 + rd + opcode
        instruction = '{:0{}X}'.format(int(instruction, 2), len(instruction) // 4)
        return instruction
    else:
        imm = bin(int(x[3]) & 0b11111)
        newimm = imm[2:len(imm)]
        while(len(newimm) != 5):
            newimm = '0' + newimm
        imm = newimm
        print(funct7)
        instruction = funct7 + imm + rs1 + funct3 + rd + opcode
        instruction = '{:0{}X}'.format(int(instruction, 2), len(instruction) // 4)
        return instruction




def main():
    with open('instructions') as f:
        lines = f.readlines()
        f2 = open("simple2", "w")
        instruction = []
        for i in range(0, len(lines)):
            x = lines[i].split(" ")
            for char in x[-1]:
                x[-1] = x[-1].strip('\n')
            opcode = dict[x[0]]
            if(opcode == 1):
                instruction = Itype(x)
            f2.write("0x")
            f2.write(instruction)
            f2.write("\n")


main()
