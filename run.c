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

//add two registers
int add(int reg1, int reg2){
  return reg1 + reg2;
}

//add register and immediate
int addi(int reg1){
  return reg1 + imm;
}

//set less than immediate
// int slti(){
//   // if(signedfunc(registers))
// }

//subtract two registers
int sub(uint32_t reg1, uint32_t reg2){
  return reg1 - reg2;
}

//subtract register and immediate
int subi(uint32_t reg1, uint32_t immOperand){
  return reg1 - immOperand;
}

//multiply two registers
int mul(uint32_t reg1, uint32_t reg2){
  return reg1 * reg2;
}

//multiply register and immediate
int muli(uint32_t reg1, uint32_t immOperand){
  return reg1 * immOperand;
}

//divide two registers
int divide(uint32_t reg1, uint32_t reg2){
  return reg1 * reg2;
}

//divide register and immediate
int dividei(uint32_t reg1, uint32_t immOperand){
  return reg1 * immOperand;
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
    case 0b0000011:
      instruction_type = 1;
    case 0b0110111:
      instruction_type = 2;
    case 0b0010111:
      instruction_type = 2;
    case 0b0110011:
      instruction_type = 3;
    case 0b1101111:
      instruction_type = 4;
    case 0b1100011:
      instruction_type = 5;
    case 0b0100011:
      instruction_type = 6;
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
    jimm1 = (instruction & 0x000FF000) >> 12;
    signedjimm1 = (instruction & 0x001FF000) >> 20;
    jimm2 = (instruction & 0x7FE00000) >> 21;
    signedjimm2 = (instruction & 0x80000000) >> 31;
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

// void execute(){
//   if(instruction_type == 1){
//     execute_rformat();
//   }
// }

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

    // execute();

    pc[0] = pc[0] + 4;
    cycle++;
    separator;
}
  imm = (instruction & 0xFFFFF000);
  printf("%x\n", imm);
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

  return 0;
}
