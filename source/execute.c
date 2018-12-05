#include "run.h"

availNum available;
instruction currentInstruction;

//execute I format instructions
void execute_iformat(int number){
  if(currentInstruction.opcode == 0b0010011){
    switch(currentInstruction.funct3){
      case(0b000):
        // printf("Instruction:Add Immediate\n");
        executed_instruction_name = "Add Immediate";
        execute_val = addi(currentInstruction.rsource1value);
        alu[number].cyclesNeeded = 1;
        break;
      case(0b111):
        // printf("Instruction:And Immediate\n");
        executed_instruction_name = "And Immediate";
        execute_val = andi(currentInstruction.rsource1value);
        break;
      case(0b110):
        // printf("Instruction:Or Immediate\n");
        executed_instruction_name = "Or Immediate";
        execute_val = ori(currentInstruction.rsource1value);
        break;
      case(0b100):
        // printf("Instruction:Xor Immediate\n");
        executed_instruction_name = "Xor Immediate";
        execute_val = xori(currentInstruction.rsource1value);
        break;
      case(0b010):
        // printf("Instruction:STLI\n");
        executed_instruction_name = "STLI";
        execute_val = slti(currentInstruction.rsource1value);
        break;
      case(0b011):
        // printf("Instruction:STLIU\n");
        executed_instruction_name = "STLIU";
        execute_val = sltiu(currentInstruction.rsource1value);
        break;
      case(0b001):
        // printf("Instruction:Logical Left Shift Immediate\n");
        executed_instruction_name = "Logical Left Shift Immediate";
        execute_val = slli(currentInstruction.rsource1value);
        break;
      case(0b101):
        if(currentInstruction.imm == 0b0000000){
          // printf("Instruction:Logical Right Shift Immediate\n");
          executed_instruction_name = "Logical Right Shift Immediate";
          execute_val = srli(currentInstruction.rsource1value);
          break;
        }
        else if(currentInstruction.imm == 0b0100000){
          // printf("Instruction:Arithmetic Right Shift Immediate\n");
          executed_instruction_name = "Arithmetic Right Shift Immediate";
          execute_val = srai(currentInstruction.rsource1value);
          break;
        }
    }
    alu[number].valueInside = execute_val;
    alu[number].shouldWriteback = 1;
  }
  else if(currentInstruction.opcode == 0b0000011){
    // printf("Instruction:Offset Calculated for Load\n");
    // executed_instruction_name
    execute_offset = execute_imm + currentInstruction.rsource1value;
    execute_iformat2();
    // printf("execute_imm:%d\n", execute_imm);
    // printf("register_imm:%d\n", currentInstruction.rsource1value);

    execute_access = 1;
    execute_load = 1;
  }
  else if(currentInstruction.opcode == 0b1100111){
    // printf("Instruction:Jump and Link Register\n");
    executed_instruction_name = "Jump and Link Register";
    execute_val = jalr(currentInstruction.rsource1value);
  }
  else{
    printf("Error\n");
    exit(1);
  }
  return;
}

