#include <stdio.h>
#include "run.h"
#include <stdint.h>
#include <stdlib.h>

#define SIZE 1024

#define XLEN 32

#define separator printf("====================================================\n");

//Instruction cache
static char Icache[SIZE];
//Data cache
static char Dcache[SIZE];

//Registers
int registers[32];
//Program Counter
int* pc = &registers[15];

//Source Registers
int rsource1;
int rsource2;
//Destination register
int rdestination;

//Current Instruction
uint32_t instruction;

//Current Instruction Type
//-1 = End, 1 = Immediate, 2 = Unsigned, 3 = Register, 4 = Jump, 5 = Branch, 6 = Store
int instruction_type;
char instruction_type_char;

//Current opcode
int opcode;

//Current Funct3
int funct3;

//Current Funct7
int funct7;

//Current Shift Amount
int shamt;

//Current Immediate Value
int imm;

//Jump Immediate Values
int jimm1;
int signedjimm1;
int jimm2;
int signedjimm2;

//Branch Immediate Values
int signedbimm1, bimm1, signedbimm2, bimm2;

//Store Immediate Values
int simm1, simm2;

//Jump Address
uint32_t addr;


//Two's complement
// int signedfunc(uint32_t regVal){
//   return 2^32
// }


//add register and immediate
int addi(int reg1){
  return reg1 + imm;
}

//subtract register and immediate
// int subi(uint32_t reg1, uint32_t immOperand){
//   return reg1 - immOperand;
// }

//bit wise and on register and immediate
int andi(int reg1){
  return (reg1 & imm);
}

//bit wise or on register and immediate
int ori(int reg1){
  return (reg1 | imm);
}

//bit wise xor on register and immediate
int xori(int reg1){
  return (reg1 ^ imm);
}

//set less than immediate
int slti(int reg1){
  if(reg1 < imm)return 1;
  else return 0;
}

//set less than immediate unsigned
int sltiu(int reg1){
  unsigned int reg1u = reg1;
  unsigned int immu = imm;
  if(reg1u < immu)return 1;
  else return 0;
}

//Logical Left Shift Immediate
int slli(int reg1){
  return (reg1 << shamt);
}

//Logical Right Shift Immediate
int srli(int reg1){
  return (reg1 >> shamt)
}

//Arithmetic Right Shift Immediate
int srai(int reg1){
  return (reg1 >> shamt);
}

//Load Upper Immediate
int lui(){
  return (imm & 0xFFFFF000);
}

//Add Upper Immediate to PC
int auipc(){
  return((imm & 0xFFFFF000) + pc[0]);
}

//Add two registers
int add(int reg1, int reg2){
  return (reg1 + reg2);
}

//Subtract two registers
int sub(int reg1, int reg2){
  return (reg1 - reg2);
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
  pc[0] = pc[0] + (imm);
  return originalPC;
}

//jump and link register
int jalr(int reg1){
  int originalPC = pc[0] + 4;
  pc[0] = (imm + reg1) & 0b0;
  return originalPC;
}

//branch if equal
void beq(int reg1, int reg2){
  if(reg1 == reg2){
    pc[0] = pc[0] + imm;
  }
  return;
}

//branch if not equal
void bne(int reg1, int reg2){
  if(reg1 != reg2){
    pc[0] = pc[0] + imm;
  }
  return;
}

//branch if less than
void blt(int reg1, int reg2){
  if(reg1 < reg2){
    pc[0] = pc[0] + imm;
  }
  return;
}

//branch if less than
void bltu(int reg1, int reg2){
  unsigned int reg1u = reg1, reg2u = reg2;
  if(reg1u < reg2u){
    pc[0] = pc[0] + imm;
  }
  return;
}

//branch if greater than
void bge(int reg1, int reg2){
  if(reg1 >= reg2){
    pc[0] = pc[0] + imm;
  }
  return;
}

void bgeu(int reg1, int reg2){
  unsigned int reg1u = reg1, reg2u = reg2;
  if(reg1u >= reg2u){
    pc[0] = pc[0] + imm;
  }
  return;
}

//load memory into register
int ld(char* cache, int reg1){
  int offset = imm + reg1;
  int* data;
  data = (int*)(cache + offset);
  return *data;
}

//load 32-bit memory into register
int lw(char* cache, int reg1){
  int offset = imm + reg1;
  int* data;
  data = (int*)(cache + offset);
  int data2 = ((*data) & 0xFFFFFFFF);
  return data2;
}

