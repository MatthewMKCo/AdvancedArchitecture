#include "run.h"

availNum available;
instruction currentInstruction;
int numberOfExecutedInstructions = 0;
int check_execute_and_reservation_units(){
  for(int i = 0; i < RESERVATION_WIDTH; i++){
    if(reservationalu[i].inuse){
      return 0;
    }
  }
  for(int i = 0; i < BRANCH_RESERVATION_WIDTH; i++){
    if(reservationbru[i].inuse){
      return 0;
    }
  }
  for(int i = 0; i < STORE_RESERVATION_WIDTH; i++){
    if(reservationlsu[i].inuse){
      return 0;
    }
  }
  for(int i = 0; i < ALU_NUM; i++){
    if(alu[i].ready == 0){
      return 0;
    }
  }
  for(int i = 0; i < BRU_NUM; i++){
    if(bru[i].ready == 0){
      return 0;
    }
  }
  for(int i = 0; i < LSU_NUM; i++){
    if(lsu[i].ready == 0){
      return 0;
    }
  }
  return 1;
}
int b2 = 0;
//execute I format instructions
void execute_iformat(int number){
  if(currentInstruction.opcode == 0b0010011){
    alu[number].instruction.instructionid = currentInstruction.instructionid;
    switch(currentInstruction.funct3){
      case(0b000):
        // printf("Instruction:Add Immediate\n");
        executed_instruction_name[numberOfExecutedInstructions] = "Add Immediate";
        execute_val = addi(currentInstruction.rsource1value);
        alu[number].cyclesNeeded = 1;
        break;
      case(0b111):
        // printf("Instruction:And Immediate\n");
        executed_instruction_name[numberOfExecutedInstructions] = "And Immediate";
        execute_val = andi(currentInstruction.rsource1value);
        alu[number].cyclesNeeded = 1;
        break;
      case(0b110):
        // printf("Instruction:Or Immediate\n");
        executed_instruction_name[numberOfExecutedInstructions] = "Or Immediate";
        execute_val = ori(currentInstruction.rsource1value);
        alu[number].cyclesNeeded = 1;
        break;
      case(0b100):
        // printf("Instruction:Xor Immediate\n");
        executed_instruction_name[numberOfExecutedInstructions] = "Xor Immediate";
        execute_val = xori(currentInstruction.rsource1value);
        alu[number].cyclesNeeded = 1;
        break;
      case(0b010):
        // printf("Instruction:STLI\n");
        executed_instruction_name[numberOfExecutedInstructions] = "STLI";
        execute_val = slti(currentInstruction.rsource1value);
        alu[number].cyclesNeeded = 1;
        break;
      case(0b011):
        // printf("Instruction:STLIU\n");
        executed_instruction_name[numberOfExecutedInstructions] = "STLIU";
        execute_val = sltiu(currentInstruction.rsource1value);
        alu[number].cyclesNeeded = 1;
        break;
      case(0b001):
        // printf("Instruction:Logical Left Shift Immediate\n");
        executed_instruction_name[numberOfExecutedInstructions] = "Logical Left Shift Immediate";
        execute_val = slli(currentInstruction.rsource1value);
        alu[number].cyclesNeeded = 1;
        break;
      case(0b101):
        if(currentInstruction.imm == 0b0000000){
          // printf("Instruction:Logical Right Shift Immediate\n");
          executed_instruction_name[numberOfExecutedInstructions] = "Logical Right Shift Immediate";
          execute_val = srli(currentInstruction.rsource1value);
          alu[number].cyclesNeeded = 1;
          break;
        }
        else if(currentInstruction.imm == 0b0100000){
          // printf("Instruction:Arithmetic Right Shift Immediate\n");
          executed_instruction_name[numberOfExecutedInstructions] = "Arithmetic Right Shift Immediate";
          execute_val = srai(currentInstruction.rsource1value);
          alu[number].cyclesNeeded = 1;
          break;
        }
    }
    alu[number].valueInside = execute_val;
    alu[number].shouldWriteback = 1;
    alu[number].instruction_name = executed_instruction_name[numberOfExecutedInstructions];
  }
  else if(currentInstruction.opcode == 0b0000011){
    // printf("Instruction:Offset Calculated for Load\n");
    // executed_instruction_name[numberOfExecutedInstructions]
    execute_offset = currentInstruction.imm + currentInstruction.rsource1value;
    execute_iformat2(number);
    // printf("execute_imm:%d\n", execute_imm);
    // printf("register_imm:%d\n", currentInstruction.rsource1value);

    execute_access = 1;
    execute_load = 1;
  }
  else if(currentInstruction.opcode == 0b1100111){
    alu[number].instruction.instructionid = currentInstruction.instructionid;
    // printf("Instruction:Jump and Link Register\n");
    executed_instruction_name[numberOfExecutedInstructions] = "Jump and Link Register";
    execute_val = jalr(currentInstruction.rsource1value);
    alu[number].cyclesNeeded = 1;
  }
  else{
    printf("Error\n");
    exit(1);
  }
  return;
}

