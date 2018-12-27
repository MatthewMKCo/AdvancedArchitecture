#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#define SIZE 1024*32

#define XLEN 32

#define REG_NUM 32

#define PHYSREG_NUM 64

#define RESERVATION_WIDTH 16

#define BRANCH_RESERVATION_WIDTH 8

#define STORE_RESERVATION_WIDTH 8

#define ROBSIZE (RESERVATION_WIDTH + BRANCH_RESERVATION_WIDTH + STORE_RESERVATION_WIDTH)

//0 - Static, never takes
//1 - Static, always takes
//2 - Backwards taken, forwards not taken
#define BRANCH_PREDICTOR 2

#define NUM_STAGES 10

#define NWAY 1
#define ALU_NUM 4

#define BRU_NUM 1

#define LSU_NUM 1

#define JLU_NUM 1

#define TAG_NUM PHYSREG_NUM

#define sizeOfBranchCache 100

#define NOTPRINT 0


#define separator printf("====================================================\n");

typedef struct acceptOrReject{
  int accept;
  int purge;
}acceptOrReject;

typedef struct branchDict{
  int pc;
  int branchpc;
  int accepted_before;
  int inuse;
}branchDict;

typedef struct instruction{
  int pcDestination;
  int pctag;
  int rdestination;
  int tagDestination;
  int rsource1;
  int rsource2;
  int tagsource1;
  int tagsource2;
  int rsource1value;
  int rsource2value;
  int opcode;
  int funct3;
  int funct7;
  int shamt;
  int imm;
  int pc;
  int instruction_type;
  uint32_t instruction_hex;
  int instructionid;
  int index;
}instruction;

//ALU struct
typedef struct execute_unit{
  int cyclesNeeded;
  int currentCycles;
  int ready;
  int valueInside;
  int destinationRegister;
  int sourceRegister1;
  int sourceRegister2;
  int wbValueInside;
  int wbDestinationRegister;
  int wbSourceRegister1;
  int wbSourceRegister2;
  int readyForWriteback;
  int shouldWriteback;
  instruction instruction;
  char* instruction_name;
}execute_unit;

typedef struct reg{
  int ready;
  int value;
}reg;

typedef struct tag{
  int registerNumber;
  int tagNumber;
}tag;

typedef struct reserve{
  int rdestination;
  int rsource1;
  int rsource1ready;
  int rsource1value;
  int rsource2;
  int rsource2ready;
  int rsource2value;
  int opcode;
  int funct3;
  int funct7;
  int shamt;
  int imm;
  int pc;
  int inuse;
  int instruction_type;
  int inExecute;
  int notExecuted;
  int instruction_hex;
  int instructionid;
  int value;
  instruction instruction;
}reserve;

typedef struct instructionwrapper{
  instruction instruction[ALU_NUM];
  int foundInstructions;
  int index;
}instructionwrapper;

typedef struct availNum{
  int number;
  int unitNumber[ALU_NUM];
}availNum;

typedef struct execute_to_writeback{
  int ready;
  int value;
  int tag;
  int instruction;
  int instructionid;
  int instruction_type;
  char* instruction_name;
}execute_to_writeback;

// typedef int item;

typedef struct Node{
  struct Node *forward;
  struct Node *back;
  tag data;
  uint32_t instruction;
  int id;
  char *name;
  int ready;
  int value;
  int unit_type;
  char* instruction_name;
}node;

typedef struct everything{
  tag tagData;
  uint32_t instruction;
  int id;
  int ready;
  int unit_type;
  int value;
}everything;

typedef struct Ring{
   node *last;
   node *first;
   node *sentinel;
   node *selected;
   char *name;
}ring;

typedef struct Find{
  int found;
  int number;
}find;

extern execute_unit alu[ALU_NUM];
extern execute_unit lsu[LSU_NUM];
extern execute_unit bru[BRU_NUM];
extern execute_unit jlu[JLU_NUM];


