#include "run.h"

int tagIterator = 0;
int reservationIteratorALU = 0;
int reservationIteratorLSU = 0;
int reservationIteratorBRU = 0;

int issue_isPhys;
int issue_isPhys2;

int stop = 0;

int instructionid = 0;
int stall_from_issue = 0;
int stall_rename = 1;

void issue_rename(){

  if(issue_instruction_struct.instruction_type  == 1 || issue_instruction_struct.instruction_type == 2 || issue_instruction_struct.instruction_type == 3){
    issue_instruction_struct.tagDestination = movenode(unusedTags, inuseTags, issue_instruction_struct.rdestination);
    if(issue_instruction_struct.tagDestination == -1){
      stall_from_issue = 2;
      stall_rename = 1;
      return;
    }
    else{
      stall_rename = 0;
    }
    physRegisters[issue_instruction_struct.tagDestination].ready = 0;
  }
  else{
    issue_instruction_struct.tagDestination = -1;
  }

  // if(issue_instruction_struct.instruction_type == 4){
  //   issue_instruction_struct.pctag = movenode(unusedTags, inuseTags, issue_instruction_struct.pcDestination);
  // }

  find foundtag = find_register_last(inuseTags, issue_instruction_struct.rsource1);

  if(foundtag.found == 1){
    issue_isPhys = 1;
    issue_instruction_struct.tagsource1 = foundtag.number;
  }
  else{
    issue_isPhys = 0;
  }

  if(issue_instruction_struct.instruction_type == 3 || issue_instruction_struct.instruction_type == 5 || issue_instruction_struct.instruction_type == 6){
    foundtag = find_register_last(inuseTags, issue_instruction_struct.rsource2);
    if(foundtag.found == 1){
      issue_isPhys2 = 1;
      issue_instruction_struct.tagsource2 = foundtag.number;
      // physRegisters[issue_instruction_struct.tagsource2].ready = 0;
    }
    else{
      issue_isPhys2 = 0;
    }
  }

  return;
}

