#define SIZE 1024

#define XLEN 32

#define NUM_STAGES 3

#define separator printf("====================================================\n");

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

//Registers
int registers[32];
//Program Counter
extern int pc[1];
extern int decodepc, fetchpc, executepc;
extern int* sp;
//Source Registers
extern int decode_rsource1, execute_rsource1;
extern int decode_rsource2, execute_rsource2;
//Destination register
extern int decode_rdestination, execute_rdestination, mem_rdestination, writeback_rdestination;

//Current Instruction
extern uint32_t decode_instruction;
extern uint32_t fetch_instruction;
extern uint32_t executed_instruction;


//Current Instruction Type
//-1 = End, 1 = Immediate, 2 = Unsigned, 3 = Register, 4 = Jump, 5 = Branch, 6 = Store
extern int decode_instruction_type, execute_instruction_type;
extern char instruction_type_char;

//first x
extern int first_fetch, first_decode, first_execute, first_mem_access;

//last instruction
extern int last_instruction;
extern int last_instruction_cycle;

//Current current_opcode
extern int execute_opcode, decode_opcode;

//Current Funct3
extern int execute_funct3, decode_funct3;

//Current Funct7
extern int execute_funct7, decode_funct7;

//Current Shift Amount
extern int execute_shamt, decode_shamt;

//Current Immediate Value
extern int execute_imm, decode_imm;

extern int mem_acc_val, execute_val, writeback_val;

extern int mem_access, execute_access;