void execute_uformat(int number){
  alu[number].instruction.instructionid = currentInstruction.instructionid;
  switch(currentInstruction.opcode){
    case(0b0110111):
      // printf("Instruction:Load Upper Immediate\n");
      executed_instruction_name[numberOfExecutedInstructions] = "Load Upper Immediate";
      execute_val = lui();
      alu[number].cyclesNeeded = 1;
      alu[number].valueInside = execute_val;
      alu[number].shouldWriteback = 1;
      break;
    case(0b0010111):
      // printf("Instruction:Add Upper Immediate to PC\n");
      executed_instruction_name[numberOfExecutedInstructions] = "Add Upper Immediate to PC";
      auipc();
      alu[number].cyclesNeeded = 1;
      alu[number].shouldWriteback = 0;
      break;
  }
  return;
}

void execute_rformat(int number){
  alu[number].instruction.instructionid = currentInstruction.instructionid;
  switch(currentInstruction.funct3){
    case(0b000):
      if(currentInstruction.funct7 == 0b0000000){
        // printf("Instruction:Add\n");
        executed_instruction_name[numberOfExecutedInstructions] = "Add";
        execute_val = add(currentInstruction.rsource1value, currentInstruction.rsource2value);
        alu[number].cyclesNeeded = 1;
      }
      else if(currentInstruction.funct7 == 0b0100000){
        // printf("Instruction:Subtract\n");
        executed_instruction_name[numberOfExecutedInstructions] = "Subtract";
        execute_val = sub(currentInstruction.rsource1value, currentInstruction.rsource2value);
        alu[number].cyclesNeeded = 1;
      }
      else if(currentInstruction.funct7 == 0b0000001){
        // printf("Instruction:Multiply\n");
        executed_instruction_name[numberOfExecutedInstructions] = "Multiply";
        execute_val = mul(currentInstruction.rsource1value, currentInstruction.rsource2value);
        alu[number].cyclesNeeded = 5;
      }
      break;
    case(0b001):
      // printf("Instruction:Logical Left Shift\n");
      executed_instruction_name[numberOfExecutedInstructions] = "Logical Left Shift";
      execute_val = sll(currentInstruction.rsource1value, currentInstruction.rsource2value);
      alu[number].cyclesNeeded = 1;
      break;
    case(0b010):
      // printf("Instruction:SLT\n");
      executed_instruction_name[numberOfExecutedInstructions] = "SLT";
      execute_val = slt(currentInstruction.rsource1value, currentInstruction.rsource2value);
      alu[number].cyclesNeeded = 1;
      break;
    case(0b011):
      // printf("Instruction:SLTU\n");
      executed_instruction_name[numberOfExecutedInstructions] = "SLTU";
      execute_val = sltu(currentInstruction.rsource1value, currentInstruction.rsource2value);
      alu[number].cyclesNeeded = 1;
      break;
    case(0b100):
      if(currentInstruction.funct7 == 0b0000001){
        // printf("Instruction:Divide\n");
        executed_instruction_name[numberOfExecutedInstructions] = "Divide";
        execute_val = divide(currentInstruction.rsource1value, currentInstruction.rsource2value);
        alu[number].cyclesNeeded = 1;
        break;
      }
      else if(currentInstruction.funct7 == 0b0000000){
        // printf("Instruction:Xor\n");
        executed_instruction_name[numberOfExecutedInstructions] = "Xor";
        execute_val = xor(currentInstruction.rsource1value, currentInstruction.rsource2value);
        alu[number].cyclesNeeded = 1;
        break;
      }
    case(0b101):
      if(currentInstruction.funct7 == 0b0000000){
        // printf("Instruction:Logical Right Shift\n");
        executed_instruction_name[numberOfExecutedInstructions] = "Logical Right Shift";
        execute_val = srl(currentInstruction.rsource1value, currentInstruction.rsource2value);
        alu[number].cyclesNeeded = 1;
      }
      else if(currentInstruction.funct7 == 0b0100000){
        // printf("Instruction:Logical Right Shift\n");
        executed_instruction_name[numberOfExecutedInstructions] = "Logical Right Shift";
        execute_val = sra(currentInstruction.rsource1value, currentInstruction.rsource2value);
        alu[number].cyclesNeeded = 1;
      }
      break;
    case(0b110):
      // printf("Instruction:Or\n");
      executed_instruction_name[numberOfExecutedInstructions] = "Or";
      execute_val = or(currentInstruction.rsource1value, currentInstruction.rsource2value);
      alu[number].cyclesNeeded = 1;
      break;
    case(0b111):
      // printf("Instruction:And\n");
      executed_instruction_name[numberOfExecutedInstructions] = "And";
      execute_val = and(currentInstruction.rsource1value, currentInstruction.rsource2value);
      alu[number].cyclesNeeded = 1;
      break;
  }
  alu[number].valueInside = execute_val;
  alu[number].shouldWriteback = 1;
  alu[number].instruction_name = executed_instruction_name[numberOfExecutedInstructions];
  return;
}

