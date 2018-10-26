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
//0 = N/A, 1 = Immediate, 2 = Unsigned, 3 = Register, 4 = Jump, 5 = Branch, 6 = Store
int instruction_type;

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
  printf("Instruction:Add Immediate\n");
  return reg1 + imm;
}

//subtract register and immediate
int subi(uint32_t reg1, uint32_t immOperand){
  printf("Instruction:Subtract Immediates\n");
  return reg1 - immOperand;
}

//bit wise and on register and immediate
int andi(int reg1){
  printf("Instruction:And Immediate\n");
  return (reg1 & imm);
}

//bit wise or on register and immediate
int ori(int reg1){
  printf("Instruction:Or Immediate\n");
  return (reg1 | imm);
}

//bit wise xor on register and immediate
int xori(int reg1){
  printf("Instruction:Xor Immediate\n");
  return (reg1 ^ imm);
}

//set less than immediate
int slti(int reg1){
  printf("Instruction:STLI\n");
  if(reg1 < imm)return 1;
  else return 0;
}

//set less than immediate unsigned
int sltiu(int reg1){
  printf("Instruction:STLIU\n");
  unsigned int reg1u = reg1;
  unsigned int immu = imm;
  if(reg1u < immu)return 1;
  else return 0;
}

//Load Upper Immediate
int lui(){
  printf("Instruction:Load Upper Immediate\n");
  return (imm & 0xFFFFF000);
}

//Add Upper Immediate to PC
int auipc(){
  printf("Instruction:Add Upper Immediate to PC\n");
  return((imm & 0xFFFFF000) + pc[0]);
}

//Add two registers
int add(int reg1, int reg2){
  printf("Instruction:Add\n");
  return (reg1 + reg2);
}

//Subtract two registers
int sub(int reg1, int reg2){
  printf("Instruction:Subtract\n");
  return (reg1 - reg2);
}

//Set if Less Than
int slt(int reg1, int reg2){
  printf("Instruction:SLT\n");
  if(reg1 < reg2)return 1;
  else return 0;
}

//Set if Less Than Unsigned
int sltu(int reg1, int reg2){
  printf("Instruction:SLTU\n");
  unsigned int reg2u = reg2;
  if(reg2u != 0)return 1;
  else return 0;
}

//bit wise and on two registers
int and(int reg1, int reg2){
  printf("Instruction:And\n");
  return(reg1 & reg2);
}

//bit wise or on two registers
int or(int reg1, int reg2){
  printf("Instruction:Or\n");
  return(reg1 | reg2);
}

//bit wise xor on two registers
int xor(int reg1, int reg2){
  printf("Instruction:Xor\n");
  return(reg1 ^ reg2);
}

//logical left shift
int sll(int reg1, int reg2){
  printf("Instruction:Logical Left Shift\n");
  int shift = (reg2 & 0x1F);
  return(reg1 << shift);
}

//logical rigth shift
int srl(int reg1, int reg2){
  printf("Instruction:Logical Right Shift\n");
  int shift = (reg2 & 0x1F);
  return(reg1 >> shift);
}

//arithmetic roght shift
int sra(int reg1, int reg2){
  printf("Instruction:Logical Right Shift\n");
  int shift = (reg2 & 0x1F);
  return(reg1 >> shift);
}

//jump and link
int jal(){
  printf("Instruction:Jump and Link\n");
  int originalPC = pc[0] + 4;
  int jimm = (jimm1 + jimm2 + signedjimm1 + signedjimm2);
  if((jimm & 0x00080000)){
    jimm += 0xFFF00000;
  }
  pc[0] = pc[0] + (jimm);
  return originalPC;
}

//jump and link register
int jalr(){
  
}

//branch if equal
int beq(int reg1, int reg2){
  printf("Instruction:Branch if Equal\n");

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

//load memory into register by returning
uint32_t ld(char* cache, uint32_t offset){
  uint32_t* data;
  data = (uint32_t*)(cache + offset);
  return *data;
}

//store contents of register into memory
void st(uint32_t regVal, char* cache, uint32_t offset){
  uint32_t* data;
  data = (uint32_t*)(cache + offset);
  *data = regVal;
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
      break;
    case 0b0000011:
      instruction_type = 1;
      break;
    case 0b0110111:
      instruction_type = 2;
      break;
    case 0b0010111:
      instruction_type = 2;
      break;
    case 0b0110011:
      instruction_type = 3;
      break;
    case 0b1101111:
      instruction_type = 4;
      break;
    case 0b1100011:
      instruction_type = 5;
      break;
    case 0b0100011:
      instruction_type = 6;
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
        printf("Sign Extension\n");
        imm += 0xFFFFF000;
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
    jimm1 = (instruction & 0x000FF000) >> 12 << 12;
    signedjimm1 = (instruction & 0x001FF000) >> 20 << 11;
    jimm2 = (instruction & 0x7FE00000) >> 21 << 1;
    signedjimm2 = (instruction & 0x80000000) >> 31 << 20;
  }
  //B type instructions
  else if(instruction_type == 5){
    signedbimm1 = (instruction & 0x00000080) >> 7;
    bimm1 = (instruction & 0x00000F00) >> 8;
    funct3 = (instruction & 0x00007000) >> 12;
    rsource1 = (instruction & 0x000F8000) >> 15;
    rsource2 = (instruction & 0x01F00000) >> 20;
    bimm2 = (instruction & 0x7E000000) >> 25;
    signedbimm2 = (instruction & 0x10000000) >> 31;
  }
  //S type instructions
  else if(instruction_type == 6){
    simm1 = (instruction & 0x00000F80) >> 7;
    funct3 = (instruction & 0x00007000) >> 12;
    rsource1 = (instruction & 0x000F8000) >> 15;
    rsource2 = (instruction & 0x01F00000) >> 20;
    simm2 = (instruction & 0xFE000000) >> 25;
  }
}

