#include <stdio.h>
#include "run.h"
#include <stdint.h>
#include <stdlib.h>

#define SIZE 1024

#define XLEN 32

#define NUM_STAGES 3

#define separator printf("====================================================\n");

//Current Cycle
int current_cycle = 1;

//Instructions executed
int instructions_executed = 0;

//Instruction cache
static char Icache[SIZE];
//Data cache
static char Dcache[SIZE];

//Registers
int registers[32];
//Program Counter
int pc[1] = {0};
int* sp = &registers[2];
//Source Registers
int next_rsource1, current_rsource1;
int next_rsource2, current_rsource2;
//Destination register
int next_rdestination, current_rdestination;

//Current Instruction
uint32_t fetch_current_instruction;
uint32_t fetch_next_instruction;


//Current Instruction Type
//-1 = End, 1 = Immediate, 2 = Unsigned, 3 = Register, 4 = Jump, 5 = Branch, 6 = Store
int next_instruction_type, current_instruction_type;
char instruction_type_char;

//first x
int first_fetch = 0, first_decode = 0, first_execute = 0;

//last instruction
int last_instruction = 0;
int last_instruction_cycle;

//Current current_opcode
int current_opcode, next_opcode;

//Current Funct3
int current_funct3, next_funct3;

//Current Funct7
int current_funct7, next_funct7;

//Current Shift Amount
int current_shamt, next_shamt;

//Current Immediate Value
int current_imm, next_imm;

int current_val, next_val;

//add register and immediate
int addi(int reg1){
  return reg1 + current_imm;
}

//subtract register and immediate
// int subi(uint32_t reg1, uint32_t immOperand){
//   return reg1 - immOperand;
// }

//bit wise and on register and immediate
int andi(int reg1){
  return (reg1 & current_imm);
}

//bit wise or on register and immediate
int ori(int reg1){
  return (reg1 | current_imm);
}

//bit wise xor on register and immediate
int xori(int reg1){
  return (reg1 ^ current_imm);
}

//set less than immediate
int slti(int reg1){
  if(reg1 < current_imm)return 1;
  else return 0;
}

//set less than immediate unsigned
int sltiu(int reg1){
  unsigned int reg1u = reg1;
  unsigned int immu = current_imm;
  if(reg1u < immu)return 1;
  else return 0;
}

//Logical Left Shift Immediate
int slli(int reg1){
  unsigned int reg1u = reg1;
  reg1u = (reg1u << current_shamt);
  reg1 = (int)reg1u;
  return (reg1 & 0xFFFFFFFF);
}

//Logical Right Shift Immediate
int srli(int reg1){
  unsigned int reg1u = reg1;
  reg1u = (reg1u >> current_shamt);
  reg1 = (int)reg1u;
  return (reg1 & 0xFFFFFFFF);
}

//Arithmetic Right Shift Immediate
int srai(int reg1){
  return ((reg1 >> current_shamt) & 0xFFFFFFFF);
}

//Load Upper Immediate
int lui(){
  return (current_imm & 0xFFFFF000);
}

//Add Upper Immediate to PC
int auipc(){
  return((current_imm & 0xFFFFF000) + pc[0]);
}

//Add two registers
int add(int reg1, int reg2){
  return ((reg1 + reg2) & 0xFFFFFFFF);
}

//Subtract two registers
int sub(int reg1, int reg2){
  return ((reg1 - reg2) & 0xFFFFFFFF);
}

//Set if Less Than
int slt(int reg1, int reg2){
  if(reg1 < reg2)return 1;
  else return 0;
}

//Set if Less Than Unsigned
int sltu(int reg1, int reg2){
  unsigned int reg2u = reg2;
  if(reg2u != 0)return 1;
  else return 0;
}

//bit wise and on two registers
int and(int reg1, int reg2){
  return(reg1 & reg2);
}