void execute_jformat(int number){
  // printf("Instruction:Jump and Link\n");
  executed_instruction_name[numberOfExecutedInstructions] = "Jump and Link";
  jal();
  return;
}

void execute_bformat(int number){
  bru[number].instruction = currentInstruction;
  switch(currentInstruction.funct3){
    case(0b000):
      // printf("Instruction:Branch if Equal\n");
      executed_instruction_name[numberOfExecutedInstructions] = "Branch if Equal";
      execute_val = beq(currentInstruction.rsource1value, currentInstruction.rsource2value);
      bru[number].cyclesNeeded = 1;
      break;
    case(0b001):
      // printf("Instruction:Branch if Not Equal\n");
      executed_instruction_name[numberOfExecutedInstructions] = "Branch if Not Equal";
      execute_val = bne(currentInstruction.rsource1value, currentInstruction.rsource2value);
      bru[number].cyclesNeeded = 1;
      break;
    case(0b100):
      // printf("Instruction:Branch if Less Than\n");
      executed_instruction_name[numberOfExecutedInstructions] = "Branch if Less Than";
      execute_val = blt(currentInstruction.rsource1value, currentInstruction.rsource2value);
      bru[number].cyclesNeeded = 1;
      break;
    case(0b101):
      // printf("Instruction:Branch if Greater Than\n");
      executed_instruction_name[numberOfExecutedInstructions] = "Branch if Greater Than";
      execute_val = bge(currentInstruction.rsource1value, currentInstruction.rsource2value);
      bru[number].cyclesNeeded = 1;
      break;
    case(0b110):
      // printf("Instruction:Branch if Less Than Unsigned\n");
      executed_instruction_name[numberOfExecutedInstructions] = "Branch if Less Than Unsigned";
      execute_val = bltu(currentInstruction.rsource1value, currentInstruction.rsource2value);
      bru[number].cyclesNeeded = 1;
      break;
    case(0b111):
      // printf("Instruction:Branch if Greater Than Unsigned\n");
      executed_instruction_name[numberOfExecutedInstructions] = "Branch if Greater Than Unsigned";
      execute_val = bgeu(currentInstruction.rsource1value, currentInstruction.rsource2value);
      bru[number].cyclesNeeded = 1;
      break;
  }
  bru[number].valueInside = execute_val;
  bru[number].instruction_name = executed_instruction_name[numberOfExecutedInstructions];
  return;
}