extern instruction currentInstruction;
//
// extern struct ALU *alu;

//Current Cycle
extern int current_cycle;

//Instructions executed
extern int instructions_executed;

//Branched
extern int branch_flag;

//Jumped
extern int jump_flag;

//Instruction cache
extern char Icache[SIZE];
//Data cache
extern char Dcache[SIZE];
//Data Memory
extern char Dmem[SIZE];
//Tag array
extern tag tags[TAG_NUM];
extern int tagIterator;

extern reserve reservationalu[RESERVATION_WIDTH];
extern reserve reservationlsu[RESERVATION_WIDTH];
extern reserve reservationbru[BRANCH_RESERVATION_WIDTH];

extern int reservationIteratorALU;

//Registers
extern int registers[REG_NUM+1];
//Re-order buffer registers
extern reg physRegisters[PHYSREG_NUM];

//Program Counter
extern int* pc;
extern int decodepc[NWAY], issuepc, fetchpc[NWAY], executepc;
extern int* sp;
//Source Registers
extern int decode_rsource1, issue_rsource1, execute_rsource1, mem_rsource1;
extern int decode_rsource2, issue_rsource2, execute_rsource2, mem_rsource2;
//Destination register
extern int decode_rdestination, issue_rdestination, execute_rdestination, mem_rdestination, writeback_rdestination;
//Execute unit type, 1 = ALU, 2 = LSU, 3 = BRU
extern int decode_unit_type[NWAY], issue_unit_type[NWAY];

extern ring* unusedTags; extern ring* inuseTags; extern ring* outOfOrderInstructions; extern ring* inOrderInstructions;
extern ring* allInOrder;

//Current Instruction
extern uint32_t decode_instruction[NWAY];
extern uint32_t fetch_instruction[NWAY];
extern uint32_t executed_instruction;
extern uint32_t issue_instruction;


//Current Instruction Type
//-1 = End, 1 = Immediate, 2 = Unsigned, 3 = Register, 4 = Jump, 5 = Branch, 6 = Store
extern int decode_instruction_type, issue_instruction_type, execute_instruction_type;

int execute_load, execute_store, mem_load, mem_store;

extern char instruction_type_char;

//first x
extern int first_fetch, first_issue ,first_decode, first_execute, first_writeback;

//last instruction
extern int last_instruction;
extern int last_instruction_cycle;

//Current current_opcode
extern int execute_opcode, issue_opcode, decode_opcode;

//Current Funct3
extern int execute_funct3, issue_funct3, decode_funct3, mem_funct3;

//Current Funct7
extern int execute_funct7, issue_funct7, decode_funct7;

//Current Shift Amount
extern int execute_shamt, issue_shamt, decode_shamt;

//Current Immediate Value
extern int execute_imm, issue_imm, decode_imm, next_imm;

extern int mem_acc_val, execute_val, writeback_val;

extern int mem_access, execute_access;

extern int execute_offset, mem_offset;

extern char* executed_instruction_name[ALU_NUM + LSU_NUM + BRU_NUM];

extern int print_decode_summary, print_execute_summary, print_issue_summary;

extern int writeback_destination, graduate_destination;

extern execute_to_writeback writebackjlu[JLU_NUM], writebackalu[ALU_NUM], writebackbru[BRU_NUM], writebacklsu[LSU_NUM];

extern int stall_from_issue;

extern instruction decode_instruction_struct[NWAY], issue_instruction_struct[NWAY];

extern int stop;

extern int decode_pcDestination;

extern int fetch_finished, decode_finished, issue_finished, execute_finished, writeback_finished, graduate_finished;

extern int execute_cycle_finished;

extern int instructionid;

extern int flush_from_issue, stall_rename;

extern int purgeid, purge;

extern int continue_execute;

extern int numberOfExecutedInstructions;

extern int block_decode_to_issue, block_fetch_to_decode;

extern branchDict branchCache[sizeOfBranchCache];

extern int rob;
