import sys

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
    "mul": 3,
    "div": 3,
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
    "srai": "0010011",
    "lui": "0110111",
    "auipc": "0010111",
    "mul": "0110011",
    "div": "0110011",
    "add": "0110011",
    "sub": "0110011",
    "sll": "0110011",
    "slt": "0110011",
    "sltu": "0110011",
    "xor": "0110011",
    "srl": "0110011",
    "sra": "0110011",
    "or": "0110011",
    "and": "0110011",
    "jal": "1101111",
    "beq": "1100011",
    "bne": "1100011",
    "blt": "1100011",
    "bge": "1100011",
    "bltu": "1100011",
    "bgeu": "1100011",
    "sb": "0100011",
    "sh": "0100011",
    "sw": "0100011"

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
    "srai": "101",
    "mul": "000",
    "div": "100",
    "add": "000",
    "sub": "000",
    "sll": "001",
    "slt": "010",
    "sltu": "011",
    "xor": "100",
    "srl": "101",
    "sra": "101",
    "or": "110",
    "and": "111",
    "beq": "000",
    "bne": "001",
    "blt": "100",
    "bge": "101",
    "bltu": "110",
    "bgeu": "111",
    "sb": "000",
    "sh": "001",
    "sw": "010"

}

dict_funct7 = {
    "slli": "0000000",
    "srli": "0000000",
    "srai": "0100000",
    "add": "0000000",
    "sub": "0100000",
    "sll": "0000000",
    "slt": "0000000",
    "sltu": "0000000",
    "xor": "0000000",
    "srl": "0000000",
    "sra": "0100000",
    "or": "0000000",
    "and": "0000000",
    "mul": "0000001",
    "div": "0000001",
    "add": "0000000",
    "sub": "0100000",
    "sll": "0000000",
    "slt": "0000000",
    "sltu": "0000000",
    "xor": "0000000",
    "srl": "0000000",
    "sra": "0100000",
    "or": "0000000"
}

dict_reg = {
    "zero": "00000",
    "ra": "00001",
    "sp": "00010",
    "gp": "00011",
    "tp": "00100",
    "t0": "00101",
    "t1": "00110",
    "t2": "00111",
    "s0": "01000",
    "fp": "01000",
    "s1": "01001",
    "a0": "01010",
    "a1": "01011",
    "a2": "01100",
    "a3": "01101",
    "a4": "01110",
    "a5": "01111",
    "a6": "10000",
    "a7": "10001",
    "s2": "10010",
    "s3": "10011",
    "s4": "10100",
    "s5": "10101",
    "s6": "10110",
    "s7": "10111",
    "s8": "11000",
    "s9": "11001",
    "s10": "11010",
    "s11": "11011",
    "t3": "11100",
    "t4": "11101",
    "t5": "11110",
    "t6": "11111"
}

dict_jump = {

}