void issue_add_to_reservation(){
  find foundtag;
  if(issue_unit_type == 1){
    if(reservationIteratorALU >= RESERVATION_WIDTH)reservationIteratorALU = 0;
    int i;
    for(i = 0; i < RESERVATION_WIDTH; i++){
      if(reservationalu[i].inuse == 1){
        continue;
      }
      else{
        reservationIteratorALU = i;
        break;
      }
    }
    if(i == RESERVATION_WIDTH){
      stall_from_issue = 2;
      return;
    }

    reservationalu[reservationIteratorALU].rdestination = issue_instruction_struct.tagDestination;
    reservationalu[reservationIteratorALU].rsource1 = issue_instruction_struct.tagsource1;

    if(issue_isPhys == 0){
      issue_instruction_struct.rsource1value = registers[issue_instruction_struct.rsource1];
      reservationalu[reservationIteratorALU].rsource1value = registers[issue_instruction_struct.rsource1];
      reservationalu[reservationIteratorALU].rsource1ready = 1;
    }
    else{

      if(physRegisters[issue_instruction_struct.tagsource1].ready == 1){
        issue_instruction_struct.rsource1value = physRegisters[issue_instruction_struct.tagsource1].value;
        reservationalu[reservationIteratorALU].rsource1value = physRegisters[issue_instruction_struct.tagsource1].value;
        reservationalu[reservationIteratorALU].rsource1ready = 1;
      }
      else{

        if(issue_instruction_struct.tagsource1 == issue_instruction_struct.tagDestination){
          foundtag = second_last(inuseTags, issue_instruction_struct.rsource1);

          if(foundtag.found == 1){
            reservationalu[reservationIteratorALU].rsource1 = foundtag.number;
            issue_instruction_struct.tagsource1 = foundtag.number;
            if(physRegisters[foundtag.number].ready){
              reservationalu[reservationIteratorALU].rsource1value = physRegisters[foundtag.number].value;
              issue_instruction_struct.rsource1value = physRegisters[foundtag.number].value;
              reservationalu[reservationIteratorALU].rsource1ready = 1;
            }
            else reservationalu[reservationIteratorALU].rsource1ready = 0;
          }
          else{
            reservationalu[reservationIteratorALU].rsource1ready = 1;
            reservationalu[reservationIteratorALU].rsource1 = issue_instruction_struct.rsource1;
            reservationalu[reservationIteratorALU].rsource1value = registers[issue_instruction_struct.rsource1];
            issue_instruction_struct.rsource1value = registers[issue_instruction_struct.rsource1];
          }
        }
        else{
          reservationalu[reservationIteratorALU].rsource1ready = 0;
        }
      }
    }

    if(issue_instruction_struct.instruction_type == 3 || issue_instruction_struct.instruction_type == 5 || issue_instruction_struct.instruction_type == 6){
      reservationalu[reservationIteratorALU].rsource2 = issue_instruction_struct.tagsource2;
      if(issue_isPhys2 == 0){
        reservationalu[reservationIteratorALU].rsource2 = issue_instruction_struct.rsource2;

        issue_instruction_struct.rsource2value = registers[issue_instruction_struct.rsource2];
        reservationalu[reservationIteratorALU].rsource2value = registers[issue_instruction_struct.rsource2];
        reservationalu[reservationIteratorALU].rsource2ready = 1;
      }
      else{
        if(physRegisters[issue_instruction_struct.tagsource2].ready == 1){
          issue_instruction_struct.rsource2value = physRegisters[issue_instruction_struct.tagsource2].value;
          reservationalu[reservationIteratorALU].rsource2 = issue_instruction_struct.tagsource2;
          reservationalu[reservationIteratorALU].rsource2value = physRegisters[issue_instruction_struct.tagsource2].value;
          reservationalu[reservationIteratorALU].rsource2ready = 1;
        }
        else{
          if(issue_instruction_struct.tagsource2 == issue_instruction_struct.tagDestination){
            foundtag = second_last(inuseTags, issue_instruction_struct.rsource2);
            if(foundtag.found == 1){
              reservationalu[reservationIteratorALU].rsource2 = foundtag.number;
              issue_instruction_struct.tagsource2 = foundtag.number;

              if(physRegisters[foundtag.number].ready){
                reservationalu[reservationIteratorALU].rsource2value = physRegisters[foundtag.number].value;
                issue_instruction_struct.rsource2value = physRegisters[foundtag.number].value;
                reservationalu[reservationIteratorALU].rsource2ready = 1;
              }
              else reservationalu[reservationIteratorALU].rsource2ready = 0;
            }
            else{
              reservationalu[reservationIteratorALU].rsource2ready = 1;
              reservationalu[reservationIteratorALU].rsource2 = issue_instruction_struct.rsource2;
              reservationalu[reservationIteratorALU].rsource2value = registers[issue_instruction_struct.rsource2];
              issue_instruction_struct.rsource2value = registers[issue_instruction_struct.rsource2];
            }
          }
          else{
            reservationalu[reservationIteratorALU].rsource2ready = 0;
          }
        }
    }
  }
    else{
      reservationalu[reservationIteratorALU].rsource2 = -1;
      reservationalu[reservationIteratorALU].rsource2ready = 1;
    }

    reservationalu[reservationIteratorALU].opcode = issue_instruction_struct.opcode;
    reservationalu[reservationIteratorALU].funct3 = issue_instruction_struct.funct3;
    reservationalu[reservationIteratorALU].funct7 = issue_instruction_struct.funct7;
    reservationalu[reservationIteratorALU].shamt = issue_instruction_struct.shamt;
    reservationalu[reservationIteratorALU].imm = issue_instruction_struct.imm;
    reservationalu[reservationIteratorALU].pc = issue_instruction_struct.pc;
    reservationalu[reservationIteratorALU].instruction_type = issue_instruction_struct.instruction_type;
    reservationalu[reservationIteratorALU].inuse = 1;
    reservationalu[reservationIteratorALU].inExecute = 0;
    reservationalu[reservationIteratorALU].instruction_hex = issue_instruction_struct.instruction_hex;
    issue_instruction_struct.instructionid = instructionid;
    reservationalu[reservationIteratorALU].instruction = issue_instruction_struct;

    reservationalu[reservationIteratorALU].instructionid = issue_instruction_struct.instructionid;
    reservationIteratorALU++;

    tag tagData;
    tagData.tagNumber = issue_instruction_struct.tagDestination;
    tagData.registerNumber = issue_instruction_struct.rdestination;

    addafternodeinstruction(inOrderInstructions, issue_instruction_struct.instruction_hex, instructionid, tagData);
    // printf("%d\n",tagData.registerNumber);
    // if(instructionid == 6)exit_early();
    instructionid++;

}

if(issue_unit_type == 3){
  if(reservationIteratorBRU >= BRANCH_RESERVATION_WIDTH)reservationIteratorBRU = 0;
  int i;
  for(i = 0; i < BRANCH_RESERVATION_WIDTH; i++){
    if(reservationbru[i].inuse == 1){
      continue;
    }
    else{
      reservationIteratorBRU = i;
      break;
    }
  }
  if(i == BRANCH_RESERVATION_WIDTH){
    stall_from_issue = 2;
    return;
  }

  reservationbru[reservationIteratorBRU].rdestination = issue_instruction_struct.tagDestination;
  reservationbru[reservationIteratorBRU].rsource1 = issue_instruction_struct.tagsource1;

  if(issue_isPhys == 0){
    issue_instruction_struct.rsource1value = registers[issue_instruction_struct.rsource1];
    reservationbru[reservationIteratorBRU].rsource1value = registers[issue_instruction_struct.rsource1];
    reservationbru[reservationIteratorBRU].rsource1ready = 1;
  }
  else{

    if(physRegisters[issue_instruction_struct.tagsource1].ready == 1){
      issue_instruction_struct.rsource1value = physRegisters[issue_instruction_struct.tagsource1].value;
      reservationbru[reservationIteratorBRU].rsource1value = physRegisters[issue_instruction_struct.tagsource1].value;
      reservationbru[reservationIteratorBRU].rsource1ready = 1;
    }
    else{
      if(issue_instruction_struct.tagsource1 == issue_instruction_struct.tagDestination){
        foundtag = second_last(inuseTags, issue_instruction_struct.rsource1);
        if(foundtag.found == 1){
          reservationbru[reservationIteratorBRU].rsource1 = foundtag.number;
          issue_instruction_struct.tagsource1 = foundtag.number;
          if(physRegisters[foundtag.number].ready){
            reservationbru[reservationIteratorBRU].rsource1value = physRegisters[foundtag.number].value;
            issue_instruction_struct.rsource1value = physRegisters[foundtag.number].value;
            reservationbru[reservationIteratorBRU].rsource1ready = 1;
          }
          else reservationbru[reservationIteratorBRU].rsource1ready = 0;
        }
        else{
          reservationbru[reservationIteratorBRU].rsource1ready = 1;
          reservationbru[reservationIteratorBRU].rsource1 = issue_instruction_struct.rsource1;
          reservationbru[reservationIteratorBRU].rsource1value = registers[issue_instruction_struct.rsource1];
          issue_instruction_struct.rsource1value = registers[issue_instruction_struct.rsource1];
        }
      }
      else{
        reservationbru[reservationIteratorBRU].rsource1ready = 0;
      }
    }
  }

  if(issue_instruction_struct.instruction_type == 3 || issue_instruction_struct.instruction_type == 5 || issue_instruction_struct.instruction_type == 6){
    reservationbru[reservationIteratorBRU].rsource2 = issue_instruction_struct.tagsource2;
    if(issue_isPhys2 == 0){
      reservationbru[reservationIteratorBRU].rsource2 = issue_instruction_struct.rsource2;

      issue_instruction_struct.rsource2value = registers[issue_instruction_struct.rsource2];
      reservationbru[reservationIteratorBRU].rsource2value = registers[issue_instruction_struct.rsource2];
      reservationbru[reservationIteratorBRU].rsource2ready = 1;
    }
    else{
      if(physRegisters[issue_instruction_struct.tagsource2].ready == 1){
        issue_instruction_struct.rsource2value = physRegisters[issue_instruction_struct.tagsource2].value;
        reservationbru[reservationIteratorBRU].rsource2 = issue_instruction_struct.tagsource2;
        reservationbru[reservationIteratorBRU].rsource2value = physRegisters[issue_instruction_struct.tagsource2].value;
        reservationbru[reservationIteratorBRU].rsource2ready = 1;
      }
      else{
        if(issue_instruction_struct.tagsource2 == issue_instruction_struct.tagDestination){
          foundtag = second_last(inuseTags, issue_instruction_struct.rsource2);
          if(foundtag.found == 1){
            reservationbru[reservationIteratorBRU].rsource2 = foundtag.number;
            issue_instruction_struct.tagsource2 = foundtag.number;
            if(physRegisters[foundtag.number].ready){
              reservationbru[reservationIteratorBRU].rsource2value = physRegisters[foundtag.number].value;
              issue_instruction_struct.rsource2value = physRegisters[foundtag.number].value;
              reservationbru[reservationIteratorBRU].rsource2ready = 1;
            }
            else reservationbru[reservationIteratorBRU].rsource2ready = 0;
          }
          else{
            reservationbru[reservationIteratorBRU].rsource2ready = 1;
            reservationbru[reservationIteratorBRU].rsource2 = issue_instruction_struct.rsource2;
            reservationbru[reservationIteratorBRU].rsource2value = registers[issue_instruction_struct.rsource2];
            issue_instruction_struct.rsource2value = registers[issue_instruction_struct.rsource2];
          }
        }
        else{
          reservationbru[reservationIteratorBRU].rsource2ready = 0;
        }
      }
  }
}
  else{
    reservationbru[reservationIteratorBRU].rsource2 = -1;
    reservationbru[reservationIteratorBRU].rsource2ready = 1;
  }

  reservationbru[reservationIteratorBRU].opcode = issue_instruction_struct.opcode;
  reservationbru[reservationIteratorBRU].funct3 = issue_instruction_struct.funct3;
  reservationbru[reservationIteratorBRU].funct7 = issue_instruction_struct.funct7;
  reservationbru[reservationIteratorBRU].shamt = issue_instruction_struct.shamt;
  reservationbru[reservationIteratorBRU].imm = issue_instruction_struct.imm;
  reservationbru[reservationIteratorBRU].pc = issue_instruction_struct.pc;
  reservationbru[reservationIteratorBRU].instruction_type = issue_instruction_struct.instruction_type;
  reservationbru[reservationIteratorBRU].inuse = 1;
  reservationbru[reservationIteratorBRU].inExecute = 0;
  reservationbru[reservationIteratorBRU].instruction_hex = issue_instruction_struct.instruction_hex;
  issue_instruction_struct.instructionid = instructionid;
  reservationbru[reservationIteratorBRU].instruction = issue_instruction_struct;

  reservationbru[reservationIteratorBRU].instructionid = issue_instruction_struct.instructionid;
  reservationIteratorBRU++;

  tag tagData;
  tagData.tagNumber = -2;
  tagData.registerNumber = issue_instruction_struct.rdestination;
  addafternodeinstruction(inOrderInstructions, issue_instruction_struct.instruction_hex, instructionid, tagData);
  instructionid++;
  // if(instructionid == 22)exit_early();
}

if(issue_unit_type == 2){
  if(reservationIteratorLSU >= STORE_RESERVATION_WIDTH)reservationIteratorLSU = 0;
  int i;
  for(i = 0; i < STORE_RESERVATION_WIDTH; i++){
    if(reservationlsu[i].inuse == 1){
      continue;
    }
    else{
      reservationIteratorLSU = i;
      break;
    }
  }
  if(i == STORE_RESERVATION_WIDTH){
    stall_from_issue = 2;
    return;
  }

  reservationlsu[reservationIteratorLSU].rdestination = issue_instruction_struct.tagDestination;
  reservationlsu[reservationIteratorLSU].rsource1 = issue_instruction_struct.tagsource1;

  if(issue_isPhys == 0){
    issue_instruction_struct.rsource1value = registers[issue_instruction_struct.rsource1];
    reservationlsu[reservationIteratorLSU].rsource1value = registers[issue_instruction_struct.rsource1];
    reservationlsu[reservationIteratorLSU].rsource1ready = 1;
  }
  else{

    if(physRegisters[issue_instruction_struct.tagsource1].ready == 1){
      issue_instruction_struct.rsource1value = physRegisters[issue_instruction_struct.tagsource1].value;
      reservationlsu[reservationIteratorLSU].rsource1value = physRegisters[issue_instruction_struct.tagsource1].value;
      reservationlsu[reservationIteratorLSU].rsource1ready = 1;
    }
    else{
      if(issue_instruction_struct.tagsource1 == issue_instruction_struct.tagDestination){
        foundtag = second_last(inuseTags, issue_instruction_struct.rsource1);
        if(foundtag.found == 1){
          reservationlsu[reservationIteratorLSU].rsource1 = foundtag.number;
          issue_instruction_struct.tagsource1 = foundtag.number;
          if(physRegisters[foundtag.number].ready){
            reservationlsu[reservationIteratorLSU].rsource1value = physRegisters[foundtag.number].value;
            issue_instruction_struct.rsource1value = physRegisters[foundtag.number].value;
            reservationlsu[reservationIteratorLSU].rsource1ready = 1;
          }
          else reservationlsu[reservationIteratorLSU].rsource1ready = 0;
        }
        else{
          reservationlsu[reservationIteratorLSU].rsource1ready = 1;
          reservationlsu[reservationIteratorLSU].rsource1 = issue_instruction_struct.rsource1;
          reservationlsu[reservationIteratorLSU].rsource1value = registers[issue_instruction_struct.rsource1];
          issue_instruction_struct.rsource1value = registers[issue_instruction_struct.rsource1];
        }
      }
      else{
        reservationlsu[reservationIteratorLSU].rsource1ready = 0;
      }
    }
  }

  if(issue_instruction_struct.instruction_type == 3 || issue_instruction_struct.instruction_type == 5 || issue_instruction_struct.instruction_type == 6){
    reservationlsu[reservationIteratorLSU].rsource2 = issue_instruction_struct.tagsource2;
    if(issue_isPhys2 == 0){
      reservationlsu[reservationIteratorLSU].rsource2 = issue_instruction_struct.rsource2;

      issue_instruction_struct.rsource2value = registers[issue_instruction_struct.rsource2];
      reservationlsu[reservationIteratorLSU].rsource2value = registers[issue_instruction_struct.rsource2];
      reservationlsu[reservationIteratorLSU].rsource2ready = 1;
    }
    else{
      if(physRegisters[issue_instruction_struct.tagsource2].ready == 1){
        issue_instruction_struct.rsource2value = physRegisters[issue_instruction_struct.tagsource2].value;
        reservationlsu[reservationIteratorLSU].rsource2 = issue_instruction_struct.tagsource2;
        reservationlsu[reservationIteratorLSU].rsource2value = physRegisters[issue_instruction_struct.tagsource2].value;
        reservationlsu[reservationIteratorLSU].rsource2ready = 1;
      }
      else{
        if(issue_instruction_struct.tagsource2 == issue_instruction_struct.tagDestination){
          foundtag = second_last(inuseTags, issue_instruction_struct.rsource2);
          if(foundtag.found == 1){
            reservationlsu[reservationIteratorLSU].rsource2 = foundtag.number;
            issue_instruction_struct.tagsource2 = foundtag.number;
            if(physRegisters[foundtag.number].ready){
              reservationlsu[reservationIteratorLSU].rsource2value = physRegisters[foundtag.number].value;
              issue_instruction_struct.rsource2value = physRegisters[foundtag.number].value;
              reservationlsu[reservationIteratorLSU].rsource2ready = 1;
            }
            else reservationlsu[reservationIteratorLSU].rsource2ready = 0;
          }
          else{
            reservationlsu[reservationIteratorLSU].rsource2ready = 1;
            reservationlsu[reservationIteratorLSU].rsource2 = issue_instruction_struct.rsource2;
            reservationlsu[reservationIteratorLSU].rsource2value = registers[issue_instruction_struct.rsource2];
            issue_instruction_struct.rsource2value = registers[issue_instruction_struct.rsource2];
          }
        }
        else{
          reservationlsu[reservationIteratorLSU].rsource2ready = 0;
        }
      }
  }
}
  else{
    reservationlsu[reservationIteratorLSU].rsource2 = -1;
    reservationlsu[reservationIteratorLSU].rsource2ready = 1;
  }

  reservationlsu[reservationIteratorLSU].opcode = issue_instruction_struct.opcode;
  reservationlsu[reservationIteratorLSU].funct3 = issue_instruction_struct.funct3;
  reservationlsu[reservationIteratorLSU].funct7 = issue_instruction_struct.funct7;
  reservationlsu[reservationIteratorLSU].shamt = issue_instruction_struct.shamt;
  reservationlsu[reservationIteratorLSU].imm = issue_instruction_struct.imm;
  reservationlsu[reservationIteratorLSU].pc = issue_instruction_struct.pc;
  reservationlsu[reservationIteratorLSU].instruction_type = issue_instruction_struct.instruction_type;
  reservationlsu[reservationIteratorLSU].inuse = 1;
  reservationlsu[reservationIteratorLSU].inExecute = 0;
  reservationlsu[reservationIteratorLSU].instruction_hex = issue_instruction_struct.instruction_hex;
  issue_instruction_struct.instructionid = instructionid;
  reservationlsu[reservationIteratorLSU].instruction = issue_instruction_struct;

  reservationlsu[reservationIteratorLSU].instructionid = issue_instruction_struct.instructionid;
  reservationIteratorLSU++;

  tag tagData;
  if(issue_instruction_struct.instruction_type == 1){
    tagData.tagNumber = issue_instruction_struct.tagDestination;
    tagData.registerNumber = issue_instruction_struct.rdestination;
  }
  else{
    tagData.tagNumber = -2;
    tagData.registerNumber = -2;
  }

  addafternodeinstruction(inOrderInstructions, issue_instruction_struct.instruction_hex, instructionid, tagData);
  instructionid++;
}

  return;

}

