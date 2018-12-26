#include <stdio.h>
#include "run.h"
#include <stdint.h>
#include <stdlib.h>

//execute units
execute_unit alu[ALU_NUM];
execute_unit lsu[LSU_NUM];
execute_unit bru[BRU_NUM];
execute_unit jlu[JLU_NUM];

//reservation station
reserve reservationalu[RESERVATION_WIDTH];
reserve reservationlsu[RESERVATION_WIDTH];
reserve reservationbru[BRANCH_RESERVATION_WIDTH];

execute_to_writeback writebackalu[ALU_NUM];
execute_to_writeback writebackbru[BRU_NUM];
execute_to_writeback writebacklsu[LSU_NUM];
execute_to_writeback writebackjlu[JLU_NUM];

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
//Data memory
char Dmem[SIZE];

//Registers
int registers[REG_NUM + 1];
//Physical registers
reg physRegisters[PHYSREG_NUM];
//Tag array
tag tags[TAG_NUM];

//Program Counter
int* pc = &registers[REG_NUM];
int decodepc[NWAY], issuepc, fetchpc[NWAY], executepc;
int* sp = &registers[2];
//Source Registers
int decode_rsource1, issue_rsource1, execute_rsource1, mem_rsource1;
int decode_rsource2, issue_rsource2, execute_rsource2, mem_rsource2;
//Destination register
int decode_rdestination, issue_rdestination, execute_rdestination, mem_rdestination, writeback_rdestination;
//Execute unit type, 1 = ALU, 2 = LSU, 3 = BRU
int decode_unit_type[NWAY], issue_unit_type[NWAY];

int flush_from_issue = 0;

//Current Instruction
uint32_t decode_instruction[NWAY];
uint32_t fetch_instruction[NWAY];
uint32_t executed_instruction;
uint32_t issue_instruction;


//Current Instruction Type
//-1 = End, 1 = Immediate, 2 = Unsigned, 3 = Register, 4 = Jump, 5 = Branch, 6 = Store
int decode_instruction_type, issue_instruction_type, execute_instruction_type;

int execute_load, execute_store, mem_load, mem_store;

char instruction_type_char;

//first x
int first_fetch = 0, first_issue = 0, first_decode = 0, first_execute = 0, first_writeback = 0;

//last instruction
int last_instruction = 0;
int last_instruction_cycle;

//Current current_opcode
int execute_opcode, issue_opcode, decode_opcode;

//Current Funct3
int execute_funct3, issue_funct3, decode_funct3, mem_funct3;

//Current Funct7
int execute_funct7, issue_funct7, decode_funct7;

//Current Shift Amount
int execute_shamt, issue_shamt, decode_shamt;

//Current Immediate Value
int execute_imm, issue_imm, decode_imm, next_imm;

int mem_acc_val, execute_val, writeback_val;

int mem_access, execute_access;

int execute_offset, mem_offset;

char* executed_instruction_name[ALU_NUM + LSU_NUM + BRU_NUM];

int print_decode_summary = 0, print_execute_summary = 0, print_issue_summary = 0;

int writeback_destination, graduate_destination;

ring* unusedTags; ring* inuseTags; ring* outOfOrderInstructions; ring* inOrderInstructions;
ring* allInOrder;

instruction decode_instruction_struct[NWAY], issue_instruction_struct[NWAY];

int fetch_finished = 0, decode_finished, issue_finished = 0, execute_finished = 0, writeback_finished = 0, graduate_finished = 0;

int execute_cycle_finished;

int purgeid = 0, purge = 0;

int continue_execute = 0;

void pipeline_flush(){
  first_fetch = 0;
  first_decode = 0;
  first_issue = 0;
  first_execute = 0;
  first_writeback = 0;
  branch_flag = 0;
  jump_flag = 0;
  mem_access = 0;
}

//runs the simulation
void run(){
  while(1){
    separator;

      // printf("LASTINSTRUCTION:%d\n",last_instruction_cycle);
      if(pc[0] == 0 && graduate_finished == 1){
          printf("End of program\n");
          printf("Number of Cycles to complete:%d\n",current_cycle - 1);
          printf("Number of Instructions executed:%d\n",instructions_executed);
          printf("Number of instructions per cycle:%.2f\n",((float)instructions_executed / (float)(current_cycle - 1)));
          separator;
          // printring(inOrderInstructions);
          break;
      }


    fetch();

    // if(instruction == -1){
    //   printf("End at address:%d\n",pc[0]);
    //   separator;
    //   break;
    // }

    decode();

    issue();

    execute();

    // if(execute_rdestination != 0 && first_execute == 1){
    //   registers[execute_rdestination] = execute_val;
    //   // printf("desitnation:%d\n",execute_rdestination);
    //   // printf("total:%d\n", execute_val);
    // }
    writeback();

    graduate();

    print_reg_summary();


    if(purge == 1){
      block_fetch_to_decode = 0;
      block_decode_to_issue = 0;
      purge = 0;
      first_fetch = 0;
      first_decode = 0;
      first_issue = 0;
      issue_finished = 0;
      flush_from_issue = 0;
      stall_rename = 0;
      stall_from_issue = 0;
      purgepipe();
    }
    // if(branch_flag == 1){
    //   pipeline_flush();
    // }
    //
    // if(jump_flag == 1){
    //   pipeline_flush();
    //   // exit(1);
    // }

    if(flush_from_issue == 1){
      flush_from_issue = 0;
      first_fetch = 0;
      first_decode = 0;
      for(int i = 0; i < NWAY; i++){
        decode_instruction_struct[i].instruction_type = 0;
      }
    }



    move_next_to_current();
    current_cycle++;
    separator;

    // if(current_cycle == 160) exit(1);
}
}

int main(int argc, char** argv){

  if(argc < 2){
    printf("Incorrect number of arguments");
    exit(1);
  }



  set_register();
  inuseTags = createring("inuseTags");
  unusedTags = createring("unusedTags");
  outOfOrderInstructions = createring("outOfOrderInstructions");
  inOrderInstructions = createring("inOrderInstructions");
  allInOrder = createring("allInOrder");

  for(int i = 0; i < PHYSREG_NUM; i++){
    tag tagPlaceholder;
    tagPlaceholder.tagNumber = i;
    tagPlaceholder.registerNumber = -1;
    addafternode(unusedTags, tagPlaceholder);
  }

  // printf("Tag:%d\n", x);
  separator;
  printf("Loading Program\n");
  load_program(argv[1]);

  separator;

  run();
  // printring(allInOrder);

  return 0;
}
