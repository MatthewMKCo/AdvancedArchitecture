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

int fetch_branch[NWAY], decode_branch[NWAY];

int fetch_inuse[NWAY], decode_inuse[NWAY], decode_decoded[NWAY], issue_inuse[NWAY];

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
          if(NOTPRINT){
            printf("Architectural Registers\n");
            //Print all register values
            for(int i = 0; i < 31; i++){
              switch(i){
                case(0):
                  printf("zero:%-6d\t", registers[i]);
                  break;
                case(1):
                  printf("ra:%-6d\t", registers[i]);
                  break;
                case(2):
                  printf("sp:%-6d\t", registers[i]);
                  break;
                case(3):
                  printf("gp:%-6d\t", registers[i]);
                  break;
                case(4):
                  printf("tp:%-6d\t", registers[i]);
                  break;
                case(5):
                  printf("t0:%-6d\t", registers[i]);
                  break;
                case(6):
                  printf("t1:%-6d\t", registers[i]);
                  break;
                case(7):
                  printf("t2:%-6d\t", registers[i]);
                  break;
                case(8):
                  printf("s0:%-6d\t", registers[i]);
                  break;
                case(9):
                  printf("s1:%-6d\t", registers[i]);
                  break;
                case(10):
                  printf("a0:%-6d\t", registers[i]);
                  break;
                case(11):
                  printf("a1:%-6d\t", registers[i]);
                  break;
                case(12):
                  printf("a2:%-6d\t", registers[i]);
                  break;
                case(13):
                  printf("a3:%-6d\t", registers[i]);
                  break;
                case(14):
                  printf("a4:%-6d\t", registers[i]);
                  break;
                case(15):
                  printf("a5:%-6d\t", registers[i]);
                  break;
                case(16):
                  printf("a6:%-6d\t", registers[i]);
                  break;
                case(17):
                  printf("a7:%-6d\t", registers[i]);
                  break;
                case(18):
                  printf("s2:%-6d\t", registers[i]);
                  break;
                case(19):
                  printf("s3:%-6d\t", registers[i]);
                  break;
                case(20):
                  printf("s4:%-6d\t", registers[i]);
                  break;
                case(21):
                  printf("s5:%-6d\t", registers[i]);
                  break;
                case(22):
                  printf("s6:%-6d\t", registers[i]);
                  break;
                case(23):
                  printf("s7:%-6d\t", registers[i]);
                  break;
                case(24):
                  printf("s8:%-6d\t", registers[i]);
                  break;
                case(25):
                  printf("s9:%-6d\t", registers[i]);
                  break;
                case(26):
                  printf("s10:%-6d\t", registers[i]);
                  break;
                case(27):
                  printf("s11:%-6d\t", registers[i]);
                  break;
                case(28):
                  printf("t3:%-6d\t", registers[i]);
                  break;
                case(29):
                  printf("t4:%-6d\t", registers[i]);
                  break;
                case(30):
                  printf("t5:%-6d\t", registers[i]);
                  break;
                case(31):
                  printf("t6:%-d\t", registers[i]);
                  break;
              }
              if((i+1) % 5 == 0)printf("\n");
            }
            printf("pc:%-d\t", pc[0]);
            printf("\n");
            printf("Physical Registers\n");
            for(int i = 0; i < PHYSREG_NUM; i++){
              printf("Register %d: %-6d\t", i, physRegisters[i].value);
              if((i + 1) % 5 == 0)printf("\n");
            }
            printf("\n");
            for(int i = 0; i < ALU_NUM; i++){
              if(alu[i].ready == 1)printf("ALU %d:FREE\t", i);
              if(alu[i].ready == 0)printf("ALU %d:INUSE\t", i);
            }
            printf("\n");
            for(int i = 0; i < BRU_NUM; i++){
              if(bru[i].ready == 1)printf("BRU %d:FREE\t", i);
              if(bru[i].ready == 0)printf("BRU %d:INUSE\t", i);
            }
            printf("\n");
            for(int i = 0; i < LSU_NUM; i++){
              if(lsu[i].ready == 1)printf("LSU %d:FREE\t", i);
              if(lsu[i].ready == 0)printf("LSU %d:INUSE\t", i);
            }
            printf("\n");

          }
          printf("End of program\n");
          printf("Branch Predictor:");
          switch(BRANCH_PREDICTOR){
            case(0):
              printf("Branch Never Taken\n");
              break;
            case(1):
              printf("Branch Always Taken\n");
              break;
            case(2):
              printf("Backwards taken, forwards not taken\n");
              break;
            case(3):
              printf("2-bit Saturating Counter\n");
              break;
            case(4):
              printf("2-level Adaptive Predictor\n");
              break;
          }
          printf("Number of Cycles to complete:%d\n",current_cycle - 1);
          printf("Number of Instructions executed:%d\n",instructions_executed);
          printf("Number of instructions per cycle:%.2f\n",((float)instructions_executed / (float)(current_cycle - 1)));
          separator;
          break;
      }


    fetch();

    decode();

    issue();

    execute();

    // if(purge == 1){
    //   exit_early();
    //   block_fetch_to_decode = 0;
    //   block_decode_to_issue = 0;
    //   purge = 0;
    //   first_fetch = 0;
    //   first_decode = 0;
    //   first_issue = 0;
    //   issue_finished = 0;
    //   flush_from_issue = 0;
    //   stall_rename = 0;
    //   stall_from_issue = 0;
    //   current = 0;
    //   purgepipe();
    // }

    writeback();

    graduate();

    // print_reg_summary();



    if(rob < 0){
      printf("ERROR IN ROB");
      exit(1);
    }

    for(int i = 0; i < RESERVATION_WIDTH; i++){
      if(reservationalu[i].inuse == 0 && reservationalu[i].inExecute == 0){
        for(int j = i + 1; j < RESERVATION_WIDTH; j++){
          reservationalu[j - 1] = reservationalu[j];
          reservationalu[j].inuse = 0;
          reservationalu[j].inExecute = 0;
        }
      }
      else continue;
    }

    print_reg_summary();

    move_next_to_current();
    // printring(allInOrder);
    current_cycle++;
    separator;

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

  separator;
  printf("Loading Program\n");
  load_program(argv[1]);

  separator;

  run();

  // printring(allInOrder);

  return 0;
}