//bit wise or on two registers
int or(int reg1, int reg2){
  return(reg1 | reg2);
}

//bit wise xor on two registers
int xor(int reg1, int reg2){
  return(reg1 ^ reg2);
}

//multiply two registers
int mul(int reg1, int reg2){
  return((reg1 * reg2) & 0xFFFFFFFF);
}

//divide two registers
int divide(int reg1, int reg2){
  return((reg1 / reg2) & 0xFFFFFFFF);
}

//logical left shift
int sll(int reg1, int reg2){
  int shift = (reg2 & 0x1F);
  return(reg1 << shift);
}

//logical rigth shift
int srl(int reg1, int reg2){
  int shift = (reg2 & 0x1F);
  return(reg1 >> shift);
}

//arithmetic roght shift
int sra(int reg1, int reg2){
  int shift = (reg2 & 0x1F);
  return(reg1 >> shift);
}

//jump and link
int jal(){
  int originalPC = pc[0] + 4;
  pc[0] = pc[0] + (current_imm);
  return originalPC;
}

//jump and link register
int jalr(int reg1){
  int originalPC = pc[0] + 4;
  pc[0] = (current_imm + reg1) & 0b0;
  return originalPC;
}

//branch if equal
void beq(int reg1, int reg2){
  if(reg1 == reg2){
    pc[0] = pc[0] + current_imm;
  }
  return;
}

//branch if not equal
void bne(int reg1, int reg2){
  if(reg1 != reg2){
    pc[0] = pc[0] + current_imm;
  }
  return;
}

//branch if less than
void blt(int reg1, int reg2){
  printf("offset:%x\n",current_imm);
  if(reg1 < reg2){
    pc[0] = pc[0] + current_imm;
  }
  return;
}

//branch if less than
void bltu(int reg1, int reg2){
  unsigned int reg1u = reg1, reg2u = reg2;
  if(reg1u < reg2u){
    pc[0] = pc[0] + current_imm;
  }
  return;
}

//branch if greater than
void bge(int reg1, int reg2){
  if(reg1 >= reg2){
    pc[0] = pc[0] + current_imm;
  }
  return;
}

void bgeu(int reg1, int reg2){
  unsigned int reg1u = reg1, reg2u = reg2;
  if(reg1u >= reg2u){
    pc[0] = pc[0] + current_imm;
  }
  return;
}

//load memory into register for instructions cache
int ld(char* cache, int reg1){
  int offset = reg1;
  int* data;
  data = (int*)(cache + offset);
  return *data;
}

//load 32-bit memory into register
int lw(char* cache, int reg1){
  int offset = current_imm + reg1;
  int* data;
  data = (int*)(cache + offset);
  int data2 = ((*data) & 0xFFFFFFFF);
  return data2;
}

//load 16-bit memory into register
int lh(char* cache, int reg1){
  int offset = current_imm + reg1;
  int* data;
  data = (int*)(cache + offset);
  int data2 = ((*data) & 0x0000FFFF);
  if(data2 & 0x00008000){
    data2 = (data2 | 0xFFFF0000);
  }
  return data2;
}

//load unsigned 16-bit memory into register
int lhu(char* cache, int reg1){
  int offset = current_imm + reg1;
  int* data;
  data = (int*)(cache + offset);
  int data2 = ((*data) & 0x0000FFFF);
  return data2;
}

//load 8-bit memory into register
int lb(char* cache, int reg1){
  int offset = current_imm + reg1;
  int* data;
  data = (int*)(cache + offset);
  int data2 = ((*data) & 0x000000FF);
  if(data2 & 0x00000080){
    data2 = (data2 | 0xFFFFFF00);
  }
  return data2;
}

//load unsigned 8-bit memory into register
int lbu(char* cache, int reg1){
  int offset = current_imm + reg1;
  int* data;
  data = (int*)(cache + offset);
  int data2 = ((*data) & 0x000000FF);
  return data2;
}

