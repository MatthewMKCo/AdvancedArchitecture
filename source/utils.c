#include "run.h"

//sets registers to 0
void set_register(){
  for(int i = 0; i < REG_NUM; i++){
    registers[i] = 0;
  }
  for(int i = 0; i < PHYSREG_NUM; i++){
    physRegisters[i].ready = 1;
    physRegisters[i].value = 0;
  }
  for(int i = 0; i < SIZE; i++){
    st(Icache, i, -1);
  }
  sp[0] = SIZE;
  pc[0] = 4;

  for(int i = 0; i < ALU_NUM; i++){
    alu[i].cyclesNeeded = 0;
    alu[i].currentCycles = 0;
    alu[i].ready = 1;
  }
  for(int i = 0; i < LSU_NUM; i++){
    lsu[i].cyclesNeeded = 0;
    lsu[i].currentCycles = 0;
    lsu[i].ready = 1;
  }
  for(int i = 0; i < AGU_NUM; i++){
    agu[i].cyclesNeeded = 0;
    agu[i].currentCycles = 0;
    agu[i].ready = 1;
  }
  for(int i = 0; i < BRU_NUM; i++){
    bru[i].cyclesNeeded = 0;
    bru[i].currentCycles = 0;
    bru[i].ready = 1;
  }

  for(int i = 0; i < RESERVATION_WIDTH; i++){
    reservationalu[i].rsource1 = -1;
    reservationalu[i].rsource2 = -1;
    reservationalu[i].rsource1ready = 0;
    reservationalu[i].rsource2ready = 0;
  }

}