void execute_iformat2(int number){
  // printf("load offset:%d\n",execute_offset);
  // if(lsu[number].currentCycles != 0 && lsu[number].cyclesNeeded != 0){
  //   if(lsu[number].currentCycles == lsu[number].cyclesNeeded){
  //     lsu[number].ready = 1;
  //   }
  // }
  // if(lsu[number].ready != 1){
  //   return;
  // }
  lsu[number].instruction.instructionid = currentInstruction.instructionid;
  switch(currentInstruction.funct3){
    case(0b000):
      // printf("Instruction:Load 8-bit\n");
      executed_instruction_name[numberOfExecutedInstructions] = "Load 8-bit";
      execute_val = lb(Dcache, execute_offset);
      lsu[number].cyclesNeeded = 1;
      break;
    case(0b001):
      // printf("Instruction:Load 16-bit\n");
      executed_instruction_name[numberOfExecutedInstructions] = "Load 16-bit";
      execute_val = lh(Dcache, execute_offset);
      lsu[number].cyclesNeeded = 1;
      break;
    case(0b010):
      // printf("Instruction:Load 32-bit\n");
      executed_instruction_name[numberOfExecutedInstructions] = "Load 32-bit";
      int i = 0;
      int old_instructionid = -1;
      int found_id_flag = 0;
      for(; i < STORE_RESERVATION_WIDTH; i++){
        if(reservationlsu[i].rdestination == execute_offset){
          if(reservationlsu[i].instructionid < currentInstruction.instructionid && reservationlsu[i].instructionid > old_instructionid && reservationlsu[i].inuse){
            old_instructionid = reservationlsu[i].instructionid;
            execute_val = reservationlsu[i].value;
            found_id_flag = 1;
            // break;
          }
        }
      }
      if(found_id_flag == 1){
        lsu[number].cyclesNeeded = 1;
        break;
      }
      execute_val = lw(Dcache, execute_offset);
      lsu[number].cyclesNeeded = 4;
      break;
    case(0b100):
      // printf("Instruction:Load 8-bit Unsigned\n");
      executed_instruction_name[numberOfExecutedInstructions] = "Load 8-bit Unsigned";
      execute_val = lbu(Dcache, execute_offset);
      lsu[number].cyclesNeeded = 1;
      break;
    case(0b101):
      // printf("Instruction:Load 16-bit Unsigned\n");
      executed_instruction_name[numberOfExecutedInstructions] = "Load 16-bit Unsigned";
      execute_val = lhu(Dcache, execute_offset);
      lsu[number].cyclesNeeded = 1;
      break;
  }
  lsu[number].shouldWriteback = 1;
  lsu[number].valueInside = execute_val;
  lsu[number].instruction_name = executed_instruction_name[numberOfExecutedInstructions];
  return;
}

void execute_sformat2(int number){
  lsu[number].instruction.instructionid = currentInstruction.instructionid;
  switch(currentInstruction.funct3){
    case(0b000):
      // printf("Instruction:Store 8-bit\n");
      executed_instruction_name[numberOfExecutedInstructions] = "Store 8-bit";
      sb(Dcache, execute_offset, currentInstruction.rsource2value);
      execute_val = currentInstruction.rsource2value;
      lsu[number].cyclesNeeded = 1;
      break;
    case(0b001):
      // printf("Instruction:Store 16-bit\n");
      executed_instruction_name[numberOfExecutedInstructions] = "Store 16-bit";
      sh(Dcache, execute_offset, currentInstruction.rsource2value);
      execute_val = currentInstruction.rsource2value;
      lsu[number].cyclesNeeded = 1;
      break;
    case(0b010):
      // printf("Instruction:Store 32-bit\n");
      executed_instruction_name[numberOfExecutedInstructions] = "Store 32-bit";
      // sw(Dcache, execute_offset, currentInstruction.rsource2value);
      int i = 0;
      for(; i < STORE_RESERVATION_WIDTH; i++){
        if(reservationlsu[i].instructionid == currentInstruction.instructionid){
          reservationlsu[i].value = currentInstruction.rsource2value;
          reservationlsu[i].rdestination = execute_offset;
          break;
        }
      }
      if(i == STORE_RESERVATION_WIDTH){
        printf("ERROR in Store Word");
        exit_early();
      }
      execute_val = currentInstruction.rsource2value;
      lsu[number].cyclesNeeded = 1;
      break;
  }
  lsu[number].valueInside = execute_val;
  lsu[number].destinationRegister = execute_offset;
  lsu[number].instruction_name = executed_instruction_name[numberOfExecutedInstructions];
  return;
}

