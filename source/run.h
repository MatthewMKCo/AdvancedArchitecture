#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "variables.h"

void fetch();
void decode();
void execute_iformat();
void execute_uformat();
void execute_rformat();
void execute_jformat();
void execute_bformat();
void execute_sformat();
void memory_sformat();
void memory_iformat();
void execute_sj();
void execute();

int addi(int reg1);
int andi(int reg1);
int ori(int reg1);
int xori(int reg1);
int slti(int reg1);
int sltiu(int reg1);
int slli(int reg1);
int srli(int reg1);
int srai(int reg1);
int lui();
int auipc();
int add(int reg1, int reg2);
int sub(int reg1, int reg2);
int slt(int reg1, int reg2);
int sltu(int reg1, int reg2);
int and(int reg1, int reg2);
int or(int reg1, int reg2);
int xor(int reg1, int reg2);
int mul(int reg1, int reg2);
int divide(int reg1, int reg2);
int sll(int reg1, int reg2);
int srl(int reg1, int reg2);
int sra(int reg1, int reg2);
int jal();
int jalr(int reg1);
void beq(int reg1, int reg2);
void bne(int reg1, int reg2);
void blt(int reg1, int reg2);
void bltu(int reg1, int reg2);
void bge(int reg1, int reg2);
void bgeu(int reg1, int reg2);
int ld(char* cache, int reg1);
int lw(char* cache, int reg1);
int lh(char* cache, int reg1);
int lhu(char* cache, int reg1);
int lb(char* cache, int reg1);
int lbu(char* cache, int reg1);
void st(char* cache, int reg1, int reg2);
void sw(char* cache, int reg1, int reg2);
void sh(char* cache, int reg1, int reg2);
void sb(char* cache, int reg1, int reg2);

void print_reg_summary();
void move_next_to_current();
void mem_acc();
