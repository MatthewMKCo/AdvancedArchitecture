#define SIZE 1024

#define XLEN 32

#define REG_NUM 32

#define PHYSREG_NUM 128

#define RESERVATION_WIDTH 64

#define BRANCH_RESERVATION_WIDTH 16

#define NUM_STAGES 10

#define ALU_NUM 4

#define AGU_NUM 1

#define BRU_NUM 1

#define LSU_NUM 1

#define TAG_NUM 128

#define separator printf("====================================================\n");

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
  int instruction_hex;
  int instructionid;
  instruction instruction;
}reserve;

typedef struct instructionwrapper{
  instruction instruction[ALU_NUM];
  int foundInstructions;
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
}node;

typedef struct everything{
  tag tagData;
  uint32_t instruction;
  int id;
  int ready;
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
extern execute_unit agu[AGU_NUM];
extern execute_unit bru[BRU_NUM];

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
extern int decodepc, issuepc, fetchpc, executepc;
extern int* sp;
//Source Registers
extern int decode_rsource1, issue_rsource1, execute_rsource1, mem_rsource1;
extern int decode_rsource2, issue_rsource2, execute_rsource2, mem_rsource2;
//Destination register
extern int decode_rdestination, issue_rdestination, execute_rdestination, mem_rdestination, writeback_rdestination;
//Execute unit type, 1 = ALU, 2 = LSU, 3 = BRU
extern int decode_unit_type, issue_unit_type;

extern ring* unusedTags; extern ring* inuseTags; extern ring* outOfOrderInstructions; extern ring* inOrderInstructions;


//Current Instruction
extern uint32_t decode_instruction;
extern uint32_t fetch_instruction;
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

extern char* executed_instruction_name;

extern int print_decode_summary, print_execute_summary, print_issue_summary;

extern int writeback_destination, graduate_destination;

extern execute_to_writeback writebackalu[ALU_NUM], writebackbru[BRU_NUM];

extern int stall_from_issue;

extern instruction decode_instruction_struct, issue_instruction_struct;

extern int stop;

extern int decode_pcDestination;

extern int fetch_finished, decode_finished, issue_finished, execute_finished, writeback_finished, graduate_finished;

extern int execute_cycle_finished;

extern int instructionid;

extern int flush_from_issue;

extern int purgeid, purge;
