#define SIZE 1024

#define XLEN 32

#define REG_NUM 32

#define PHYSREG_NUM 128

#define NUM_STAGES 4

#define ALU_NUM 4

#define AGU_NUM 1

#define BRU_NUM 1

#define LSU_NUM 1

#define TAG_NUM 128

#define separator printf("====================================================\n");

//ALU struct
typedef struct execute_unit{
  int cyclesNeeded;
  int currentCycles;
  int ready;
  int valueInside;
  int destinationRegister;
  int sourceRegister1;
  int sourceRegister2;
  int readyForWriteback;
}execute_unit;

typedef struct reg{
  int ready;
  int value;
}reg;
//
// typedef int item;
//
// typedef struct Node{
//   struct Node *forward;
//   struct Node *back;
//   item data;
// }node;
//
// typdef struct Ring{
//    node *current;
//    node *first
//    node *sentinel;
// }ring;

extern execute_unit alu[ALU_NUM];
extern execute_unit lsu[LSU_NUM];
extern execute_unit agu[AGU_NUM];
extern execute_unit bru[BRU_NUM];

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
extern int tag[TAG_NUM];

//Registers
extern int registers[32];
//Re-order buffer registers
extern int rob[32];

//Program Counter
extern int pc[1];
extern int decodepc, fetchpc, executepc;
extern int* sp;
//Source Registers
extern int decode_rsource1, execute_rsource1, mem_rsource1;
extern int decode_rsource2, execute_rsource2, mem_rsource2;
//Destination register
extern int decode_rdestination, execute_rdestination, mem_rdestination, writeback_rdestination;

//Current Instruction
extern uint32_t decode_instruction;
extern uint32_t fetch_instruction;
extern uint32_t executed_instruction;


//Current Instruction Type
//-1 = End, 1 = Immediate, 2 = Unsigned, 3 = Register, 4 = Jump, 5 = Branch, 6 = Store
extern int decode_instruction_type, execute_instruction_type;

int execute_load, execute_store, mem_load, mem_store;

extern char instruction_type_char;

//first x
extern int first_fetch, first_decode, first_execute, first_mem_access;

//last instruction
extern int last_instruction;
extern int last_instruction_cycle;

//Current current_opcode
extern int execute_opcode, decode_opcode;

//Current Funct3
extern int execute_funct3, decode_funct3, mem_funct3;

//Current Funct7
extern int execute_funct7, decode_funct7;

//Current Shift Amount
extern int execute_shamt, decode_shamt;

//Current Immediate Value
extern int execute_imm, decode_imm, next_imm;

extern int mem_acc_val, execute_val, writeback_val;

extern int mem_access, execute_access;

extern int execute_offset, mem_offset;

extern char* executed_instruction_name;

extern int print_decode_summary, print_execute_summary;