void execute_sformat(int number){
  // printf("Instruction:Offset Calculated for Store\n");
  execute_offset = currentInstruction.imm + currentInstruction.rsource1value;
  execute_sformat2(number);
}

void increment_units(){
  for(int i = 0; i < ALU_NUM; i++){
    if(alu[i].ready == 0){
      alu[i].currentCycles++;
      if(alu[i].currentCycles == alu[i].cyclesNeeded){
        alu[i].currentCycles = 0;
        alu[i].ready = 1;
        alu[i].readyForWriteback = 1;
        // alu[i].wbValueInside = alu[i].valueInside;
        // alu[i].wbDestinationRegister = alu[i].destinationRegister;
      }
    }
  }
  for(int i = 0; i < BRU_NUM; i++){
    if(bru[i].ready == 0){
      bru[i].currentCycles++;
      if(bru[i].currentCycles == bru[i].cyclesNeeded){
        bru[i].currentCycles = 0;
        bru[i].ready = 1;
        bru[i].readyForWriteback = 1;
        if(bru[i].valueInside == 1){
          purge = 1;
          purgeid = bru[i].instruction.instructionid;
          instructionid = bru[i].instruction.instructionid + 1;
          pc[0] = change_pc_execute(bru[i].instruction.pc, bru[i].instruction.imm, bru[i].instruction.branchTaken);
          // exit_early();
          // printf("%d\n",purgeid);
          // if(bru[i].instruction.instructionid == 88)exit_early();
        }
        // alu[i].wbValueInside = alu[i].valueInside;
        // alu[i].wbDestinationRegister = alu[i].destinationRegister;
      }
    }
  }
  for(int i = 0; i < LSU_NUM; i++){
    if(lsu[i].ready == 0){
      lsu[i].currentCycles++;
      if(lsu[i].currentCycles == lsu[i].cyclesNeeded){
        lsu[i].currentCycles = 0;
        lsu[i].ready = 1;
        lsu[i].readyForWriteback = 1;
        // alu[i].wbValueInside = alu[i].valueInside;
        // alu[i].wbDestinationRegister = alu[i].destinationRegister;
      }
    }
  }
  return;
}