//load 16-bit memory into register
int lh(char* cache, int reg1){
  int offset = imm + reg1;
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
  int offset = imm + reg1;
  int* data;
  data = (int*)(cache + offset);
  int data2 = ((*data) & 0x0000FFFF);
  return data2;
}

//load 8-bit memory into register
int lb(char* cache, int reg1){
  int offset = imm + reg1;
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
  int offset = imm + reg1;
  int* data;
  data = (int*)(cache + offset);
  int data2 = ((*data) & 0x000000FF);
  return data2;
}

//store contents of register into memory
void st(char* cache, int reg1, int reg2){
  int offset = imm + reg1;
  int* data;
  data = (int*)(cache + offset);
  *data = reg2;
}

//store 32-bit value from low bits of register into memory
void sw(char* cache, int reg1, int reg2){
  int offset = imm + reg1;
  int* data;
  data = (int*)(cache + offset);
  *data = reg2;
}

//store 16-bit value from low bits of register into memory
void sh(char* cache, int reg1, int reg2){
  int offset = imm + reg1;
  int* data;
  data = (int*)(cache + offset);
  int reg16bit = (reg2 & 0x0000FFFF);
  *data = reg16bit;
}

//store 8-bit value from low bits of register into memory
void sb(char* cache, int reg1, int reg2){
  int offset = imm + reg1;
  int* data;
  data = (int*)(cache + offset);
  int reg8bit = (reg2 & 0x000000FF);
  *data = reg8bit;
}

// //multiply two registers
// int mul(uint32_t reg1, uint32_t reg2){
//   return reg1 * reg2;
// }
//
// //multiply register and immediate
// int muli(uint32_t reg1, uint32_t immOperand){
//   return reg1 * immOperand;
// }
//
// //divide two registers
// int divide(uint32_t reg1, uint32_t reg2){
//   return reg1 * reg2;
// }
//
// //divide register and immediate
// int dividei(uint32_t reg1, uint32_t immOperand){
//   return reg1 * immOperand;
// }



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
  instruction = ld(Icache, pc[0]);
  // printf("Current Instruction:%x\n", instruction);
}

void decode(){
  opcode = (instruction & 0x0000007F);
  printf("Opcode:%d\n",opcode);
  switch(opcode){
    case 0b0010011:
      instruction_type = 1;
      instruction_type_char = 'I';
      break;
    case 0b0000011:
      instruction_type = 1;
      instruction_type_char = 'I';
      break;
    case 0b0110111:
      instruction_type = 2;
      instruction_type_char = 'U';
      break;
    case 0b0010111:
      instruction_type = 2;
      instruction_type_char = 'U';
      break;
    case 0b0110011:
      instruction_type = 3;
      instruction_type_char = 'R';
      break;
    case 0b1101111:
      instruction_type = 4;
      instruction_type_char = 'J';
      break;
    case 0b1100011:
      instruction_type = 5;
      instruction_type_char = 'B';
      break;
    case 0b0100011:
      instruction_type = 6;
      instruction_type_char = 'S';
      break;
    default:
      instruction_type = -1;
      break;
  }

  //I type instructions
  if(instruction_type == 1){
    rdestination = (instruction & 0x00000F80) >> 7;
    funct3 = (instruction & 0x00007000) >> 12;
    rsource1 = (instruction & 0x000F8000) >> 15;
    if(funct3 == (0b001 || 0b101)){
      shamt = (instruction & 0x01F00000) >> 20;
      imm = (instruction & 0xFE000000) >> 25;
    }
    else if(opcode == 0b1100111 && funct3 == 0b000){
      imm = (instruction & 0xFFF00000) >> 20;
      imm = (imm & 0x00000001);
    }
    else{
      imm = (instruction & 0xFFF00000) >> 20;
      if((imm & 0x00000800)){
        imm = (imm | 0xFFFFF000);
      }
    }
  }
  //U type instructions
  else if(instruction_type == 2){
    rdestination = (instruction & 0x00000F80) >> 7;
    imm = (instruction & 0xFFFFF000);
  }
  //R type instructions
  else if(instruction_type == 3){
    rdestination = (instruction & 0x00000F80) >> 7;
    funct3 = (instruction & 0x00007000) >> 12;
    rsource1 = (instruction & 0x000F8000) >> 15;
    rsource2 = (instruction & 0x01F00000) >> 20;
    funct7 = (instruction & 0xFE000000) >> 25;
  }
  //J type instructions
  else if(instruction_type == 4){
    rdestination = (instruction & 0x00000F80) >> 7;
    imm = (((instruction & 0x000FF000) >> 12 << 12) | ((instruction & 0x001FF000) >> 20 << 11) | ((instruction & 0x7FE00000) >> 21 << 1) | ((instruction & 0x80000000) >> 31 << 20));
    if((imm & 0x00080000)){
      imm = (imm | 0xFFF00000);
    }
  }
  //B type instructions
  else if(instruction_type == 5){
    funct3 = (instruction & 0x00007000) >> 12;
    rsource1 = (instruction & 0x000F8000) >> 15;
    rsource2 = (instruction & 0x01F00000) >> 20;
    imm = (((instruction & 0x00000080) >> 7 << 11) | ((instruction & 0x00000F00) >> 8 << 1) | ((instruction & 0x7E000000) >> 25 << 5) | ((instruction & 0x10000000) >> 31 << 5));
  }
  //S type instructions
  else if(instruction_type == 6){
    funct3 = (instruction & 0x00007000) >> 12;
    rsource1 = (instruction & 0x000F8000) >> 15;
    rsource2 = (instruction & 0x01F00000) >> 20;
    imm = (((instruction & 0x00000F80) >> 7) | ((instruction & 0xFE000000) >> 25 << 5));
  }
}

