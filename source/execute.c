#include "run.h"

//execute I format instructions
void execute_iformat(){
  if(execute_opcode == 0b0010011){
    int number = find_available_alu();
    switch(execute_funct3){
      case(0b000):
        // printf("Instruction:Add Immediate\n");
        executed_instruction_name = "Add Immediate";
        execute_val = addi(execute_rsource1);
        alu[number].cyclesNeeded = 2;
        break;
      case(0b111):
        // printf("Instruction:And Immediate\n");
        executed_instruction_name = "And Immediate";
        execute_val = andi(execute_rsource1);
        break;
      case(0b110):
        // printf("Instruction:Or Immediate\n");
        executed_instruction_name = "Or Immediate";
        execute_val = ori(execute_rsource1);
        break;
      case(0b100):
        // printf("Instruction:Xor Immediate\n");
        executed_instruction_name = "Xor Immediate";
        execute_val = xori(execute_rsource1);
        break;
      case(0b010):
        // printf("Instruction:STLI\n");
        executed_instruction_name = "STLI";
        execute_val = slti(execute_rsource1);
        break;
      case(0b011):
        // printf("Instruction:STLIU\n");
        executed_instruction_name = "STLIU";
        execute_val = sltiu(execute_rsource1);
        break;
      case(0b001):
        // printf("Instruction:Logical Left Shift Immediate\n");
        executed_instruction_name = "Logical Left Shift Immediate";
        execute_val = slli(execute_rsource1);
        break;
      case(0b101):
        if(execute_imm == 0b0000000){
          // printf("Instruction:Logical Right Shift Immediate\n");
          executed_instruction_name = "Logical Right Shift Immediate";
          execute_val = srli(execute_rsource1);
          break;
        }
        else if(execute_imm == 0b0100000){
          // printf("Instruction:Arithmetic Right Shift Immediate\n");
          executed_instruction_name = "Arithmetic Right Shift Immediate";
          execute_val = srai(execute_rsource1);
          break;
        }
    }
    alu[number].valueInside = execute_val;
  }
  else if(execute_opcode == 0b0000011){
    // printf("Instruction:Offset Calculated for Load\n");
    // executed_instruction_name
    execute_offset = execute_imm + execute_rsource1;
    execute_iformat2();
    // printf("execute_imm:%d\n", execute_imm);
    // printf("register_imm:%d\n", execute_rsource1);

    execute_access = 1;
    execute_load = 1;
  }
  else if(execute_opcode == 0b1100111){
    // printf("Instruction:Jump and Link Register\n");
    executed_instruction_name = "Jump and Link Register";
    execute_val = jalr(execute_rsource1);
  }
  else{
    printf("Error\n");
    exit(1);
  }
  return;
}

void execute_uformat(){
  switch(execute_opcode){
    case(0b0110111):
      // printf("Instruction:Load Upper Immediate\n");
      executed_instruction_name = "Load Upper Immediate";
      execute_val = lui();
      break;
    case(0b0010111):
      // printf("Instruction:Add Upper Immediate to PC\n");
      executed_instruction_name = "Add Upper Immediate to PC";
      auipc();
      break;
  }
  return;
}

void execute_rformat(){
  switch(execute_funct3){
    case(0b000):
      if(execute_funct7 == 0b0000000){
        // printf("Instruction:Add\n");
        executed_instruction_name = "Add";
        execute_val = add(execute_rsource1, execute_rsource2);
      }
      else if(execute_funct7 == 0b0100000){
        // printf("Instruction:Subtract\n");
        executed_instruction_name = "Subtract";
        execute_val = sub(execute_rsource1, execute_rsource2);
      }
      else if(execute_funct7 == 0b0000001){
        // printf("Instruction:Multiply\n");
        executed_instruction_name = "Multiply";
        execute_val = mul(execute_rsource1, execute_rsource2);
      }
      break;
    case(0b001):
      // printf("Instruction:Logical Left Shift\n");
      executed_instruction_name = "Logical Left Shift";
      execute_val = sll(execute_rsource1, execute_rsource2);
      break;
    case(0b010):
      // printf("Instruction:SLT\n");
      executed_instruction_name = "SLT";
      execute_val = slt(execute_rsource1, execute_rsource2);
      break;
    case(0b011):
      // printf("Instruction:SLTU\n");
      executed_instruction_name = "SLTU";
      execute_val = sltu(execute_rsource1, execute_rsource2);
      break;
    case(0b100):
      if(execute_funct7 == 0b0000001){
        // printf("Instruction:Divide\n");
        executed_instruction_name = "Divide";
        execute_val = divide(execute_rsource1, execute_rsource2);
        break;
      }
      else if(execute_funct7 == 0b0000000){
        // printf("Instruction:Xor\n");
        executed_instruction_name = "Xor";
        execute_val = xor(execute_rsource1, execute_rsource2);
        break;
      }
    case(0b101):
      if(execute_funct7 == 0b0000000){
        // printf("Instruction:Logical Right Shift\n");
        executed_instruction_name = "Logical Right Shift";
        execute_val = srl(execute_rsource1, execute_rsource2);
      }
      else if(execute_funct7 == 0b0100000){
        // printf("Instruction:Logical Right Shift\n");
        executed_instruction_name = "Logical Right Shift";
        execute_val = sra(execute_rsource1, execute_rsource2);
      }
      break;
    case(0b110):
      // printf("Instruction:Or\n");
      executed_instruction_name = "Or";
      execute_val = or(execute_rsource1, execute_rsource2);
      break;
    case(0b111):
      // printf("Instruction:And\n");
      executed_instruction_name = "And";
      execute_val = and(execute_rsource1, execute_rsource2);
      break;
  }
  return;
}