//store contents of register into memory
void st(char* cache, int reg1, int reg2){
  int offset = current_imm + reg1;
  int* data;
  data = (int*)(cache + offset);
  *data = reg2;
}

//store 32-bit value from low bits of register into memory
void sw(char* cache, int reg1, int reg2){
  int offset = current_imm + reg1;
  int* data;
  data = (int*)(cache + offset);
  *data = reg2;
}

//store 16-bit value from low bits of register into memory
void sh(char* cache, int reg1, int reg2){
  int offset = current_imm + reg1;
  int* data;
  data = (int*)(cache + offset);
  int reg16bit = (reg2 & 0x0000FFFF);
  *data = reg16bit;
}

//store 8-bit value from low bits of register into memory
void sb(char* cache, int reg1, int reg2){
  int offset = current_imm + reg1;
  int* data;
  data = (int*)(cache + offset);
  int reg8bit = (reg2 & 0x000000FF);
  *data = reg8bit;
}

//compare two registers
int cmp(uint32_t reg1, uint32_t reg2){
  if(reg1 < reg2) return -1;
  else if(reg1 == reg2) return 0;
  else if(reg1 > reg2) return 1;
  else return 404;
}

int nand(uint32_t reg1, uint32_t reg2){
  if(!(reg1 & reg2))return 1;
  else return 0;
}

//move value to register
void mov(uint32_t* reg, uint32_t val){
  *reg = val;
}

void fetch(){
  printf("Current PC:%d\n",pc[0]);
  if(last_instruction == 1)return;
  fetch_next_instruction = ld(Icache, pc[0]);
  if(first_fetch < 2){
    first_fetch = first_fetch + 1;
  }
  if(fetch_next_instruction == -1 && last_instruction != 1){
    last_instruction = 1;
    last_instruction_cycle = current_cycle - 1;
  }
  printf("Current Instruction:%x\n", fetch_next_instruction);
}

