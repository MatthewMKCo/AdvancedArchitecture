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

//Instruction cache
extern char Icache[SIZE];
//Data cache
extern char Dcache[SIZE];

//Registers
int registers[32];
//Program Counter
extern int pc[1];
extern int currentpc, nextpc, executepc;
extern int* sp;
//Source Registers
extern int next_rsource1, current_rsource1;
extern int next_rsource2, current_rsource2;
//Destination register
extern int next_rdestination, current_rdestination;

//Current Instruction
extern uint32_t fetch_current_instruction;
extern uint32_t fetch_next_instruction;
extern uint32_t executed_instruction;


//Current Instruction Type
//-1 = End, 1 = Immediate, 2 = Unsigned, 3 = Register, 4 = Jump, 5 = Branch, 6 = Store
extern int next_instruction_type, current_instruction_type;
extern char instruction_type_char;

//first x
extern int first_fetch, first_decode, first_execute;

//last instruction
extern int last_instruction;
extern int last_instruction_cycle;

//Current current_opcode
extern int current_opcode, next_opcode;

//Current Funct3
extern int current_funct3, next_funct3;

//Current Funct7
extern int current_funct7, next_funct7;

//Current Shift Amount
extern int current_shamt, next_shamt;

//Current Immediate Value
extern int current_imm, next_imm;

extern int current_val, next_val;