void execute_jformat(){
  // printf("Instruction:Jump and Link\n");
  executed_instruction_name = "Jump and Link";
  jal();
  return;
}

void execute_bformat(){
  switch(execute_funct3){
    case(0b000):
      // printf("Instruction:Branch if Equal\n");
      executed_instruction_name = "Branch if Equal";
      beq(execute_rsource1, execute_rsource2);
      break;
    case(0b001):
      // printf("Instruction:Branch if Not Equal\n");
      executed_instruction_name = "Branch if Not Equal";
      bne(execute_rsource1, execute_rsource2);
      break;
    case(0b100):
      // printf("Instruction:Branch if Less Than\n");
      executed_instruction_name = "Branch if Less Than";
      blt(execute_rsource1, execute_rsource2);
      break;
    case(0b101):
      // printf("Instruction:Branch if Greater Than\n");
      executed_instruction_name = "Branch if Greater Than";
      bge(execute_rsource1, execute_rsource2);
      break;
    case(0b110):
      // printf("Instruction:Branch if Less Than Unsigned\n");
      executed_instruction_name = "Branch if Less Than Unsigned";
      bltu(execute_rsource1, execute_rsource2);
      break;
    case(0b111):
      // printf("Instruction:Branch if Greater Than Unsigned\n");
      executed_instruction_name = "Branch if Greater Than Unsigned";
      bgeu(execute_rsource1, execute_rsource2);
      break;
  }
  return;
}

void execute_iformat2(){
  // printf("load offset:%d\n",execute_offset);
  // if(lsu[number].currentCycles != 0 && lsu[number].cyclesNeeded != 0){
  //   if(lsu[number].currentCycles == lsu[number].cyclesNeeded){
  //     lsu[number].ready = 1;
  //   }
  // }
  // if(lsu[number].ready != 1){
  //   return;
  // }
  switch(execute_funct3){
    case(0b000):
      // printf("Instruction:Load 8-bit\n");
      executed_instruction_name = "Load 8-bit";
      execute_val = lb(Dcache, execute_offset);
      break;
    case(0b001):
      // printf("Instruction:Load 16-bit\n");
      executed_instruction_name = "Load 16-bit";
      execute_val = lh(Dcache, execute_offset);
      break;
    case(0b010):
      // printf("Instruction:Load 32-bit\n");
      executed_instruction_name = "Load 32-bit";
      execute_val = lw(Dcache, execute_offset);
      break;
    case(0b100):
      // printf("Instruction:Load 8-bit Unsigned\n");
      executed_instruction_name = "Load 8-bit Unsigned";
      execute_val = lbu(Dcache, execute_offset);
      break;
    case(0b101):
      // printf("Instruction:Load 16-bit Unsigned\n");
      executed_instruction_name = "Load 16-bit Unsigned";
      execute_val = lhu(Dcache, execute_offset);
      break;
  }
  return;
}

void execute_sformat2(){
  switch(execute_funct3){
    case(0b000):
      printf("Instruction:Store 8-bit\n");
      executed_instruction_name = "Store 8-bit";
      sb(Dcache, execute_offset, execute_rsource2);
      break;
    case(0b001):
      // printf("Instruction:Store 16-bit\n");
      executed_instruction_name = "Store 16-bit";
      sh(Dcache, execute_offset, execute_rsource2);
      break;
    case(0b010):
      // printf("Instruction:Store 32-bit\n");
      executed_instruction_name = "Store 32-bit";
      sw(Dcache, execute_offset, execute_rsource2);
      break;
  }
  return;
}