void decode(){
  if(first_fetch < 2)return;
  if(last_instruction == 1){
    if(current_cycle > last_instruction_cycle + 1)return;
  }
  if(first_decode < 2){
    first_decode = first_decode + 1;
  }


  next_opcode = (fetch_current_instruction & 0x0000007F);
  printf("Opcode:%d\n",next_opcode);
  switch(next_opcode){
    case 0b0010011:
      next_instruction_type = 1;
      instruction_type_char = 'I';
      break;
    case 0b0000011:
      next_instruction_type = 1;
      instruction_type_char = 'I';
      break;
    case 0b0110111:
      next_instruction_type = 2;
      instruction_type_char = 'U';
      break;
    case 0b0010111:
      next_instruction_type = 2;
      instruction_type_char = 'U';
      break;
    case 0b0110011:
      next_instruction_type = 3;
      instruction_type_char = 'R';
      break;
    case 0b1101111:
      next_instruction_type = 4;
      instruction_type_char = 'J';
      break;
    case 0b1100011:
      next_instruction_type = 5;
      instruction_type_char = 'B';
      break;
    case 0b0100011:
      next_instruction_type = 6;
      instruction_type_char = 'S';
      break;
    default:
      next_instruction_type = -1;
      break;
  }

  //I type instructions
  if(next_instruction_type == 1){
    next_rdestination = (fetch_current_instruction & 0x00000F80) >> 7;
    next_funct3 = (fetch_current_instruction & 0x00007000) >> 12;
    next_rsource1 = (fetch_current_instruction & 0x000F8000) >> 15;
    if(next_funct3 == 0b101 || next_funct3 == 0b001){
      next_shamt = (fetch_current_instruction & 0x01F00000) >> 20;
      next_imm = (fetch_current_instruction & 0xFE000000) >> 25;
    }
    else if(next_opcode == 0b1100111 && next_funct3 == 0b000){
      next_imm = (fetch_current_instruction & 0xFFF00000) >> 20;
      next_imm = (next_imm & 0x00000001);
    }
    else{
      next_imm = (fetch_current_instruction & 0xFFF00000) >> 20;
      if((next_imm & 0x00000800)){
        next_imm = (next_imm | 0xFFFFF000);
      }
    }
  }
  //U type instructions
  else if(next_instruction_type == 2){
    next_rdestination = (fetch_current_instruction & 0x00000F80) >> 7;
    next_imm = (fetch_current_instruction & 0xFFFFF000);
  }
  //R type instructions
  else if(next_instruction_type == 3){
    next_rdestination = (fetch_current_instruction & 0x00000F80) >> 7;
    next_funct3 = (fetch_current_instruction & 0x00007000) >> 12;
    next_rsource1 = (fetch_current_instruction & 0x000F8000) >> 15;
    next_rsource2 = (fetch_current_instruction & 0x01F00000) >> 20;
    next_funct7 = (fetch_current_instruction & 0xFE000000) >> 25;
  }
  //J type instructions
  else if(next_instruction_type == 4){
    next_rdestination = (fetch_current_instruction & 0x00000F80) >> 7;
    next_imm = (((fetch_current_instruction & 0x000FF000) >> 12 << 12) | ((fetch_current_instruction & 0x001FF000) >> 20 << 11) | ((fetch_current_instruction & 0x7FE00000) >> 21 << 1) | ((fetch_current_instruction & 0x80000000) >> 31 << 20));
    if((next_imm & 0x00080000)){
      next_imm = (next_imm | 0xFFF00000);
    }
  }
  //B type instructions
  else if(next_instruction_type == 5){
    next_funct3 = (fetch_current_instruction & 0x00007000) >> 12;
    next_rsource1 = (fetch_current_instruction & 0x000F8000) >> 15;
    next_rsource2 = (fetch_current_instruction & 0x01F00000) >> 20;
    next_imm = (((fetch_current_instruction & 0x00000080) >> 7 << 11) | ((fetch_current_instruction & 0x00000F00) >> 8 << 1) | ((fetch_current_instruction & 0x7E000000) >> 25 << 5) | ((fetch_current_instruction & 0x10000000) >> 31 << 5));
    if((next_imm & 0x00000800)){
      next_imm = (next_imm | 0xFFFFF000);
    }
  }
  //S type instructions
  else if(next_instruction_type == 6){
    next_funct3 = (fetch_current_instruction & 0x00007000) >> 12;
    next_rsource1 = (fetch_current_instruction & 0x000F8000) >> 15;
    next_rsource2 = (fetch_current_instruction & 0x01F00000) >> 20;
    next_imm = (((fetch_current_instruction & 0x00000F80) >> 7) | ((fetch_current_instruction & 0xFE000000) >> 25 << 5));
    if((next_imm & 0x00000800)){
      next_imm = (next_imm | 0xFFFFF000);
    }
  }
}