//execute I format instructions
void execute_iformat(){
  if(opcode == 0b0010011){
    switch(funct3){
      case(0b000):
      printf("Instruction:Add Immediate\n");
      registers[rdestination] = addi(registers[rsource1]);
      break;
      case(0b111):
      printf("Instruction:And Immediate\n");
      registers[rdestination] = andi(registers[rsource1]);
      break;
      case(0b110):
      printf("Instruction:Or Immediate\n");
      registers[rdestination] = ori(registers[rsource1]);
      break;
      case(0b100):
      printf("Instruction:Xor Immediate\n");
      registers[rdestination] = xori(registers[rsource1]);
      break;
      case(0b010):
      printf("Instruction:STLI\n");
      registers[rdestination] = slti(registers[rsource1]);
      break;
      case(0b011):
      printf("Instruction:STLIU\n");
      registers[rdestination] = sltiu(registers[rsource1]);
      break;
      //TODO:still have shift functions to create
    }
  }
  else if(opcode == 0b0000011){
    switch(funct3){
      case(0b000):
        printf("Instruction:Load 8-bit\n");
        registers[rdestination] = lb(Dcache, registers[rsource1]);
        break;
      case(0b001):
        printf("Instruction:Load 16-bit\n");
        registers[rdestination] = lh(Dcache, registers[rsource1]);
        break;
      case(0b010):
        printf("Instruction:Load 32-bit\n");
        registers[rdestination] = lw(Dcache, registers[rsource1]);
        break;
      case(0b100):
        printf("Instruction:Load 8-bit Unsigned\n");
        registers[rdestination] = lbu(Dcache, registers[rsource1]);
        break;
      case(0b101):
        printf("Instruction:Load 16-bit Unsigned\n");
        registers[rdestination] = lhu(Dcache, registers[rsource1]);
        break;
    }
  }
  else if(opcode == 0b1100111){
    printf("Instruction:Jump and Link Register\n");
    registers[rdestination] = jalr(registers[rsource1]);
  }
  else{
    printf("Error\n");
    exit(1);
  }
  return;
}

void execute_uformat(){
  switch(opcode){
    case(0b0110111):
      printf("Instruction:Load Upper Immediate\n");
      lui();
      break;
    case(0b0010111):
      printf("Instruction:Add Upper Immediate to PC\n");
      auipc();
      break;
  }
  return;
}