void issue(){
  if(stall_from_issue == 0){
    if(issue_finished)return;
    if(pc[0] == 0){
      if(current_cycle > last_instruction_cycle + 2){
        issue_finished = 1;
        return;
      }
    }
  if(first_decode < 2){
    print_issue_summary = 0;
    return;
  }

  if(first_issue < 2)first_issue++;

  if(issue_instruction_struct.instruction_type == 0){
    return;
  }

  // printf("%d\n", issue_instruction_struct.instruction_type);
  if(issue_instruction_struct.instruction_type == 4 && issue_instruction_struct.rdestination == 0 && pc[0] != 0){
    issue_instruction_struct.instruction_type = 0;
    pc[0] = issue_instruction_struct.pc + issue_instruction_struct.imm;

    instructions_executed++;

    if(pc[0] == 0){
      // issue_finished = 1;
    }
    else{

      flush_from_issue = 1;
    }

    return;
  }

  print_issue_summary = 1;

  issue_rename();
  if(stall_rename == 1)return;

  issue_add_to_reservation();

  }
  else if(stall_rename){
    if(issue_instruction_struct.instruction_type == 0){
      return;
    }

    // printf("%d\n", issue_instruction_struct.instruction_type);
    if(issue_instruction_struct.instruction_type == 4 && issue_instruction_struct.rdestination == 0 && pc[0] != 0){
      issue_instruction_struct.instruction_type = 0;
      pc[0] = issue_instruction_struct.pc + issue_instruction_struct.imm;

      instructions_executed++;

      if(pc[0] == 0){
        // issue_finished = 1;
      }
      else{

        flush_from_issue = 1;
      }
      printf("INSTRUCTIONS EXECUTED FROM ISSUE:%d\n\n\n\n", instructions_executed);

      return;
    }
    issue_rename();
    if(stall_rename == 1)return;

    issue_add_to_reservation();
  }
  else{
    if(issue_instruction_struct.instruction_type == 0){
      return;
    }

    // printf("%d\n", issue_instruction_struct.instruction_type);
    if(issue_instruction_struct.instruction_type == 4 && issue_instruction_struct.rdestination == 0 && pc[0] != 0){
      issue_instruction_struct.instruction_type = 0;
      pc[0] = issue_instruction_struct.pc + issue_instruction_struct.imm;

      instructions_executed++;

      if(pc[0] == 0){
        // issue_finished = 1;
      }
      else{

        flush_from_issue = 1;
      }
      printf("INSTRUCTIONS EXECUTED FROM ISSUE:%d\n\n\n\n", instructions_executed);

      return;
    }
    issue_add_to_reservation();
  }
  return;
}