void execute_sformat(){
  // printf("Instruction:Offset Calculated for Store\n");
  execute_offset = execute_imm + execute_rsource1;
  execute_sformat2();
}

void execute_sj(){
  if(execute_instruction_type == 4){
    execute_jformat();
  }
  else if(execute_instruction_type == 6){
    execute_sformat();
  }
  return;
}

void increment_units(){
  for(int i = 0; i < ALU_NUM; i++){
    if(alu[ALU_NUM].ready == 0){
      alu[ALU_NUM].currentCycles++;
      if(alu[ALU_NUM].currentCycles == alu[ALU_NUM].cyclesNeeded){
        alu[ALU_NUM].ready = 1;
        alu[ALU_NUM].readyForWriteback = 1;
      }
    }
  }
  for(int i = 0; i < LSU_NUM; i++){
    if(lsu[LSU_NUM].ready == 0){
      lsu[LSU_NUM].currentCycles++;
      if(lsu[LSU_NUM].currentCycles == lsu[LSU_NUM].cyclesNeeded){
        lsu[LSU_NUM].ready = 1;
        lsu[LSU_NUM].readyForWriteback = 1;
      }
    }
  }
  for(int i = 0; i < AGU_NUM; i++){
    if(agu[AGU_NUM].ready == 0){
      agu[AGU_NUM].currentCycles++;
      if(agu[AGU_NUM].currentCycles == agu[AGU_NUM].cyclesNeeded){
        agu[AGU_NUM].readyForWriteback = 1;
      }
    }
  }
  for(int i = 0; i < BRU_NUM; i++){
    if(bru[BRU_NUM].ready == 0){
      bru[BRU_NUM].currentCycles++;
      if(bru[BRU_NUM].currentCycles == bru[BRU_NUM].cyclesNeeded){
        bru[BRU_NUM].ready = 1;
        bru[BRU_NUM].readyForWriteback = 1;
      }
    }
  }
  return;
}

void send_for_writeback(){
  for(int i = 0; i < ALU_NUM; i++){
    if(alu[ALU_NUM].readyForWriteback == 1){
      alu[ALU_NUM].readyForWriteback = 2;
    }
  }
  for(int i = 0; i < LSU_NUM; i++){
    if(lsu[LSU_NUM].readyForWriteback == 1){
      lsu[LSU_NUM].readyForWriteback = 2;
    }
  }
  for(int i = 0; i < AGU_NUM; i++){
    if(agu[AGU_NUM].readyForWriteback == 1){
      agu[AGU_NUM].readyForWriteback = 2;
    }
  }
  for(int i = 0; i < BRU_NUM; i++){
    if(bru[BRU_NUM].readyForWriteback == 1){
      bru[BRU_NUM].readyForWriteback = 2;
    }
  }
}

int find_available_alu(){
  int i;
  for(i = 0; i < ALU_NUM; i++){
    if(alu[ALU_NUM].ready == 1){
      break;
    }
  }
  if(i == ALU_NUM){
    return -1;
  }
  else{
    return i;
  }
}

int find_available_lsu(){
  int i;
  for(i = 0; i < LSU_NUM; i++){
    if(lsu[LSU_NUM].ready == 1){
      break;
    }
  }
  if(i == LSU_NUM){
    return -1;
  }
  else{
    return i;
  }
}

int find_available_agu(){
  int i;
  for(i = 0; i < AGU_NUM; i++){
    if(agu[AGU_NUM].ready == 1){
      break;
    }
  }
  if(i == AGU_NUM){
    return -1;
  }
  else{
    return i;
  }
}

int find_available_bru(){
  int i;
  for(i = 0; i < BRU_NUM; i++){
    if(bru[BRU_NUM].ready == 1){
      break;
    }
  }
  if(i == BRU_NUM){
    return -1;
  }
  else{
    return i;
  }
}

void execute(){

  if(first_decode < 2){
    print_execute_summary = 0;
    return;
  }
  if(first_execute < 2)first_execute++;
  if(last_instruction == 1){
    if(current_cycle > last_instruction_cycle + 2){
      print_execute_summary = 0;
      return;
    }
  }
  print_execute_summary = 1;

  if(execute_instruction_type == -1){
    printf("End of Program\n");
    exit(1);
  }
  instructions_executed++;

  if(execute_instruction_type == 4 || execute_instruction_type == 6){
    execute_sj();
    return;
  }

  if(execute_instruction_type == 1){
    execute_iformat();
  }
  else if(execute_instruction_type == 2){
    execute_uformat();
  }
  else if(execute_instruction_type == 3){
    execute_rformat();
  }
  else if(execute_instruction_type == 5){
    execute_bformat();
  }

  send_for_writeback();

  increment_units();

  return;
}
