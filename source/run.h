#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "variables.h"


//fetch functions
void fetch();

//decode functions
void decode();

//issue functions
void issue();

//execute functions
//execute.c functions
void execute_iformat(int number);
void execute_uformat(int number);
void execute_rformat(int number);
void execute_jformat(int number);
void execute_bformat(int number);
void execute_sformat(int number);
void execute_sformat2(int number);
void execute_iformat2(int number);
// void execute_sj();
void execute();
int find_available_agu();
availNum find_available_alu();
availNum find_available_bru();
availNum find_available_lsu();
void send_for_writeback();
//operations.c functions
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
int beq(int reg1, int reg2);
int bne(int reg1, int reg2);
int blt(int reg1, int reg2);
int bltu(int reg1, int reg2);
int bge(int reg1, int reg2);
int bgeu(int reg1, int reg2);
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

//writeback functions
void writeback();

//graduate functions
void graduate();

//utility functions
void set_register();
void load_program();
void print_reg_summary();
void move_next_to_current();
void mem_acc();
void exit_early();
void forward_reservation_stations(int tagPassed, int value);
void are_instructions_ready();
void purgepipe();

//lists functions
ring *createring(char* name);
void addafternode(ring *currentRing, tag data);
void addafternodeinstruction(ring *currentRing, uint32_t instruction, int id, tag tagData, int unit_type, int value, char* instruction_name);
int movenode(ring *unused, ring *inuse, int registerNumber, int id, int selected);
tag get(ring *currentRing);
void next(ring *currentRing);
find find_tag(ring *currentRing, int tagNumber);
find find_register(ring *currentRing, int registerNumber);
find find_register_last(ring *currentRing, int registerNumber);
find check_first(ring *currentRing, ring *secondRing, int tagDestination);
void start(ring *currentRing);
tag getSelected(ring *currentRing);
void deletenode(ring *currentRing);
void printring(ring *currentRing);
void printringbackwards(ring *currentRing);
find second_last(ring *currentRing, int registerDestination);
int getinstructionid(ring *currentRing);
void change_selected_to_first(ring* currentRing);
find find_id(ring *currentRing, int id);
everything get_everything(ring *currentRing);
void change_to_ready(ring *currentRing);
void deletenodeswithgreaterthanid();
void addvaluetolast(ring* currentRing, int value);
int check_tag_for_zero(ring* currentRing, int tagPassed);
int get_register(ring* currentRing, int tag);
int get_register2(ring* currentRing);
int moveselectednode(ring *unused, ring *inuse, int purgeid);
int list_empty(ring* currentRing);

//Branch Functions
int branch_predictor(int fedPC, int is_branch, int offset);
int check_purge_accepted(int pc, int offset, int accepted);
int check_purge_rejected(int pc, int offset, int accepted);
int change_pc_execute(int pc, int offset, int accepted);