void execute_rformat(){
  switch(funct3){
    case(0b000):
      if(funct7 == 0b0000000){
        printf("Instruction:Add\n");
        add(registers[rsource1], registers[rsource2]);
      }
      else if(funct7 == 0b0100000){
        printf("Instruction:Subtract\n");
        sub(registers[rsource1], registers[rsource2]);
      }
      break;
    case(0b001):
      printf("Instruction:Logical Left Shift\n");
      sll(registers[rsource1], registers[rsource2]);
      break;
    case(0b010):
      printf("Instruction:SLT\n");
      slt(registers[rsource1], registers[rsource2]);
      break;
    case(0b011):
      printf("Instruction:SLTU\n");
      sltu(registers[rsource1], registers[rsource2]);
      break;
    case(0b100):
      printf("Instruction:Xor\n");
      xor(registers[rsource1], registers[rsource2]);
      break;
    case(0b101):
      if(funct7 == 0b0000000){
        printf("Instruction:Logical Right Shift\n");
        srl(registers[rsource1], registers[rsource2]);
      }
      else if(funct7 == 0b0100000){
        printf("Instruction:Logical Right Shift\n");
        sra(registers[rsource1], registers[rsource2]);
      }
      break;
    case(0b110):
      printf("Instruction:Or\n");
      or(registers[rsource1], registers[rsource2]);
      break;
    case(0b111):
      printf("Instruction:And\n");
      and(registers[rsource1], registers[rsource2]);
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
  switch(funct3){
    case(0b000):
      printf("Instruction:Branch if Equal\n");
      beq(registers[rsource1], registers[rsource2]);
      break;
    case(0b001):
      printf("Instruction:Branch if Not Equal\n");
      bne(registers[rsource1], registers[rsource2]);
      break;
    case(0b100):
      printf("Instruction:Branch if Less Than\n");
      blt(registers[rsource1], registers[rsource2]);
      break;
    case(0b101):
      printf("Instruction:Branch if Greater Than\n");
      bge(registers[rsource1], registers[rsource2]);
      break;
    case(0b110):
      printf("Instruction:Branch if Less Than Unsigned\n");
      bltu(registers[rsource1], registers[rsource2]);
      break;
    case(0b111):
      printf("Instruction:Branch if Greater Than Unsigned\n");
      bgeu(registers[rsource1], registers[rsource2]);
      break;
  }
  return;
}

void execute_sformat(){
  switch(funct3){
    case(0b000):
      printf("Instruction:Store 8-bit\n");
      sb(Dcache, registers[rsource1], registers[rsource2]);
      break;
    case(0b001):
      printf("Instruction:Store 16-bit\n");
      sh(Dcache, registers[rsource1], registers[rsource2]);
      break;
    case(0b010):
      printf("Instruction:Store 32-bit\n");
      sw(Dcache, registers[rsource1], registers[rsource2]);
      break;
  }
  return;
}

void execute_sj(){
  if(instruction_type == 4){
    execute_jformat();
  }
  else if(instruction_type == 7){
    execute_sformat();
  }
  return;
}

void execute(){
  if(instruction_type == -1){
    printf("End of Program\n");
    exit(1);
  }
  printf("Instruction Type:%c\n", instruction_type_char);
  if(instruction_type == 4 || instruction_type == 6){
    execute_sj();
    return;
  }

  if(rdestination == 0){
    printf("Error, register destination cannot be written into\n");
    exit(1);
  }
  if(instruction_type == 1){
    execute_iformat();
  }
  else if(instruction_type == 2){
    execute_uformat();
  }
  else if(instruction_type == 3){
    execute_rformat();
  }
  else if(instruction_type == 5){
    execute_bformat();
  }
  return;
}

//runs the simulation
void run(){
  int cycle = 0;
  while(1){
    separator;
    printf("Cycle:%d\n", cycle);
    fetch();

    if(instruction == -1){
      printf("End at address:%d\n",pc[0]);
      separator;
      break;
    }

    decode();

    execute();

    //Print all register values
    for(int i = 0; i < 31; i++){
      printf("r%d:%d\t", i, registers[i]);
      if(i % 10 == 0 && i != 0)printf("\n");
    }
      printf("r31:%d\n", registers[31]);
      printf("Rsources:%d\t%d\n",rsource1, rsource2);
      printf("rdestination:%d\n", rdestination);


    pc[0] = pc[0] + 4;
    cycle++;
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
}

void load_program(char* file){
  FILE* fp;
  fp = fopen(file, "r");
  unsigned int tinstruction, loadpc = 0;
  imm = 0;
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
  // for(int i = 0; i < 20; i++){
  //   printf("%x\n", Icache[i]);
  // }
  printf("%x\n",Icache[0]);
  separator;

  run();

  // printf("HELLO:%u\n", y);
  // unsigned short int x = 65529U;
  // short int y = *(short int*)&x;
  //
  // printf("%d\n", y);

  return 0;
}
