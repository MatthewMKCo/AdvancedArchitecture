#include <stdio.h>
#include "run.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define SIZE 1024

#define XLEN 32

#define NUM_STAGES 3

#define separator printf("====================================================\n");

int input;
int stop_while_cycle;
int next_cycle_step = 0;

//Current Cycle
int current_cycle = 1;

//Instructions executed
int instructions_executed = 0;

//Branched
int branch_flag = 0;

//Jumped
int jump_flag = 0;

//Instruction cache
char Icache[SIZE];
//Data cache
char Dcache[SIZE];

//Registers
int registers[32];
//Program Counter
int pc[1] = {0};
int decodepc, fetchpc, executepc;
int* sp = &registers[2];
//Source Registers
int decode_rsource1, execute_rsource1;
int decode_rsource2, execute_rsource2;
//Destination register
int decode_rdestination, execute_rdestination, mem_rdestination, writeback_rdestination;

//Current Instruction
uint32_t decode_instruction;
uint32_t fetch_instruction;
uint32_t executed_instruction;


//Current Instruction Type
//-1 = End, 1 = Immediate, 2 = Unsigned, 3 = Register, 4 = Jump, 5 = Branch, 6 = Store
int decode_instruction_type, execute_instruction_type;
char instruction_type_char;

//first x
int first_fetch = 0, first_decode = 0, first_execute = 0, first_mem_access = 0;

//last instruction
int last_instruction = 0;
int last_instruction_cycle;

//Current current_opcode
int execute_opcode, decode_opcode;

//Current Funct3
int execute_funct3, decode_funct3;

//Current Funct7
int execute_funct7, decode_funct7;

//Current Shift Amount
int execute_shamt, decode_shamt;

//Current Immediate Value
int execute_imm, decode_imm, next_imm;

int mem_acc_val, execute_val, writeback_val;

int mem_access, execute_access;


void pipeline_flush(){
  first_fetch = 0;
  first_decode = 0;
  first_execute = 0;
  branch_flag = 0;
  jump_flag = 0;
}

//runs the simulation
void run(){
  while(1){
    separator;
    printf("Cycle:%d\n", current_cycle);

    if(last_instruction == 1){
      // printf("LASTINSTRUCTION:%d\n",last_instruction_cycle);
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

    if(execute_rdestination != 0 && first_execute == 1){
      registers[execute_rdestination] = execute_val;
      // printf("desitnation:%d\n",execute_rdestination);
      // printf("total:%d\n", execute_val);
    }

    print_reg_summary();

    if(branch_flag == 1){
      pipeline_flush();
    }

    if(jump_flag == 1){
      pipeline_flush();
      // exit(1);
    }

    move_next_to_current();

    current_cycle++;
    separator;

    // if(current_cycle == 75) exit(1);
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
