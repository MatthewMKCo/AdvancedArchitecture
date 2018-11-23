#include "run.h"

//add register and immediate
int addi(int reg1){
  return reg1 + execute_imm;
}

//subtract register and immediate
// int subi(uint32_t reg1, uint32_t immOperand){
//   return reg1 - immOperand;
// }

//bit wise and on register and immediate
int andi(int reg1){
  return (reg1 & execute_imm);
}

//bit wise or on register and immediate
int ori(int reg1){
  return (reg1 | execute_imm);
}

//bit wise xor on register and immediate
int xori(int reg1){
  return (reg1 ^ execute_imm);
}

//set less than immediate
int slti(int reg1){
  if(reg1 < execute_imm)return 1;
  else return 0;
}

//set less than immediate unsigned
int sltiu(int reg1){
  unsigned int reg1u = reg1;
  unsigned int immu = execute_imm;
  if(reg1u < immu)return 1;
  else return 0;
}

//Logical Left Shift Immediate
int slli(int reg1){
  unsigned int reg1u = reg1;
  reg1u = (reg1u << execute_shamt);
  reg1 = (int)reg1u;
  return (reg1 & 0xFFFFFFFF);
}

//Logical Right Shift Immediate
int srli(int reg1){
  unsigned int reg1u = reg1;
  reg1u = (reg1u >> execute_shamt);
  reg1 = (int)reg1u;
  return (reg1 & 0xFFFFFFFF);
}

//Arithmetic Right Shift Immediate
int srai(int reg1){
  return ((reg1 >> execute_shamt) & 0xFFFFFFFF);
}

//Load Upper Immediate
int lui(){
  return (execute_imm & 0xFFFFF000);
}

//Add Upper Immediate to PC
int auipc(){
  return((execute_imm & 0xFFFFF000) + pc[0]);
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
  int originalPC = executepc + 4;
  pc[0] = executepc + execute_imm;
  printf("%d\n",execute_imm);
  jump_flag = 1;
  return originalPC;
}

//jump and link register
int jalr(int reg1){
  int originalPC = executepc + 4;
  pc[0] = (execute_imm + reg1) & 0b0;
  jump_flag = 1;
  return originalPC;
}

//branch if equal
void beq(int reg1, int reg2){
  if(reg1 == reg2){
    // printf("offset:%d\n",execute_imm);
    // printf("actual pc:%d\n", executepc);
    // printf("actual pc:%d\n", currentpc);
    printf("%d\n",execute_imm);
    pc[0] = executepc + execute_imm;
    branch_flag = 1;
  }
  return;
}

//branch if not equal
void bne(int reg1, int reg2){
  if(reg1 != reg2){
    pc[0] = executepc + execute_imm;
    branch_flag = 1;
  }
  return;
}

//branch if less than
void blt(int reg1, int reg2){
  // printf("please\n");
  // printf("%d\t%d\n",reg1,reg2);
  if(reg1 < reg2){
    pc[0] = executepc + execute_imm;
    branch_flag = 1;
  }
  return;
}

//branch if less than
void bltu(int reg1, int reg2){
  unsigned int reg1u = reg1, reg2u = reg2;
  if(reg1u < reg2u){
    pc[0] = executepc + execute_imm;
    branch_flag = 1;
  }
  return;
}

//branch if greater than
void bge(int reg1, int reg2){
  if(reg1 >= reg2){
    pc[0] = executepc + execute_imm;
    branch_flag = 1;
  }
  return;
}

void bgeu(int reg1, int reg2){
  unsigned int reg1u = reg1, reg2u = reg2;
  if(reg1u >= reg2u){
    pc[0] = executepc + execute_imm;
    branch_flag = 1;
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
int lw(char* cache, int offset){
  int* data;
  data = (int*)(cache + offset);
  int data2 = ((*data) & 0xFFFFFFFF);
  return data2;
}

//load 16-bit memory into register
int lh(char* cache, int offset){
  int* data;
  data = (int*)(cache + offset);
  int data2 = ((*data) & 0x0000FFFF);
  if(data2 & 0x00008000){
    data2 = (data2 | 0xFFFF0000);
  }
  return data2;
}

//load unsigned 16-bit memory into register
int lhu(char* cache, int offset){
  int* data;
  data = (int*)(cache + offset);
  int data2 = ((*data) & 0x0000FFFF);
  return data2;
}

//load 8-bit memory into register
int lb(char* cache, int offset){
  int* data;
  data = (int*)(cache + offset);
  int data2 = ((*data) & 0x000000FF);
  if(data2 & 0x00000080){
    data2 = (data2 | 0xFFFFFF00);
  }
  return data2;
}

//load unsigned 8-bit memory into register
int lbu(char* cache, int offset){
  int* data;
  data = (int*)(cache + offset);
  int data2 = ((*data) & 0x000000FF);
  return data2;
}

//store contents of register into memory
void st(char* cache, int offset, int reg2){
  int* data;
  data = (int*)(cache + offset);
  *data = reg2;
}

//store 32-bit value from low bits of register into memory
void sw(char* cache, int offset, int reg2){
  int* data;
  data = (int*)(cache + offset);
  *data = reg2;
}

//store 16-bit value from low bits of register into memory
void sh(char* cache, int offset, int reg2){
  int* data;
  data = (int*)(cache + offset);
  int reg16bit = (reg2 & 0x0000FFFF);
  *data = reg16bit;
}

//store 8-bit value from low bits of register into memory
void sb(char* cache, int offset, int reg2){
  int* data;
  data = (int*)(cache + offset);
  int reg8bit = (reg2 & 0x000000FF);
  *data = reg8bit;
}