//execute I format instructions
void execute_iformat(){
  if(current_opcode == 0b0010011){
    switch(current_funct3){
      case(0b000):
        printf("Instruction:Add Immediate\n");
        next_val = addi(registers[current_rsource1]);
        break;
      case(0b111):
        printf("Instruction:And Immediate\n");
        next_val = andi(registers[current_rsource1]);
        break;
      case(0b110):
        printf("Instruction:Or Immediate\n");
        next_val = ori(registers[current_rsource1]);
        break;
      case(0b100):
        printf("Instruction:Xor Immediate\n");
        next_val = xori(registers[current_rsource1]);
        break;
      case(0b010):
        printf("Instruction:STLI\n");
        next_val = slti(registers[current_rsource1]);
        break;
      case(0b011):
        printf("Instruction:STLIU\n");
        next_val = sltiu(registers[current_rsource1]);
        break;
      case(0b001):
        printf("Instruction:Logical Left Shift Immediate\n");
        next_val = slli(registers[current_rsource1]);
        break;
      case(0b101):
        if(current_imm == 0b0000000){
          printf("Instruction:Logical Right Shift Immediate\n");
          next_val = srli(registers[current_rsource1]);
          break;
        }
        else if(current_imm == 0b0100000){
          printf("Instruction:Arithmetic Right Shift Immediate\n");
          next_val = srai(registers[current_rsource1]);
          break;
        }


    }
  }
  else if(current_opcode == 0b0000011){
    switch(current_funct3){
      case(0b000):
        printf("Instruction:Load 8-bit\n");
        next_val = lb(Dcache, registers[current_rsource1]);
        break;
      case(0b001):
        printf("Instruction:Load 16-bit\n");
        next_val = lh(Dcache, registers[current_rsource1]);
        break;
      case(0b010):
        printf("Instruction:Load 32-bit\n");
        next_val = lw(Dcache, registers[current_rsource1]);
        break;
      case(0b100):
        printf("Instruction:Load 8-bit Unsigned\n");
        next_val = lbu(Dcache, registers[current_rsource1]);
        break;
      case(0b101):
        printf("Instruction:Load 16-bit Unsigned\n");
        next_val = lhu(Dcache, registers[current_rsource1]);
        break;
    }
  }
  else if(current_opcode == 0b1100111){
    printf("Instruction:Jump and Link Register\n");
    next_val = jalr(registers[current_rsource1]);
  }
  else{
    printf("Error\n");
    exit(1);
  }
  return;
}

void execute_uformat(){
  switch(current_opcode){
    case(0b0110111):
      printf("Instruction:Load Upper Immediate\n");
      next_val = lui();
      break;
    case(0b0010111):
      printf("Instruction:Add Upper Immediate to PC\n");
      auipc();
      break;
  }
  return;
}

void execute_rformat(){
  switch(current_funct3){
    case(0b000):
      if(current_funct7 == 0b0000000){
        printf("Instruction:Add\n");
        next_val = add(registers[current_rsource1], registers[current_rsource2]);
      }
      else if(current_funct7 == 0b0100000){
        printf("Instruction:Subtract\n");
        next_val = sub(registers[current_rsource1], registers[current_rsource2]);
      }
      else if(current_funct7 == 0b0000001){
        printf("Instruction:Multiply\n");
        next_val = mul(registers[current_rsource1], registers[current_rsource2]);
      }
      break;
    case(0b001):
      printf("Instruction:Logical Left Shift\n");
      next_val = sll(registers[current_rsource1], registers[current_rsource2]);
      break;
    case(0b010):
      printf("Instruction:SLT\n");
      next_val = slt(registers[current_rsource1], registers[current_rsource2]);
      break;
    case(0b011):
      printf("Instruction:SLTU\n");
      next_val = sltu(registers[current_rsource1], registers[current_rsource2]);
      break;
    case(0b100):
      if(current_funct7 == 0b0000001){
        printf("Instruction:Divide\n");
        next_val = divide(registers[current_rsource1], registers[current_rsource2]);
        break;
      }
      else if(current_funct7 == 0b0000000){
        printf("Instruction:Xor\n");
        next_val = xor(registers[current_rsource1], registers[current_rsource2]);
        break;
      }
    case(0b101):
      if(current_funct7 == 0b0000000){
        printf("Instruction:Logical Right Shift\n");
        next_val = srl(registers[current_rsource1], registers[current_rsource2]);
      }
      else if(current_funct7 == 0b0100000){
        printf("Instruction:Logical Right Shift\n");
        next_val = sra(registers[current_rsource1], registers[current_rsource2]);
      }
      break;
    case(0b110):
      printf("Instruction:Or\n");
      next_val = or(registers[current_rsource1], registers[current_rsource2]);
      break;
    case(0b111):
      printf("Instruction:And\n");
      next_val = and(registers[current_rsource1], registers[current_rsource2]);
      break;
  }
  return;
}