void forward_reservation_stations(int tagPassed, int value){
    if(check_tag_for_zero(inuseTags, tagPassed))value = 0;
    for(int i = 0; i < RESERVATION_WIDTH; i++){
      if(reservationalu[i].rsource1ready == 0 && reservationalu[i].rsource1 == tagPassed){
        reservationalu[i].rsource1value = value;
        reservationalu[i].instruction.rsource1value = value;
        reservationalu[i].rsource1ready = 1;
      }
      if(reservationalu[i].rsource2ready == 0 && reservationalu[i].rsource2 == tagPassed){
        reservationalu[i].rsource2value = value;
        reservationalu[i].instruction.rsource2value = value;
        reservationalu[i].rsource2ready = 1;
      }
    }

    for(int i = 0; i < BRANCH_RESERVATION_WIDTH; i++){
      if(reservationbru[i].rsource1ready == 0 && reservationbru[i].rsource1 == tagPassed){
        reservationbru[i].rsource1value = value;
        reservationbru[i].instruction.rsource1value = value;
        reservationbru[i].rsource1ready = 1;
      }
      if(reservationbru[i].rsource2ready == 0 && reservationbru[i].rsource2 == tagPassed){
        reservationbru[i].rsource2value = value;
        reservationbru[i].instruction.rsource2value = value;
        reservationbru[i].rsource2ready = 1;
        // printf("%d\n",value);
      }
    }

    for(int i = 0; i < STORE_RESERVATION_WIDTH; i++){

      if(reservationlsu[i].rsource1ready == 0 && reservationlsu[i].rsource1 == tagPassed){
        reservationlsu[i].rsource1value = value;
        reservationlsu[i].instruction.rsource1value = value;
        reservationlsu[i].rsource1ready = 1;
        // printf("%d\n",value);
      }
      if(reservationlsu[i].rsource2ready == 0 && reservationlsu[i].rsource2 == tagPassed){
        reservationlsu[i].rsource2value = value;
        reservationlsu[i].instruction.rsource2value = value;
        reservationlsu[i].rsource2ready = 1;
        // printf("%d\n",value);
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
      writebackalu[i].instruction = alu[i].instruction.instruction_hex;
      writebackalu[i].instructionid = alu[i].instruction.instructionid;
      writebackalu[i].instruction_type = alu[i].instruction.instruction_type;
      writebackalu[i].instruction_name = alu[i].instruction_name;
      // forward_reservation_stations(alu[i].destinationRegister, alu[i].valueInside, 1);
    }
  }
  for(int i = 0; i < BRU_NUM; i++){
    if(bru[i].readyForWriteback == 1){
      bru[i].readyForWriteback = 0;
      bru[i].shouldWriteback = 0;
      // writebackbru[i].value = bru[i].valueInside;
      writebackbru[i].value = bru[i].valueInside;
      writebackbru[i].ready = 1;
      writebackbru[i].instruction = bru[i].instruction.instruction_hex;
      writebackbru[i].instructionid = bru[i].instruction.instructionid;
      writebackbru[i].instruction_type = bru[i].instruction.instruction_type;
      writebackbru[i].instruction_name = bru[i].instruction_name;

      // forward_reservation_stations(alu[i].destinationRegister, alu[i].valueInside, 1);
    }
  }
  for(int i = 0; i < LSU_NUM; i++){
    if(lsu[i].readyForWriteback == 1){
      lsu[i].readyForWriteback = 0;
      lsu[i].shouldWriteback = 0;

      writebacklsu[i].ready = 1;
      writebacklsu[i].value = lsu[i].valueInside;
      writebacklsu[i].tag = lsu[i].destinationRegister;
      writebacklsu[i].instruction = lsu[i].instruction.instruction_hex;
      writebacklsu[i].instructionid = lsu[i].instruction.instructionid;
      writebacklsu[i].instruction_type = lsu[i].instruction.instruction_type;
      writebacklsu[i].instruction_name = lsu[i].instruction_name;


      // forward_reservation_stations(alu[i].destinationRegister, alu[i].valueInside, 1);
    }
  }
  for(int i = 0; i < JLU_NUM; i++){
    if(jlu[i].readyForWriteback == 1){
      jlu[i].readyForWriteback = 0;

      writebackjlu[i].ready = 1;
      writebackjlu[i].value = jlu[i].valueInside;
      writebackjlu[i].instruction = jlu[i].instruction.instruction_hex;
      writebackjlu[i].instructionid = jlu[i].instruction.instructionid;
      writebackjlu[i].instruction_type = jlu[i].instruction.instruction_type;
      writebackjlu[i].instruction_name = jlu[i].instruction_name;

    }
  }
  return;
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

availNum find_available_lsu(){
  int i;
  available.number = 0;
  for(i = 0; i < LSU_NUM; i++){
    if(lsu[i].ready == 1){
      available.unitNumber[available.number] = i;
      available.number++;
    }
  }
  return available;
}

availNum find_available_bru(){
  int i;
  available.number = 0;
  for(i = 0; i < BRU_NUM; i++){
    if(bru[i].ready == 1){
      available.unitNumber[available.number] = i;
      available.number++;
    }
  }
  return available;
}

//Fetches instructions from Reservation Stations
instructionwrapper check_reservation_alu(int numberOfAvailableUnits){
  instructionwrapper wrappedListofInstructions;
  wrappedListofInstructions.foundInstructions = 0;
  for(int i = 0; i < RESERVATION_WIDTH; i++){
    if(wrappedListofInstructions.foundInstructions == numberOfAvailableUnits){
      return wrappedListofInstructions;
    }
    if(reservationalu[i].rsource1ready && reservationalu[i].rsource2ready && reservationalu[i].inuse && reservationalu[i].inExecute){
      instruction newInstruction;
      newInstruction = reservationalu[i].instruction;
      wrappedListofInstructions.instruction[wrappedListofInstructions.foundInstructions] = newInstruction;
      wrappedListofInstructions.foundInstructions++;
      reservationalu[i].inuse = 0;
      reservationalu[i].inExecute = 0;

    }

  }

  return wrappedListofInstructions;
}

//Fetches instructions from Reservation Stations
instructionwrapper check_reservation_bru(int numberOfAvailableUnits){
  // instructionwrapper wrappedListofInstructions;
  // wrappedListofInstructions.foundInstructions = 0;
  // if(numberOfAvailableUnits == 0)return wrappedListofInstructions;
  // for(int i = 0; i < BRANCH_RESERVATION_WIDTH; i++){
  //   if(reservationbru[i].rsource1ready && reservationbru[i].rsource2ready && reservationbru[i].inuse && reservationbru[i].inExecute){
  //     if(wrappedListofInstructions.foundInstructions == numberOfAvailableUnits){
  //       instruction newInstruction;
  //       newInstruction = reservationbru[i].instruction;
  //       for(int j = 0; j < numberOfAvailableUnits; j++){
  //         if(newInstruction.instructionid < wrappedListofInstructions.instruction[j].instructionid){
  //           int x = wrappedListofInstructions.instruction[j].index;
  //           newInstruction.index = i;
  //           wrappedListofInstructions.instruction[j] = newInstruction;
  //           reservationbru[i].inuse = 0;
  //           reservationbru[i].inExecute = 0;
  //           reservationbru[x].inuse = 1;
  //           reservationbru[x].inExecute = 1;
  //
  //         }
  //       }
  //     }
  //     else{
  //       instruction newInstruction;
  //       newInstruction = reservationbru[i].instruction;
  //       newInstruction.index = i;
  //       wrappedListofInstructions.instruction[wrappedListofInstructions.foundInstructions] = newInstruction;
  //       wrappedListofInstructions.foundInstructions++;
  //       reservationbru[i].inuse = 0;
  //       reservationbru[i].inExecute = 0;
  //   }
  //   }
  // }
  instructionwrapper wrappedListofInstructions;
  wrappedListofInstructions.foundInstructions = 0;
  if(numberOfAvailableUnits == 0)return wrappedListofInstructions;
  for(int i = 0; i < RESERVATION_WIDTH; i++){
    if(wrappedListofInstructions.foundInstructions == numberOfAvailableUnits){
      return wrappedListofInstructions;
    }
    if(reservationbru[i].rsource1ready && reservationbru[i].rsource2ready && reservationbru[i].inuse && reservationbru[i].inExecute){
      instruction newInstruction;
      newInstruction = reservationbru[i].instruction;
      wrappedListofInstructions.instruction[wrappedListofInstructions.foundInstructions] = newInstruction;
      wrappedListofInstructions.foundInstructions++;
      reservationbru[i].inuse = 0;
      reservationbru[i].inExecute = 0;

    }

  }
  return wrappedListofInstructions;
}

instructionwrapper check_reservation_lsu(int numberOfAvailableUnits){
  instructionwrapper wrappedListofInstructions;
  wrappedListofInstructions.foundInstructions = 0;
  instruction newInstruction;
  int first_time = 1;
  if(numberOfAvailableUnits == 0)return wrappedListofInstructions;
  for(int j = 0; j < numberOfAvailableUnits; j++){
    for(int i = 0; i < STORE_RESERVATION_WIDTH; i++){
      if(reservationlsu[i].inuse && reservationlsu[i].inExecute && reservationlsu[i].notExecuted){
        if(first_time == 1){
          first_time = 0;
          newInstruction = reservationlsu[i].instruction;
          newInstruction.index = i;
        }
        else{
          if(newInstruction.instructionid > reservationlsu[i].instruction.instructionid){
            newInstruction = reservationlsu[i].instruction;
            newInstruction.index = i;
          }
        }
      }
    }
    if(first_time != 1){
      wrappedListofInstructions.instruction[j] = newInstruction;
      first_time = 1;
      if(reservationlsu[newInstruction.index].rsource1ready && reservationlsu[newInstruction.index].rsource2ready){
        wrappedListofInstructions.foundInstructions++;
        if(reservationlsu[newInstruction.index].instruction_type == 6){
          reservationlsu[newInstruction.index].inuse = 1;
          reservationlsu[newInstruction.index].inExecute = 1;
        }
        else{
          reservationlsu[newInstruction.index].inuse = 0;
          reservationlsu[newInstruction.index].inExecute = 0;
        }
        reservationlsu[newInstruction.index].notExecuted = 0;

      }
    }
    else break;
  }
  return wrappedListofInstructions;
}

void execute(){

  if(first_issue < 2 && continue_execute == 0){
    print_execute_summary = 0;
    return;
  }
  if(first_execute < 2){
    continue_execute = 1;
    first_execute++;
  }
  if(last_instruction == 1){
    if(current_cycle > last_instruction_cycle + 3){
      print_execute_summary = 0;
    }
  }
  numberOfExecutedInstructions = 0;
  print_execute_summary = 1;
  availNum currentAvailable = find_available_alu();
  instructionwrapper currentInstructions = check_reservation_alu(currentAvailable.number);


  for(int i = 0; i < currentInstructions.foundInstructions; i++){
    int alu_unit_number = currentAvailable.unitNumber[i];
    currentInstruction = currentInstructions.instruction[i];

    alu[alu_unit_number].ready = 0;
    alu[alu_unit_number].destinationRegister = currentInstruction.tagDestination;

    if(currentInstruction.instruction_type == 1){
      execute_iformat(alu_unit_number);
      numberOfExecutedInstructions++;
    }
    else if(currentInstruction.instruction_type == 2){
      execute_uformat(alu_unit_number);
      numberOfExecutedInstructions++;
    }
    else if(currentInstruction.instruction_type == 3){
      execute_rformat(alu_unit_number);
      numberOfExecutedInstructions++;
    }
    else if(currentInstruction.instruction_type == 4){
      // execute_jformat(alu_unit_number);
    }
  }

  currentAvailable = find_available_bru();
  currentInstructions = check_reservation_bru(currentAvailable.number);

  for(int i = 0; i < currentInstructions.foundInstructions; i++){
    int bru_unit_number = currentAvailable.unitNumber[i];
    currentInstruction = currentInstructions.instruction[i];

    bru[bru_unit_number].ready = 0;
    bru[bru_unit_number].destinationRegister = currentInstruction.tagDestination;

    if(currentInstruction.instruction_type == 5){
      execute_bformat(bru_unit_number);
      numberOfExecutedInstructions++;
    }
  }

  currentAvailable = find_available_lsu();
  currentInstructions = check_reservation_lsu(currentAvailable.number);

  for(int i = 0; i < currentInstructions.foundInstructions; i++){
    int lsu_unit_number = currentAvailable.unitNumber[i];
    currentInstruction = currentInstructions.instruction[i];

    lsu[lsu_unit_number].ready = 0;
    lsu[lsu_unit_number].destinationRegister = currentInstruction.tagDestination;
    lsu[lsu_unit_number].instruction = currentInstruction;
    if(currentInstruction.instruction_type == 1){
      execute_iformat(lsu_unit_number);
      numberOfExecutedInstructions++;
    }
    else if(currentInstruction.instruction_type == 6){
      execute_sformat(lsu_unit_number);
      numberOfExecutedInstructions++;
    }
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
    current = 0;
    // stall_from_issue = 0;
    purgepipe();
  }

  if(issue_finished == 1 && execute_finished != 1 && current_cycle > last_instruction_cycle + 3){
    execute_finished = check_execute_and_reservation_units();
    if(execute_finished){
      execute_cycle_finished = current_cycle;
    }
  }
  return;
}