void execute_uformat(){
  switch(currentInstruction.opcode){
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

void execute_rformat(int number){
  switch(currentInstruction.funct3){
    case(0b000):
      if(currentInstruction.funct7 == 0b0000000){
        // printf("Instruction:Add\n");
        exit_early();
        executed_instruction_name = "Add";
        execute_val = add(currentInstruction.rsource1value, currentInstruction.rsource2value);
        alu[number].cyclesNeeded = 1;
      }
      else if(currentInstruction.funct7 == 0b0100000){
        // printf("Instruction:Subtract\n");
        executed_instruction_name = "Subtract";
        execute_val = sub(currentInstruction.rsource1value, currentInstruction.rsource2value);
        alu[number].cyclesNeeded = 1;
      }
      else if(currentInstruction.funct7 == 0b0000001){
        // printf("Instruction:Multiply\n");
        executed_instruction_name = "Multiply";
        execute_val = mul(currentInstruction.rsource1value, currentInstruction.rsource2value);
      }
      break;
    case(0b001):
      // printf("Instruction:Logical Left Shift\n");
      executed_instruction_name = "Logical Left Shift";
      execute_val = sll(currentInstruction.rsource1value, currentInstruction.rsource2value);
      break;
    case(0b010):
      // printf("Instruction:SLT\n");
      executed_instruction_name = "SLT";
      execute_val = slt(currentInstruction.rsource1value, currentInstruction.rsource2value);
      break;
    case(0b011):
      // printf("Instruction:SLTU\n");
      executed_instruction_name = "SLTU";
      execute_val = sltu(currentInstruction.rsource1value, currentInstruction.rsource2value);
      break;
    case(0b100):
      if(currentInstruction.funct7 == 0b0000001){
        // printf("Instruction:Divide\n");
        executed_instruction_name = "Divide";
        execute_val = divide(currentInstruction.rsource1value, currentInstruction.rsource2value);
        break;
      }
      else if(currentInstruction.funct7 == 0b0000000){
        // printf("Instruction:Xor\n");
        executed_instruction_name = "Xor";
        execute_val = xor(currentInstruction.rsource1value, currentInstruction.rsource2value);
        break;
      }
    case(0b101):
      if(currentInstruction.funct7 == 0b0000000){
        // printf("Instruction:Logical Right Shift\n");
        executed_instruction_name = "Logical Right Shift";
        execute_val = srl(currentInstruction.rsource1value, currentInstruction.rsource2value);
      }
      else if(currentInstruction.funct7 == 0b0100000){
        // printf("Instruction:Logical Right Shift\n");
        executed_instruction_name = "Logical Right Shift";
        execute_val = sra(currentInstruction.rsource1value, currentInstruction.rsource2value);
      }
      break;
    case(0b110):
      // printf("Instruction:Or\n");
      executed_instruction_name = "Or";
      execute_val = or(currentInstruction.rsource1value, currentInstruction.rsource2value);
      break;
    case(0b111):
      // printf("Instruction:And\n");
      executed_instruction_name = "And";
      execute_val = and(currentInstruction.rsource1value, currentInstruction.rsource2value);
      break;
  }
  alu[number].valueInside = execute_val;
  alu[number].shouldWriteback = 1;
  return;
}

void execute_jformat(){
  // printf("Instruction:Jump and Link\n");
  executed_instruction_name = "Jump and Link";
  jal();
  return;
}

void execute_bformat(){
  switch(currentInstruction.funct3){
    case(0b000):
      // printf("Instruction:Branch if Equal\n");
      executed_instruction_name = "Branch if Equal";
      beq(currentInstruction.rsource1value, currentInstruction.rsource2value);
      break;
    case(0b001):
      // printf("Instruction:Branch if Not Equal\n");
      executed_instruction_name = "Branch if Not Equal";
      bne(currentInstruction.rsource1value, currentInstruction.rsource2value);
      break;
    case(0b100):
      // printf("Instruction:Branch if Less Than\n");
      executed_instruction_name = "Branch if Less Than";
      blt(currentInstruction.rsource1value, currentInstruction.rsource2value);
      break;
    case(0b101):
      // printf("Instruction:Branch if Greater Than\n");
      executed_instruction_name = "Branch if Greater Than";
      bge(currentInstruction.rsource1value, currentInstruction.rsource2value);
      break;
    case(0b110):
      // printf("Instruction:Branch if Less Than Unsigned\n");
      executed_instruction_name = "Branch if Less Than Unsigned";
      bltu(currentInstruction.rsource1value, currentInstruction.rsource2value);
      break;
    case(0b111):
      // printf("Instruction:Branch if Greater Than Unsigned\n");
      executed_instruction_name = "Branch if Greater Than Unsigned";
      bgeu(currentInstruction.rsource1value, currentInstruction.rsource2value);
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
  int number = find_available_lsu();
  switch(currentInstruction.funct3){
    case(0b000):
      // printf("Instruction:Load 8-bit\n");
      executed_instruction_name = "Load 8-bit";
      execute_val = lb(Dcache, execute_offset);
      lsu[number].cyclesNeeded = 1;
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
  lsu[number].shouldWriteback = 1;
  lsu[number].valueInside = execute_val;
  return;
}

void execute_sformat2(){
  switch(currentInstruction.funct3){
    case(0b000):
      printf("Instruction:Store 8-bit\n");
      executed_instruction_name = "Store 8-bit";
      sb(Dcache, execute_offset, currentInstruction.rsource2value);
      break;
    case(0b001):
      // printf("Instruction:Store 16-bit\n");
      executed_instruction_name = "Store 16-bit";
      sh(Dcache, execute_offset, currentInstruction.rsource2value);
      break;
    case(0b010):
      // printf("Instruction:Store 32-bit\n");
      executed_instruction_name = "Store 32-bit";
      sw(Dcache, execute_offset, currentInstruction.rsource2value);
      break;
  }
  return;
}

void execute_sformat(){
  // printf("Instruction:Offset Calculated for Store\n");
  execute_offset = execute_imm + currentInstruction.rsource1value;
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
    if(alu[i].ready == 0){
      printf("HELLO\n\n");

      alu[i].currentCycles++;
      if(alu[i].currentCycles == alu[i].cyclesNeeded){
        printf("HELLO2\n\n");

        // exit_early();
        alu[i].currentCycles = 0;
        alu[i].ready = 1;
        alu[i].readyForWriteback = 1;
        // alu[i].wbValueInside = alu[i].valueInside;
        // alu[i].wbDestinationRegister = alu[i].destinationRegister;
      }
    }
  }
  for(int i = 0; i < LSU_NUM; i++){
    if(lsu[i].ready == 0){
      lsu[i].currentCycles++;
      if(lsu[i].currentCycles == lsu[i].cyclesNeeded){
        lsu[i].ready = 1;
        lsu[i].readyForWriteback = 1;
      }
    }
  }
  for(int i = 0; i < AGU_NUM; i++){
    if(agu[i].ready == 0){
      agu[i].currentCycles++;
      if(agu[i].currentCycles == agu[i].cyclesNeeded){
        agu[i].readyForWriteback = 1;
      }
    }
  }
  for(int i = 0; i < BRU_NUM; i++){
    if(bru[i].ready == 0){
      bru[i].currentCycles++;
      if(bru[i].currentCycles == bru[i].cyclesNeeded){
        bru[i].ready = 1;
        bru[i].readyForWriteback = 1;
      }
    }
  }
  return;
}

void forward_reservation_stations(int tagPassed, int value, int unit_type){
  if(unit_type == 1){
    for(int i = 0; i < reservationIteratorALU; i++){
      if(reservationalu[i].rsource1ready == 0 && reservationalu[i].rsource1 == tagPassed){
        reservationalu[i].rsource1value = value;
        reservationalu[i].rsource1ready = 1;
      }
      if(reservationalu[i].rsource2ready == 0 && reservationalu[i].rsource1 == tagPassed){
        reservationalu[i].rsource2value = value;
        reservationalu[i].rsource2ready = 1;
      }
    }
  }
}

void send_for_writeback(){
  for(int i = 0; i < ALU_NUM; i++){
    if(alu[i].readyForWriteback == 1 && alu[i].shouldWriteback == 1){
      alu[i].wbValueInside = alu[i].valueInside;
      alu[i].readyForWriteback = 0;
      alu[i].shouldWriteback = 0;
      // alu[i].wbDestinationRegister = alu[i].destinationRegister;
      // alu[i].readyForWriteback = 2;
      writebackalu[i].value = alu[i].valueInside;
      writebackalu[i].ready = 1;
      writebackalu[i].tag = alu[i].destinationRegister;
      forward_reservation_stations(alu[i].destinationRegister, alu[i].valueInside, 1);
    }
  }
  for(int i = 0; i < LSU_NUM; i++){
    if(lsu[i].readyForWriteback == 1){
      lsu[i].readyForWriteback = 2;
    }
  }
  for(int i = 0; i < AGU_NUM; i++){
    if(agu[i].readyForWriteback == 1){
      agu[i].readyForWriteback = 2;
    }
  }
  for(int i = 0; i < BRU_NUM; i++){
    if(bru[i].readyForWriteback == 1){
      bru[i].readyForWriteback = 2;
    }
  }
}

//finds number of available ALUs
availNum find_available_alu(){
  int i;
  available.number = 0;
  for(i = 0; i < ALU_NUM; i++){
    if(alu[i].ready == 1){
      available.unitNumber[available.number] = i;
      available.number++;
    }
  }
  return available;
}

int find_available_lsu(){
  int i;
  for(i = 0; i < LSU_NUM; i++){
    if(lsu[i].ready == 1){
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
    if(agu[i].ready == 1){
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
    if(bru[i].ready == 1){
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

//Fetches instructions from Reservation Stations
instructionwrapper check_reservation_alu(int numberOfAvailableUnits){
  instructionwrapper wrappedListofInstructions;
  wrappedListofInstructions.foundInstructions = 0;
  for(int i = 0; i < RESERVATION_WIDTH; i++){
    if(reservationalu[i].rsource1ready && reservationalu[i].rsource2ready && reservationalu[i].inuse && reservationalu[i].inExecute){
      instruction newInstruction;
      newInstruction.rdestination = reservationalu[i].rdestination;
      newInstruction.rsource1value = reservationalu[i].rsource1value;
      newInstruction.rsource2value = reservationalu[i].rsource2value;
      newInstruction.opcode = reservationalu[i].opcode;
      newInstruction.funct3 = reservationalu[i].funct3;
      newInstruction.funct7 = reservationalu[i].funct7;
      newInstruction.shamt = reservationalu[i].shamt;
      newInstruction.imm = reservationalu[i].imm;
      newInstruction.pc = reservationalu[i].pc;
      newInstruction.instruction_type = reservationalu[i].instruction_type;
      wrappedListofInstructions.instruction[wrappedListofInstructions.foundInstructions] = newInstruction;
      wrappedListofInstructions.foundInstructions++;
      reservationalu[i].inuse = 0;
    }
    if(wrappedListofInstructions.foundInstructions == numberOfAvailableUnits){
      return wrappedListofInstructions;
    }

  }

  return wrappedListofInstructions;
}

void execute(){

  if(first_issue < 2){
    print_execute_summary = 0;
    return;
  }
  if(first_execute < 2)first_execute++;
  if(last_instruction == 1){
    if(current_cycle > last_instruction_cycle + 3){
      print_execute_summary = 0;
      // return;
    }
  }
  print_execute_summary = 1;
  availNum currentAvailable = find_available_alu();
  instructionwrapper currentInstructions = check_reservation_alu(currentAvailable.number);

  for(int i = 0; i < currentInstructions.foundInstructions; i++){

    int alu_unit_number = currentAvailable.unitNumber[i];
    currentInstruction = currentInstructions.instruction[i];
    alu[alu_unit_number].ready = 0;
    alu[alu_unit_number].destinationRegister = currentInstruction.rdestination;

    printf("INSTURCDSHHFI%d\n",currentInstruction.instruction_type);
    // exit_early();

    if(currentInstruction.instruction_type == 1){
      execute_iformat(alu_unit_number);
    }
    else if(currentInstruction.instruction_type == 3){
      execute_rformat(alu_unit_number);
    }
    instructions_executed++;
    // exit_early();
  }


  // if(execute_instruction_type == 4 || execute_instruction_type == 6){
  //   execute_sj();
  //   return;
  // }
  //
  // if(execute_instruction_type == 1){
  //   // execute_iformat();
  // }
  // else if(execute_instruction_type == 2){
  //   execute_uformat();
  // }
  // else if(execute_instruction_type == 3){
  //   // execute_rformat();
  // }
  // else if(execute_instruction_type == 5){
  //   execute_bformat();
  // }

  // send_for_writeback();

  increment_units();

  return;
}