void execute_jformat(){
  printf("Instruction:Jump and Link\n");
  jal();
  return;
}

void execute_bformat(){
  switch(current_funct3){
    case(0b000):
      printf("Instruction:Branch if Equal\n");
      beq(registers[current_rsource1], registers[current_rsource2]);
      break;
    case(0b001):
      printf("Instruction:Branch if Not Equal\n");
      bne(registers[current_rsource1], registers[current_rsource2]);
      break;
    case(0b100):
      printf("Instruction:Branch if Less Than\n");
      blt(registers[current_rsource1], registers[current_rsource2]);
      break;
    case(0b101):
      printf("Instruction:Branch if Greater Than\n");
      bge(registers[current_rsource1], registers[current_rsource2]);
      break;
    case(0b110):
      printf("Instruction:Branch if Less Than Unsigned\n");
      bltu(registers[current_rsource1], registers[current_rsource2]);
      break;
    case(0b111):
      printf("Instruction:Branch if Greater Than Unsigned\n");
      bgeu(registers[current_rsource1], registers[current_rsource2]);
      break;
  }
  return;
}

void execute_sformat(){
  switch(current_funct3){
    case(0b000):
      printf("Instruction:Store 8-bit\n");
      sb(Dcache, registers[current_rsource1], registers[current_rsource2]);
      break;
    case(0b001):
      printf("Instruction:Store 16-bit\n");
      sh(Dcache, registers[current_rsource1], registers[current_rsource2]);
      break;
    case(0b010):
      printf("Instruction:Store 32-bit\n");
      sw(Dcache, registers[current_rsource1], registers[current_rsource2]);
      break;
  }
  return;
}

void execute_sj(){
  if(current_instruction_type == 4){
    execute_jformat();
  }
  else if(current_instruction_type == 6){
    execute_sformat();
  }
  return;
}

void execute(){

  if(first_decode < 2)return;

  if(last_instruction == 1){
    if(current_cycle > last_instruction_cycle + 2)return;
  }

  if(current_instruction_type == -1){
    printf("End of Program\n");
    exit(1);
  }

  instructions_executed++;

  printf("Instruction Type:%c\n", instruction_type_char);
  if(current_instruction_type == 4 || current_instruction_type == 6){
    execute_sj();
    return;
  }

  if(current_rdestination == 0){
    printf("Error, register destination cannot be written into\n");
    exit(1);
  }
  if(current_instruction_type == 1){
    execute_iformat();
  }
  else if(current_instruction_type == 2){
    execute_uformat();
  }
  else if(current_instruction_type == 3){
    execute_rformat();
  }
  else if(current_instruction_type == 5){
    execute_bformat();
  }
  return;
}

void move_next_to_current(){
  //fetch
  fetch_current_instruction = fetch_next_instruction;

  //decode
  current_rsource1 = next_rsource1;
  current_rsource2 = next_rsource2;
  current_rdestination = next_rdestination;
  current_instruction_type = next_instruction_type;
  current_opcode = next_opcode;
  current_funct3 = next_funct3;
  current_funct7 = next_funct7;
  current_shamt = next_shamt;
  current_imm = next_imm;

  //execute
  current_val = next_val;
  //memory access

  //write-back
  return;
}