// void decode(){
//   opcode = (instruction & 0xFC000000) >> 26;
//   switch(opcode){
//     case 0x0:
//       instruction_type = 1;
//       break;
//     case 0x02:
//       instruction_type = 3;
//       break;
//     case 0x03:
//       instruction_type = 3;
//       break;
//     case 0x04:
//       instruction_type = 2;
//       break;
//     case 0x05:
//       instruction_type = 2;
//       break;
//     case 0x06:
//       instruction_type = 2;
//       break;
//     case 0x08:
//       instruction_type = 2;
//       break;
//     case 0x09:
//       instruction_type = 2;
//       break;
//     case 0x0A:
//       instruction_type = 2;
//       break;
//     case 0x0B:
//       instruction_type = 2;
//       break;
//     case 0x0C:
//       instruction_type = 2;
//       break;
//     case 0x0D:
//       instruction_type = 2;
//       break;
//     case 0x0F:
//       instruction_type = 2;
//       break;
//     // case 0x10:
//     //   instruction_type = 1;
//     //   break;
//     case 0x23:
//       instruction_type = 2;
//       break;
//     case 0x24:
//       instruction_type = 2;
//       break;
//     case 0x25:
//       instruction_type = 2;
//       break;
//     case 0x28:
//       instruction_type = 2;
//       break;
//     case 0x29:
//       instruction_type = 2;
//       break;
//     case 0x2B:
//       instruction_type = 2;
//       break;
//     default:
//       printf("Error in decode stage\n");
//       printf("Opcode was:%d\n", opcode);
//       separator;
//       exit(1);
//   }
//
//   if(instruction_type == 1){
//     rsource = (instruction & 0x03E00000) >> 21;
//     rtarget = (instruction & 0x001F0000) >> 16;
//     rdestination = (instruction & 0x0000F800) >> 11;
//     shamt = (instruction & 0x000007C0) >> 6;
//     funct = (instruction & 0x0000003F);
//   }
//   else if(instruction_type == 2){
//     rsource = (instruction & 0x03E00000) >> 21;
//     rtarget = (instruction & 0x001F0000) >> 16;
//     imm = (instruction & 0x0000FFFF);
//   }
//   else if(instruction_type == 3){
//     printf("Not done yet\n");
//     exit(1);
//   }
//
//   switch(instruction_type){
//     case 0:
//       break;
//     case 1:
//       printf("Instruction format:R\n");
//       break;
//     case 2:
//       printf("Instruction format:I\n");
//       break;
//     case 3:
//       printf("Instruction format:J\n");
//       break;
//   }
//   printf("Opcode:%d\n", opcode);
// }

// execute_rformat(){
//   switch(funct){
//     case 0x18:
//       registers[rdestination] = mul(registers[rsource], registers[rtarget]);
//     case 0x1A:
//       registers[rdestination] = div(registers[rsource], registers[rtarget]);
//     case 0x20:
//       registers[rdestination] = add(registers[rsource], registers[rtarget]);
//     case 0x22:
//       registers[rdestination] = sub(registers[rsource], registers[rtarget]);
//   }
// }

//execute I format instructions
void execute_iformat(){
  switch(funct3){
    case(0b000):
      registers[rdestination] = addi(registers[rsource1]);
  }
}

void execute_sj(){
  return;
}

void execute(){
  printf("Instruction Type:%d\n", instruction_type);
  if(instruction_type == 5 || instruction_type == 6){
    execute_sj();
    return;
  }

  if(rdestination == 0){
    printf("Error, register destination cannot be written into");
    exit(1);
  }
  if(instruction_type == 1){
    execute_iformat();
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
    st(-1, Icache, i);
  }
}

void load_program(char* file){
  FILE* fp;
  fp = fopen(file, "r");
  unsigned int tinstruction, loadpc = 0;
  if(fp == NULL){
    printf("Error in loading program, file is null");
    exit(1);
  }
  while(fscanf(fp, "%x", &tinstruction) != EOF){
    st(tinstruction, Icache, loadpc);
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
  load_program(argv[1]);

  run();

  // printf("HELLO:%u\n", y);
  // unsigned short int x = 65529U;
  // short int y = *(short int*)&x;
  //
  // printf("%d\n", y);

  return 0;
}