void load_program(char* file){
  FILE* fp;
  fp = fopen(file, "r");
  unsigned int tinstruction, loadpc = 4;
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

void print_reg_summary(){
  printf("Cycle:%d\n", current_cycle);
  if(last_instruction != 1){
    printf("Current PC in Fetch:%d\n",fetchpc);
  }
  if(print_decode_summary){
    printf("Decode Instruction:%x\n",decode_instruction);
    // printf("Decode Opcode:%d\n",decode_opcode);
  }
  if(print_execute_summary){
    printf("Executed Instruction Type:%c\n", instruction_type_char);
    printf("Instruction Executed:0x%.8X\n", executed_instruction);
    printf("Instruction Executed:%s\n", executed_instruction_name);
  }

  //Print all register values
  for(int i = 0; i < 31; i++){
    switch(i){
      case(0):
        printf("zero:%d\t", registers[i]);
        break;
      case(1):
        printf("ra:%d\t", registers[i]);
        break;
      case(2):
        printf("sp:%d\t", registers[i]);
        break;
      case(3):
        printf("gp:%d\t", registers[i]);
        break;
      case(4):
        printf("tp:%d\t", registers[i]);
        break;
      case(5):
        printf("t0:%d\t", registers[i]);
        break;
      case(6):
        printf("t1:%d\t", registers[i]);
        break;
      case(7):
        printf("t2:%d\t", registers[i]);
        break;
      case(8):
        printf("s0:%d\t", registers[i]);
        break;
      case(9):
        printf("s1:%d\t", registers[i]);
        break;
      case(10):
        printf("a0:%d\t", registers[i]);
        break;
      case(11):
        printf("a1:%d\t", registers[i]);
        break;
      case(12):
        printf("a2:%d\t", registers[i]);
        break;
      case(13):
        printf("a3:%d\t", registers[i]);
        break;
      case(14):
        printf("a4:%d\t", registers[i]);
        break;
      case(15):
        printf("a5:%d\t", registers[i]);
        break;
      case(16):
        printf("a6:%d\t", registers[i]);
        break;
      case(17):
        printf("a7:%d\t", registers[i]);
        break;
      case(18):
        printf("s2:%d\t", registers[i]);
        break;
      case(19):
        printf("s3:%d\t", registers[i]);
        break;
      case(20):
        printf("s4:%d\t", registers[i]);
        break;
      case(21):
        printf("s5:%d\t", registers[i]);
        break;
      case(22):
        printf("s6:%d\t", registers[i]);
        break;
      case(23):
        printf("s7:%d\t", registers[i]);
        break;
      case(24):
        printf("s8:%d\t", registers[i]);
        break;
      case(25):
        printf("s9:%d\t", registers[i]);
        break;
      case(26):
        printf("s10:%d\t", registers[i]);
        break;
      case(27):
        printf("s11:%d\t", registers[i]);
        break;
      case(28):
        printf("t3:%d\t", registers[i]);
        break;
      case(29):
        printf("t4:%d\t", registers[i]);
        break;
      case(30):
        printf("t5:%d\t", registers[i]);
        break;
      case(31):
        printf("t6:%d\t", registers[i]);
        break;
    }
    if((i+1) % 5 == 0)printf("\n");
  }
  printf("pc:%d\t", pc[0]);
  printf("\n");
  for(int i = 0; i < PHYSREG_NUM; i++){
    printf("Register %d; %d\t", i, physRegisters[i].value);
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
  // printf("%d\n", fetch_finished);
  // printf("%d\n", decode_finished);
  // printf("%d\n", issue_finished);
  // printf("%d\n", execute_finished);
  // printf("%d\n", writeback_finished);
  // printf("%d\n", graduate_finished);

  // for(int i = 0; i < BRANCH_RESERVATION_WIDTH; i++){
  //   if(reservationbru[i].instructionid == 21){
  //     printf("%d\n",i);
  //     exit_early();
  //   }
  // }
  // for(int i = 0; i < RESERVATION_WIDTH; i++){
  //   if(reservationalu[i].instructionid == 21){
  //     printf("%d\n",i);
  //     exit_early();
  //   }
  // }


}

void move_next_to_current(){
  // mem_rdestination = execute_rdestination;
  // mem_rsource1 = execute_rsource1;
  // mem_rsource2 = execute_rsource2;
  // mem_funct3 = execute_funct3;
  // mem_offset = execute_offset;
  // mem_access = execute_access;
  // mem_store = execute_store;
  // mem_load = execute_load;
  // printf("EXECUTE VALUE IN UTILS:%d\n",execute_val);
  // mem_acc_val = execute_val;
  // printf("MEMORY VALUE IN UTILS:%d\n",mem_acc_val);
  if(stall_from_issue != 0){
    stall_from_issue--;
    // exit_early();
  }
  change_to_ready(outOfOrderInstructions);
  graduate_destination = writeback_destination;

  send_for_writeback();

  if(stall_from_issue == 0 && first_decode >= 1){
    // issue_instruction = decode_instruction;
    // issue_rsource1 = decode_rsource1;
    // issue_rsource2 = decode_rsource2;
    // issue_rdestination = decode_rdestination;
    // issue_instruction_type = decode_instruction_type;
    // issue_opcode = decode_opcode;
    // issue_funct3 = decode_funct3;
    // issue_funct7 = decode_funct7;
    // issue_shamt = decode_shamt;
    // issue_imm = decode_imm;
    // issuepc = decodepc;
    issue_unit_type = decode_unit_type;
    issue_instruction_struct = decode_instruction_struct;
  }

  for(int i = 0; i < RESERVATION_WIDTH; i++){
    if(reservationalu[i].inuse == 1)reservationalu[i].inExecute = 1;
    // if(reservationalu[i].instruction.tagDestination == 22 && reservationalu[i].inuse == 1){
    //   printf("%d\n",i);
    //   exit_early();
    // }
    if(reservationalu[i].rdestination == 69){
    // if(i == 13){
      // printf("%d\n", reservationalu[i].inuse);
      // printf("Destination:%d\n",reservationalu[i].rdestination);
      // printf("Source1 ready:%d\tSource is:%d\tPhysReg1 is ready:%d\n", reservationalu[i].rsource1ready, reservationalu[i].rsource1, physRegisters[reservationalu[i].rsource1].ready);
      // printf("Source2 ready:%d\tSource is:%d\tPhysReg2 is ready:%d\n", reservationalu[i].rsource2ready, reservationalu[i].rsource2, physRegisters[reservationalu[i].rsource2].ready);
      // printf("Physical:%d\n\n",physRegisters[9].ready);
      // exit_early();
    }
  }
  for(int i = 0; i < BRANCH_RESERVATION_WIDTH; i++){
    if(reservationbru[i].inuse == 1)reservationbru[i].inExecute = 1;
  }

  if(stall_from_issue == 0 && first_fetch >= 1){
    decode_instruction = fetch_instruction;
    decodepc = fetchpc;

  }
  // if(stop == 1)exit_early();

  return;
}

void exit_early(){
  printf("EXIT EARLY\n");
  exit(1);
}

void purgepipe(){
  for(int i = 0; i < RESERVATION_WIDTH; i++){
    if(reservationalu[i].instruction.instructionid > purgeid){
      reservationalu[i].inuse = 0;
      reservationalu[i].inExecute = 0;
    }
  }
  for(int i = 0; i < BRANCH_RESERVATION_WIDTH; i++){
    if(reservationbru[i].instruction.instructionid >= purgeid){
      reservationbru[i].inuse = 0;
      reservationbru[i].inExecute = 0;
    }
  }
  deletenodeswithgreaterthanid(purgeid);
}