def Itype(x, linenumber, f2, check):
    if(len(x) != 4):
        print(x)
        print("Error wrong number of arguments on line " + str(linenumber))
        exit()

    if(int(x[3]) > 2047 or int(x[3]) < -2048):
        if(x[0] == "addi"):
            if(check == 0):
                complicated_procedure(x, f2)
                return

    funct7 = dict_funct7.get(x[0], -1)
    opcode = dict_op[x[0]]
    funct3 = dict_funct3[x[0]]

    rd = dict_reg.get(x[1], -1)
    if(rd == -1):
        rd = bin(int(x[1]))
        newrd = rd[2:len(rd)]
        while(len(newrd) != 5):
            newrd = '0' + newrd
        rd = newrd

    rs1 = dict_reg.get(x[2], -1)
    if(rs1 == -1):
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
    else:
        imm = bin(int(x[3]) & 0b11111)
        newimm = imm[2:len(imm)]
        while(len(newimm) != 5):
            newimm = '0' + newimm
        imm = newimm
        instruction = funct7 + imm + rs1 + funct3 + rd + opcode
        instruction = '{:0{}X}'.format(int(instruction, 2), len(instruction) // 4)

    f2.write("0x")
    f2.write(instruction)
    f2.write("\n")

    return

def complicated_procedure(x, f2):
    utypeimm = str((int(x[3]) & 0b11111111111111111111000000000000) >> 12)
    imm = str((int(x[3]) & 0b00000000000000000000111111111111))
    newimm = bin(int(imm))
    newimm = newimm[2:len(newimm)]
    while(len(newimm) != 12):
        newimm = '0' + newimm

    if(newimm[0] == "1"):
        utypeimm = str(int(utypeimm) + 1)

    x2 = ["lui", x[1], utypeimm]
    Utype(x2, -1, f2)
    x3 = ["addi", x[1], x[1], imm]
    Itype(x3, -1, f2, 1)
    return

def Utype(x, linenumber, f2):
    opcode = dict_op[x[0]]

    rd = dict_reg.get(x[1], -1)
    if(rd == -1):
        rd = bin(int(x[1]))
        newrd = rd[2:len(rd)]
        while(len(newrd) != 5):
            newrd = '0' + newrd
        rd = newrd

    imm = bin(int(x[2]) & 0b11111111111111111111)
    newimm = imm[2:len(imm)]
    while(len(newimm) != 20):
        newimm = '0' + newimm
    imm = newimm

    instruction = imm + rd + opcode
    instruction = '{:0{}X}'.format(int(instruction, 2), len(instruction) // 4)

    f2.write("0x")
    f2.write(instruction)
    f2.write("\n")





def Rtype(x, linenumber, f2):
    funct7 = dict_funct7[x[0]]
    opcode = dict_op[x[0]]
    funct3 = dict_funct3[x[0]]

    rd = dict_reg.get(x[1], -1)
    if(rd == -1):
        rd = bin(int(x[1]))
        newrd = rd[2:len(rd)]
        while(len(newrd) != 5):
            newrd = '0' + newrd
        rd = newrd

    rs1 = dict_reg.get(x[2], -1)
    if(rs1 == -1):
        rs1 = bin(int(x[2]))
        newrs1 = rs1[2:len(rs1)]
        while(len(newrs1) != 5):
            newrs1 = '0' + newrs1
        rs1 = newrs1

    rs2 = dict_reg.get(x[3], -1)
    if(rs2 == -1):
        rs2 = bin(int(x[3]))
        newrs2 = rs2[2:len(rs2)]
        while(len(newrs2) != 5):
            newrs2 = '0' + newrs2
        rs2 = newrs2

    instruction = funct7 + rs2 + rs1 + funct3 + rd + opcode
    instruction = '{:0{}X}'.format(int(instruction, 2), len(instruction) // 4)

    f2.write("0x")
    f2.write(instruction)
    f2.write("\n")

    return

def Jtype(x, linenumber, f2):
    opcode = dict_op[x[0]]

    rd = dict_reg.get(x[1], -1)
    if(rd == -1):
        rd = bin(int(x[1]))
        newrd = rd[2:len(rd)]
        while(len(newrd) != 5):
            newrd = '0' + newrd
        rd = newrd


    if(x[2][0] == '.'):
        jump = dict_jump[x[2]]
        offset = (int(jump) - linenumber) * 2
        offset = bin(offset & 0b11111111111111111111)
        offset = offset[2:len(offset)]
        while(len(offset) != 20):
            offset = '0' + offset

    else:
        offset = bin(int(x[2]) & 0b11111111111111111111)
        offset = offset[2:len(offset)]
        while(len(offset) != 20):
            offset = '0' + offset

    imm1 = offset[0]
    imm2 = offset[10:20]
    imm3 = offset[9]
    imm4 = offset[1:9]

    imm = imm1 + imm2 + imm3 + imm4


    instruction = imm + rd + opcode
    instruction = '{:0{}X}'.format(int(instruction, 2), len(instruction) // 4)

    f2.write("0x")
    f2.write(instruction)
    f2.write("\n")

    return

def Stype(x, linenumber, f2):
    opcode = dict_op[x[0]]
    funct3 = dict_funct3[x[0]]

    rs1 = dict_reg.get(x[1], -1)
    if(rs1 == -1):
        rs1 = bin(int(x[1]))
        newrs1 = rs1[2:len(rs1)]
        while(len(newrs1) != 5):
            newrs1 = '0' + newrs1
        rs1 = newrs1

    rs2 = dict_reg.get(x[2], -1)
    if(rs2 == -1):
        rs2 = bin(int(x[2]))
        newrs2 = rs2[2:len(rs2)]
        while(len(newrs2) != 5):
            newrs2 = '0' + newrs2
        rs2 = newrs2

    offset = bin(int(x[3]) & 0b111111111111)
    offset = offset[2:len(offset)]
    while(len(offset) != 12):
        offset = '0' + offset

    imm = offset[7:len(offset)]

    imm2 = offset[0:7]

    instruction = imm2 + rs2 + rs1 + funct3 + imm + opcode
    instruction = '{:0{}X}'.format(int(instruction, 2), len(instruction) // 4)

    f2.write("0x")
    f2.write(instruction)
    f2.write("\n")

    return

def Btype(x, linenumber, f2):
    # print("Linenumber:" + str(linenumber))
    opcode = dict_op[x[0]]
    funct3 = dict_funct3[x[0]]

    rs1 = dict_reg.get(x[1], -1)
    if(rs1 == -1):
        rs1 = bin(int(x[1]))
        newrs1 = rs1[2:len(rs1)]
        while(len(newrs1) != 5):
            newrs1 = '0' + newrs1
        rs1 = newrs1

    rs2 = dict_reg.get(x[2], -1)
    if(rs2 == -1):
        rs2 = bin(int(x[2]))
        newrs2 = rs2[2:len(rs2)]
        while(len(newrs2) != 5):
            newrs2 = '0' + newrs2
        rs2 = newrs2

    if(x[3][0] == '.'):
        jump = dict_jump[x[3]]
        offset = (int(jump) - linenumber) * 2
        offset = bin((offset) & 0b111111111111)
        offset = offset[2:len(offset)]

    else:
        offset = bin(int(x[3]) & 0b111111111111)
        offset = offset[2:len(offset)]

    while(len(offset) != 12):
        offset = '0' + offset

    imm1 = offset[0]
    imm2 = offset[2:8]
    imm3 = offset[8:12]
    imm4 = offset[1]


    instruction = imm1 + imm2 + rs2 + rs1 + funct3 + imm3 + imm4 + opcode
    instruction = '{:0{}X}'.format(int(instruction, 2), len(instruction) // 4)

    f2.write("0x")
    f2.write(instruction)
    f2.write("\n")

    return


def main():
    if(len(sys.argv) < 3):
        print("Missing arguments")
        exit()
    if(len(sys.argv) > 3):
        print("Too many arguments")
        exit()
    with open(sys.argv[1]) as f:
        lines = f.readlines()
        f2 = open(sys.argv[2], "w")
        instruction = []
        lines2 = []
        lines3 = []
        lines4 = []
        for i in range(0, len(lines)):
            if(lines[i] == '\n'):
                continue
            lines2.append(lines[i])

        count = 0
        count3 = 0

        for i in range(0, len(lines2)):
            z = lines2[i].split(" ")
            if(z[0] == "addi"):
                if(int(z[3]) > 2047 or int(z[3]) < -2048):
                    count = count - 1
            if(lines2[i][0] == '.'):
                addr = lines2[i]
                x = [(i + 1 - count), addr[:-2]]
                count = count + 1
                dict_jump[str(x[1])] = str(x[0])
                continue
            lines3.append(lines2[i])

        for i in range(0, len(lines3)):
            x = lines3[i].split(" ")
            z = lines2[i].split(" ")
            if(z[0] == "addi"):
                if(int(z[3]) > 2047 or int(z[3]) < -2048):
                    count3 = count3 + 1
            if(x[2] == "return\n"):
                addr = (i + count3) * (-2) - 2
                x[2] = str(addr)
                y = x[0] + " " + x[1] + " " + x[2]
                lines4.append(y)
                continue
            if(i == (len(lines3))-1):
                if(x[2] != "return\n"):
                    lines4.append(lines3[i])
                    i = i + 1
                    addr = (i + count3) * (-2) - 2
                    y = "jal zero " + str(addr)
                    lines4.append(y)
                    continue
            lines4.append(lines3[i])

        count2 = 0
        for i in range(0, len(lines4)):
            x = lines4[i].split(" ")
            if(x[0] == '\n'):
                continue
            if(x[0][0] == '.'):
                continue
            for char in x[-1]:
                x[-1] = x[-1].strip('\n')
            opcode = dict[x[0]]
            if(opcode == 1):
                Itype(x, (i+1+count2), f2, 0)
            elif(opcode == 2):
                Utype(x, (i+1+count2), f2)
            elif(opcode == 3):
                Rtype(x, (i+1+count2), f2)
            elif(opcode == 4):
                Jtype(x, (i+1+count2), f2)
            elif(opcode == 5):
                Btype(x, (i+1+count2), f2)
            elif(opcode == 6):
                Stype(x, (i+1+count2), f2)
            if(x[0] == "addi"):
                if(int(x[3]) > 2047 or int(x[3]) < -2048):
                    count2 = count2 + 1

main()