//runs the simulation
void run(){
  while(1){
    separator;
    printf("Cycle:%d\n", current_cycle);

    if(last_instruction == 1){
      printf("LASTINSTRUCTION:%d\n",last_instruction_cycle);
      if(current_cycle > last_instruction_cycle + (NUM_STAGES - 1)){
          printf("End of program\n");
          printf("Number of Cycles to complete:%d\n",current_cycle - 1);
          printf("Number of Instructions executed:%d\n",instructions_executed);
          printf("Number of instructions per cycle:%.2f\n",((float)instructions_executed / (float)(current_cycle - 1)));
          separator;
          break;
      }
    }

    fetch();

    // if(instruction == -1){
    //   printf("End at address:%d\n",pc[0]);
    //   separator;
    //   break;
    // }

    decode();

    execute();

    registers[current_rdestination] = next_val;

    //Print all register values
    for(int i = 0; i < 31; i++){
      switch(i){
        case(0):
          printf("zero:%d\t", registers[i]);
          break;
        case(1):
          printf("ra:%d\t", registers[i]);
          break;
        case(2):
          printf("sp:%d\t", registers[i]);
          break;
        case(3):
          printf("gp:%d\t", registers[i]);
          break;
        case(4):
          printf("tp:%d\t", registers[i]);
          break;
        case(5):
          printf("t0:%d\t", registers[i]);
          break;
        case(6):
          printf("t1:%d\t", registers[i]);
          break;
        case(7):
          printf("t2:%d\t", registers[i]);
          break;
        case(8):
          printf("s0:%d\t", registers[i]);
          break;
        case(9):
          printf("s1:%d\t", registers[i]);
          break;
        case(10):
          printf("a0:%d\t", registers[i]);
          break;
        case(11):
          printf("a1:%d\t", registers[i]);
          break;
        case(12):
          printf("a2:%d\t", registers[i]);
          break;
        case(13):
          printf("a3:%d\t", registers[i]);
          break;
        case(14):
          printf("a4:%d\t", registers[i]);
          break;
        case(15):
          printf("a5:%d\t", registers[i]);
          break;
        case(16):
          printf("a6:%d\t", registers[i]);
          break;
        case(17):
          printf("a7:%d\t", registers[i]);
          break;
        case(18):
          printf("s2:%d\t", registers[i]);
          break;
        case(19):
          printf("s3:%d\t", registers[i]);
          break;
        case(20):
          printf("s4:%d\t", registers[i]);
          break;
        case(21):
          printf("s5:%d\t", registers[i]);
          break;
        case(22):
          printf("s6:%d\t", registers[i]);
          break;
        case(23):
          printf("s7:%d\t", registers[i]);
          break;
        case(24):
          printf("s8:%d\t", registers[i]);
          break;
        case(25):
          printf("s9:%d\t", registers[i]);
          break;
        case(26):
          printf("s10:%d\t", registers[i]);
          break;
        case(27):
          printf("s11:%d\t", registers[i]);
          break;
        case(28):
          printf("t3:%d\t", registers[i]);
          break;
        case(29):
          printf("t4:%d\t", registers[i]);
          break;
        case(30):
          printf("t5:%d\t", registers[i]);
          break;
        case(31):
          printf("t6:%d\t", registers[i]);
          break;
      }
      if((i+1) % 5 == 0)printf("\n");
    }
    printf("\n");
    move_next_to_current();

    pc[0] = pc[0] + 4;
    current_cycle++;
    separator;
}
}

//sets registers to 0
void set_register(){
  size_t n = sizeof(registers)/sizeof(*registers);
  for(int i = 0; i < n; i++){
    registers[i] = 0;
  }
  for(int i = 0; i < SIZE; i++){
    st(Icache, i, -1);
  }
  sp[0] = SIZE;
}

void load_program(char* file){
  FILE* fp;
  fp = fopen(file, "r");
  unsigned int tinstruction, loadpc = 0;
  next_imm = 0;
  if(fp == NULL){
    printf("Error in loading program, file is null");
    exit(1);
  }
  while(fscanf(fp, "%x", &tinstruction) != EOF){
    st(Icache, loadpc, tinstruction);
    loadpc = loadpc + 4;
  }

  fclose(fp);
}

int main(int argc, char** argv){

  if(argc < 2){
    printf("Incorrect number of arguments");
    exit(1);
  }


  set_register();
  separator;
  printf("Loading Program\n");
  load_program(argv[1]);

  separator;

  run();

  return 0;
}
