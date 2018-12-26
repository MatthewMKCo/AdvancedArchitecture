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
int stall_rename = 0;
int current = 0;
int block_decode_to_issue = 0;
int b4 = 0;
void issue_rename(int current){
  if(issue_instruction_struct[current].instruction_type  == 1 || issue_instruction_struct[current].instruction_type == 2 || issue_instruction_struct[current].instruction_type == 3){
    issue_instruction_struct[current].tagDestination = movenode(unusedTags, inuseTags, issue_instruction_struct[current].rdestination, instructionid, 0);
    if(issue_instruction_struct[current].tagDestination == -1){
      stall_from_issue = 2;
      stall_rename = 1;
      return;
    }
    else{
      stall_rename = 0;
    }
    physRegisters[issue_instruction_struct[current].tagDestination].ready = 0;
  }
  else{
    issue_instruction_struct[current].tagDestination = -1;
  }

  find foundtag = find_register_last(inuseTags, issue_instruction_struct[current].rsource1);

  if(foundtag.found == 1){
    issue_isPhys = 1;
    issue_instruction_struct[current].tagsource1 = foundtag.number;
  }
  else{
    issue_isPhys = 0;
  }

  if(issue_instruction_struct[current].instruction_type == 3 || issue_instruction_struct[current].instruction_type == 5 || issue_instruction_struct[current].instruction_type == 6){
    foundtag = find_register_last(inuseTags, issue_instruction_struct[current].rsource2);
    if(foundtag.found == 1){
      issue_isPhys2 = 1;
      issue_instruction_struct[current].tagsource2 = foundtag.number;
      // physRegisters[issue_instruction_struct[current].tagsource2].ready = 0;
    }
    else{
      issue_isPhys2 = 0;
    }
  }

  stall_from_issue = 0;
  stall_rename = 0;

  return;
}

void issue_add_to_reservation(int current){
  find foundtag;
  if(issue_unit_type[current] == 1){
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

    reservationalu[reservationIteratorALU].rdestination = issue_instruction_struct[current].tagDestination;
    reservationalu[reservationIteratorALU].rsource1 = issue_instruction_struct[current].tagsource1;

    if(issue_isPhys == 0){
      issue_instruction_struct[current].rsource1value = registers[issue_instruction_struct[current].rsource1];
      reservationalu[reservationIteratorALU].rsource1value = registers[issue_instruction_struct[current].rsource1];
      reservationalu[reservationIteratorALU].rsource1ready = 1;
    }
    else{

      if(physRegisters[issue_instruction_struct[current].tagsource1].ready == 1){
        issue_instruction_struct[current].rsource1value = physRegisters[issue_instruction_struct[current].tagsource1].value;
        reservationalu[reservationIteratorALU].rsource1value = physRegisters[issue_instruction_struct[current].tagsource1].value;
        reservationalu[reservationIteratorALU].rsource1ready = 1;
      }
      else{

        if(issue_instruction_struct[current].tagsource1 == issue_instruction_struct[current].tagDestination){
          foundtag = second_last(inuseTags, issue_instruction_struct[current].rsource1);

          if(foundtag.found == 1){
            reservationalu[reservationIteratorALU].rsource1 = foundtag.number;
            issue_instruction_struct[current].tagsource1 = foundtag.number;
            if(physRegisters[foundtag.number].ready){
              reservationalu[reservationIteratorALU].rsource1value = physRegisters[foundtag.number].value;
              issue_instruction_struct[current].rsource1value = physRegisters[foundtag.number].value;
              reservationalu[reservationIteratorALU].rsource1ready = 1;
            }
            else reservationalu[reservationIteratorALU].rsource1ready = 0;
          }
          else{
            reservationalu[reservationIteratorALU].rsource1ready = 1;
            reservationalu[reservationIteratorALU].rsource1 = issue_instruction_struct[current].rsource1;
            reservationalu[reservationIteratorALU].rsource1value = registers[issue_instruction_struct[current].rsource1];
            issue_instruction_struct[current].rsource1value = registers[issue_instruction_struct[current].rsource1];
          }
        }
        else{
          reservationalu[reservationIteratorALU].rsource1ready = 0;
        }
      }
    }

    if(issue_instruction_struct[current].instruction_type == 3 || issue_instruction_struct[current].instruction_type == 5 || issue_instruction_struct[current].instruction_type == 6){
      reservationalu[reservationIteratorALU].rsource2 = issue_instruction_struct[current].tagsource2;
      if(issue_isPhys2 == 0){
        reservationalu[reservationIteratorALU].rsource2 = issue_instruction_struct[current].rsource2;

        issue_instruction_struct[current].rsource2value = registers[issue_instruction_struct[current].rsource2];
        reservationalu[reservationIteratorALU].rsource2value = registers[issue_instruction_struct[current].rsource2];
        reservationalu[reservationIteratorALU].rsource2ready = 1;
      }
      else{
        if(physRegisters[issue_instruction_struct[current].tagsource2].ready == 1){
          issue_instruction_struct[current].rsource2value = physRegisters[issue_instruction_struct[current].tagsource2].value;
          reservationalu[reservationIteratorALU].rsource2 = issue_instruction_struct[current].tagsource2;
          reservationalu[reservationIteratorALU].rsource2value = physRegisters[issue_instruction_struct[current].tagsource2].value;
          reservationalu[reservationIteratorALU].rsource2ready = 1;
        }
        else{
          if(issue_instruction_struct[current].tagsource2 == issue_instruction_struct[current].tagDestination){
            foundtag = second_last(inuseTags, issue_instruction_struct[current].rsource2);
            if(foundtag.found == 1){
              reservationalu[reservationIteratorALU].rsource2 = foundtag.number;
              issue_instruction_struct[current].tagsource2 = foundtag.number;

              if(physRegisters[foundtag.number].ready){
                reservationalu[reservationIteratorALU].rsource2value = physRegisters[foundtag.number].value;
                issue_instruction_struct[current].rsource2value = physRegisters[foundtag.number].value;
                reservationalu[reservationIteratorALU].rsource2ready = 1;
              }
              else reservationalu[reservationIteratorALU].rsource2ready = 0;
            }
            else{
              reservationalu[reservationIteratorALU].rsource2ready = 1;
              reservationalu[reservationIteratorALU].rsource2 = issue_instruction_struct[current].rsource2;
              reservationalu[reservationIteratorALU].rsource2value = registers[issue_instruction_struct[current].rsource2];
              issue_instruction_struct[current].rsource2value = registers[issue_instruction_struct[current].rsource2];
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

    reservationalu[reservationIteratorALU].opcode = issue_instruction_struct[current].opcode;
    reservationalu[reservationIteratorALU].funct3 = issue_instruction_struct[current].funct3;
    reservationalu[reservationIteratorALU].funct7 = issue_instruction_struct[current].funct7;
    reservationalu[reservationIteratorALU].shamt = issue_instruction_struct[current].shamt;
    reservationalu[reservationIteratorALU].imm = issue_instruction_struct[current].imm;
    reservationalu[reservationIteratorALU].pc = issue_instruction_struct[current].pc;
    reservationalu[reservationIteratorALU].instruction_type = issue_instruction_struct[current].instruction_type;
    reservationalu[reservationIteratorALU].inuse = 1;
    reservationalu[reservationIteratorALU].inExecute = 0;
    reservationalu[reservationIteratorALU].instruction_hex = issue_instruction_struct[current].instruction_hex;
    issue_instruction_struct[current].instructionid = instructionid;
    reservationalu[reservationIteratorALU].instruction = issue_instruction_struct[current];

    reservationalu[reservationIteratorALU].instructionid = issue_instruction_struct[current].instructionid;
    reservationIteratorALU++;

    tag tagData;
    tagData.tagNumber = issue_instruction_struct[current].tagDestination;
    tagData.registerNumber = issue_instruction_struct[current].rdestination;

    addafternodeinstruction(inOrderInstructions, issue_instruction_struct[current].instruction_hex, instructionid, tagData, issue_unit_type[current], 0, 0);
    instructionid++;

}

if(issue_unit_type[current] == 3){
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

  reservationbru[reservationIteratorBRU].rdestination = issue_instruction_struct[current].tagDestination;
  reservationbru[reservationIteratorBRU].rsource1 = issue_instruction_struct[current].tagsource1;

  if(issue_isPhys == 0){
    issue_instruction_struct[current].rsource1value = registers[issue_instruction_struct[current].rsource1];
    reservationbru[reservationIteratorBRU].rsource1value = registers[issue_instruction_struct[current].rsource1];
    reservationbru[reservationIteratorBRU].rsource1ready = 1;
  }
  else{

    if(physRegisters[issue_instruction_struct[current].tagsource1].ready == 1){
      issue_instruction_struct[current].rsource1value = physRegisters[issue_instruction_struct[current].tagsource1].value;
      reservationbru[reservationIteratorBRU].rsource1value = physRegisters[issue_instruction_struct[current].tagsource1].value;
      reservationbru[reservationIteratorBRU].rsource1ready = 1;
    }
    else{
      if(issue_instruction_struct[current].tagsource1 == issue_instruction_struct[current].tagDestination){
        foundtag = second_last(inuseTags, issue_instruction_struct[current].rsource1);
        if(foundtag.found == 1){
          reservationbru[reservationIteratorBRU].rsource1 = foundtag.number;
          issue_instruction_struct[current].tagsource1 = foundtag.number;
          if(physRegisters[foundtag.number].ready){
            reservationbru[reservationIteratorBRU].rsource1value = physRegisters[foundtag.number].value;
            issue_instruction_struct[current].rsource1value = physRegisters[foundtag.number].value;
            reservationbru[reservationIteratorBRU].rsource1ready = 1;
          }
          else reservationbru[reservationIteratorBRU].rsource1ready = 0;
        }
        else{
          reservationbru[reservationIteratorBRU].rsource1ready = 1;
          reservationbru[reservationIteratorBRU].rsource1 = issue_instruction_struct[current].rsource1;
          reservationbru[reservationIteratorBRU].rsource1value = registers[issue_instruction_struct[current].rsource1];
          issue_instruction_struct[current].rsource1value = registers[issue_instruction_struct[current].rsource1];
        }
      }
      else{
        reservationbru[reservationIteratorBRU].rsource1ready = 0;
      }
    }
  }

  if(issue_instruction_struct[current].instruction_type == 3 || issue_instruction_struct[current].instruction_type == 5 || issue_instruction_struct[current].instruction_type == 6){
    reservationbru[reservationIteratorBRU].rsource2 = issue_instruction_struct[current].tagsource2;
    if(issue_isPhys2 == 0){
      reservationbru[reservationIteratorBRU].rsource2 = issue_instruction_struct[current].rsource2;

      issue_instruction_struct[current].rsource2value = registers[issue_instruction_struct[current].rsource2];
      reservationbru[reservationIteratorBRU].rsource2value = registers[issue_instruction_struct[current].rsource2];
      reservationbru[reservationIteratorBRU].rsource2ready = 1;
    }
    else{
      if(physRegisters[issue_instruction_struct[current].tagsource2].ready == 1){
        issue_instruction_struct[current].rsource2value = physRegisters[issue_instruction_struct[current].tagsource2].value;
        reservationbru[reservationIteratorBRU].rsource2 = issue_instruction_struct[current].tagsource2;
        reservationbru[reservationIteratorBRU].rsource2value = physRegisters[issue_instruction_struct[current].tagsource2].value;
        reservationbru[reservationIteratorBRU].rsource2ready = 1;
      }
      else{
        if(issue_instruction_struct[current].tagsource2 == issue_instruction_struct[current].tagDestination){
          foundtag = second_last(inuseTags, issue_instruction_struct[current].rsource2);
          if(foundtag.found == 1){
            reservationbru[reservationIteratorBRU].rsource2 = foundtag.number;
            issue_instruction_struct[current].tagsource2 = foundtag.number;
            if(physRegisters[foundtag.number].ready){
              reservationbru[reservationIteratorBRU].rsource2value = physRegisters[foundtag.number].value;
              issue_instruction_struct[current].rsource2value = physRegisters[foundtag.number].value;
              reservationbru[reservationIteratorBRU].rsource2ready = 1;
            }
            else reservationbru[reservationIteratorBRU].rsource2ready = 0;
          }
          else{
            reservationbru[reservationIteratorBRU].rsource2ready = 1;
            reservationbru[reservationIteratorBRU].rsource2 = issue_instruction_struct[current].rsource2;
            reservationbru[reservationIteratorBRU].rsource2value = registers[issue_instruction_struct[current].rsource2];
            issue_instruction_struct[current].rsource2value = registers[issue_instruction_struct[current].rsource2];
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

  reservationbru[reservationIteratorBRU].opcode = issue_instruction_struct[current].opcode;
  reservationbru[reservationIteratorBRU].funct3 = issue_instruction_struct[current].funct3;
  reservationbru[reservationIteratorBRU].funct7 = issue_instruction_struct[current].funct7;
  reservationbru[reservationIteratorBRU].shamt = issue_instruction_struct[current].shamt;
  reservationbru[reservationIteratorBRU].imm = issue_instruction_struct[current].imm;
  reservationbru[reservationIteratorBRU].pc = issue_instruction_struct[current].pc;
  reservationbru[reservationIteratorBRU].instruction_type = issue_instruction_struct[current].instruction_type;
  reservationbru[reservationIteratorBRU].inuse = 1;
  reservationbru[reservationIteratorBRU].inExecute = 0;
  reservationbru[reservationIteratorBRU].instruction_hex = issue_instruction_struct[current].instruction_hex;
  issue_instruction_struct[current].instructionid = instructionid;
  reservationbru[reservationIteratorBRU].instruction = issue_instruction_struct[current];

  reservationbru[reservationIteratorBRU].instructionid = issue_instruction_struct[current].instructionid;
  reservationIteratorBRU++;

  tag tagData;
  tagData.tagNumber = -2;
  tagData.registerNumber = issue_instruction_struct[current].rdestination;
  addafternodeinstruction(inOrderInstructions, issue_instruction_struct[current].instruction_hex, instructionid, tagData, issue_unit_type[current], 0, 0);
  instructionid++;
}

if(issue_unit_type[current] == 2){
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

  reservationlsu[reservationIteratorLSU].rdestination = issue_instruction_struct[current].tagDestination;
  reservationlsu[reservationIteratorLSU].rsource1 = issue_instruction_struct[current].tagsource1;

  if(issue_isPhys == 0){
    issue_instruction_struct[current].rsource1value = registers[issue_instruction_struct[current].rsource1];
    reservationlsu[reservationIteratorLSU].rsource1value = registers[issue_instruction_struct[current].rsource1];
    reservationlsu[reservationIteratorLSU].rsource1ready = 1;
  }
  else{

    if(physRegisters[issue_instruction_struct[current].tagsource1].ready == 1){
      issue_instruction_struct[current].rsource1value = physRegisters[issue_instruction_struct[current].tagsource1].value;
      reservationlsu[reservationIteratorLSU].rsource1value = physRegisters[issue_instruction_struct[current].tagsource1].value;
      reservationlsu[reservationIteratorLSU].rsource1ready = 1;
    }
    else{
      if(issue_instruction_struct[current].tagsource1 == issue_instruction_struct[current].tagDestination){
        foundtag = second_last(inuseTags, issue_instruction_struct[current].rsource1);
        if(foundtag.found == 1){
          reservationlsu[reservationIteratorLSU].rsource1 = foundtag.number;
          issue_instruction_struct[current].tagsource1 = foundtag.number;
          if(physRegisters[foundtag.number].ready){
            reservationlsu[reservationIteratorLSU].rsource1value = physRegisters[foundtag.number].value;
            issue_instruction_struct[current].rsource1value = physRegisters[foundtag.number].value;
            reservationlsu[reservationIteratorLSU].rsource1ready = 1;
          }
          else reservationlsu[reservationIteratorLSU].rsource1ready = 0;
        }
        else{
          reservationlsu[reservationIteratorLSU].rsource1ready = 1;
          reservationlsu[reservationIteratorLSU].rsource1 = issue_instruction_struct[current].rsource1;
          reservationlsu[reservationIteratorLSU].rsource1value = registers[issue_instruction_struct[current].rsource1];
          issue_instruction_struct[current].rsource1value = registers[issue_instruction_struct[current].rsource1];
        }
      }
      else{
        reservationlsu[reservationIteratorLSU].rsource1ready = 0;
      }
    }
  }

  if(issue_instruction_struct[current].instruction_type == 3 || issue_instruction_struct[current].instruction_type == 5 || issue_instruction_struct[current].instruction_type == 6){
    reservationlsu[reservationIteratorLSU].rsource2 = issue_instruction_struct[current].tagsource2;
    if(issue_isPhys2 == 0){
      reservationlsu[reservationIteratorLSU].rsource2 = issue_instruction_struct[current].rsource2;

      issue_instruction_struct[current].rsource2value = registers[issue_instruction_struct[current].rsource2];
      reservationlsu[reservationIteratorLSU].rsource2value = registers[issue_instruction_struct[current].rsource2];
      reservationlsu[reservationIteratorLSU].rsource2ready = 1;
    }
    else{
      if(physRegisters[issue_instruction_struct[current].tagsource2].ready == 1){
        issue_instruction_struct[current].rsource2value = physRegisters[issue_instruction_struct[current].tagsource2].value;
        reservationlsu[reservationIteratorLSU].rsource2 = issue_instruction_struct[current].tagsource2;
        reservationlsu[reservationIteratorLSU].rsource2value = physRegisters[issue_instruction_struct[current].tagsource2].value;
        reservationlsu[reservationIteratorLSU].rsource2ready = 1;
      }
      else{
        if(issue_instruction_struct[current].tagsource2 == issue_instruction_struct[current].tagDestination){
          foundtag = second_last(inuseTags, issue_instruction_struct[current].rsource2);
          if(foundtag.found == 1){
            reservationlsu[reservationIteratorLSU].rsource2 = foundtag.number;
            issue_instruction_struct[current].tagsource2 = foundtag.number;
            if(physRegisters[foundtag.number].ready){
              reservationlsu[reservationIteratorLSU].rsource2value = physRegisters[foundtag.number].value;
              issue_instruction_struct[current].rsource2value = physRegisters[foundtag.number].value;
              reservationlsu[reservationIteratorLSU].rsource2ready = 1;
            }
            else reservationlsu[reservationIteratorLSU].rsource2ready = 0;
          }
          else{
            reservationlsu[reservationIteratorLSU].rsource2ready = 1;
            reservationlsu[reservationIteratorLSU].rsource2 = issue_instruction_struct[current].rsource2;
            reservationlsu[reservationIteratorLSU].rsource2value = registers[issue_instruction_struct[current].rsource2];
            issue_instruction_struct[current].rsource2value = registers[issue_instruction_struct[current].rsource2];
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

  reservationlsu[reservationIteratorLSU].opcode = issue_instruction_struct[current].opcode;
  reservationlsu[reservationIteratorLSU].funct3 = issue_instruction_struct[current].funct3;
  reservationlsu[reservationIteratorLSU].funct7 = issue_instruction_struct[current].funct7;
  reservationlsu[reservationIteratorLSU].shamt = issue_instruction_struct[current].shamt;
  reservationlsu[reservationIteratorLSU].imm = issue_instruction_struct[current].imm;
  reservationlsu[reservationIteratorLSU].pc = issue_instruction_struct[current].pc;
  reservationlsu[reservationIteratorLSU].instruction_type = issue_instruction_struct[current].instruction_type;
  reservationlsu[reservationIteratorLSU].inuse = 1;
  reservationlsu[reservationIteratorLSU].inExecute = 0;
  reservationlsu[reservationIteratorLSU].instruction_hex = issue_instruction_struct[current].instruction_hex;
  issue_instruction_struct[current].instructionid = instructionid;
  reservationlsu[reservationIteratorLSU].instruction = issue_instruction_struct[current];

  reservationlsu[reservationIteratorLSU].instructionid = issue_instruction_struct[current].instructionid;
  reservationIteratorLSU++;

  tag tagData;
  if(issue_instruction_struct[current].instruction_type == 1){
    tagData.tagNumber = issue_instruction_struct[current].tagDestination;
    tagData.registerNumber = issue_instruction_struct[current].rdestination;
  }
  else{
    tagData.tagNumber = -2;
    tagData.registerNumber = -2;
  }

  addafternodeinstruction(inOrderInstructions, issue_instruction_struct[current].instruction_hex, instructionid, tagData, issue_unit_type[current], 0, 0);
  instructionid++;
}
  stall_from_issue = 0;
  return;

}

void issue(){
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

  if(block_decode_to_issue){
    block_decode_to_issue = 0;
    return;
  }

  for(; current < NWAY; current++){
  if(stall_from_issue == 0){

  if(issue_instruction_struct[current].instruction_type == 0){
    // current = 0;
    // break;
    continue;
  }

  // printf("%d\n", issue_instruction_struct[current].instruction_type);
  if(issue_instruction_struct[current].instruction_type == 4 && issue_instruction_struct[current].rdestination == 0 && pc[0] != 0){
    issue_instruction_struct[current].instruction_type = 0;
    pc[0] = issue_instruction_struct[current].pc + issue_instruction_struct[current].imm;
    if(pc[0] == 0){
      // issue_finished = 1;
    }
    else{

      flush_from_issue = 1;
    }
    current = 0;

    tag tagData;
    tagData.tagNumber = -2;
    tagData.registerNumber = 0;
    addafternodeinstruction(inOrderInstructions, issue_instruction_struct[current].instruction_hex, instructionid, tagData, issue_unit_type[current], 0, "jal");
    issue_instruction_struct[current].instructionid = instructionid;
    jlu[0].instruction = issue_instruction_struct[current];
    jlu[0].valueInside = pc[0];
    jlu[0].instruction_name = "jal";
    jlu[0].readyForWriteback = 1;
    purge = 1;//TODO:try this when you wake up matthew
    purgeid = instructionid;

    instructionid++;

    break;
  }

  print_issue_summary = 1;

  issue_rename(current);
  if(stall_rename == 1)break;

  issue_add_to_reservation(current);
  if(stall_from_issue == 2)break;

  }
  else if(stall_rename){
    if(issue_instruction_struct[current].instruction_type == 0){
      // current = 0;
      // break;
      continue;
    }

    // printf("%d\n", issue_instruction_struct[current].instruction_type);
    if(issue_instruction_struct[current].instruction_type == 4 && issue_instruction_struct[current].rdestination == 0 && pc[0] != 0){
      issue_instruction_struct[current].instruction_type = 0;
      pc[0] = issue_instruction_struct[current].pc + issue_instruction_struct[current].imm;


      if(pc[0] == 0){
        // issue_finished = 1;
      }
      else{

        flush_from_issue = 1;
      }
      current = 0;

      tag tagData;
      tagData.tagNumber = -2;
      tagData.registerNumber = 0;
      addafternodeinstruction(inOrderInstructions, issue_instruction_struct[current].instruction_hex, instructionid, tagData, issue_unit_type[current], 0, "jal");
      issue_instruction_struct[current].instructionid = instructionid;
      jlu[0].instruction = issue_instruction_struct[current];
      jlu[0].valueInside = pc[0];
      jlu[0].instruction_name = "jal";
      jlu[0].readyForWriteback = 1;
      purge = 1;//TODO:try this when you wake up matthew
      purgeid = instructionid;
      instructionid++;

      break;
    }
    issue_rename(current);
    if(stall_rename == 1)break;

    issue_add_to_reservation(current);
    if(stall_from_issue == 2)break;
  }
  else{
    if(issue_instruction_struct[current].instruction_type == 0){
      // current = 0;
      // break;
      continue;
    }

    // printf("%d\n", issue_instruction_struct[current].instruction_type);
    if(issue_instruction_struct[current].instruction_type == 4 && issue_instruction_struct[current].rdestination == 0 && pc[0] != 0){
      issue_instruction_struct[current].instruction_type = 0;
      pc[0] = issue_instruction_struct[current].pc + issue_instruction_struct[current].imm;


      if(pc[0] == 0){
        // issue_finished = 1;
      }
      else{

        flush_from_issue = 1;
      }
      current = 0;

      tag tagData;
      tagData.tagNumber = -2;
      tagData.registerNumber = 0;
      addafternodeinstruction(inOrderInstructions, issue_instruction_struct[current].instruction_hex, instructionid, tagData, issue_unit_type[current], 0, "jal");
      issue_instruction_struct[current].instructionid = instructionid;
      jlu[0].instruction = issue_instruction_struct[current];
      jlu[0].valueInside = pc[0];
      jlu[0].instruction_name = "jal";
      jlu[0].readyForWriteback = 1;
      purge = 1;//TODO:try this when you wake up matthew
      purgeid = instructionid;

      instructionid++;

      break;
    }
    issue_add_to_reservation(current);
    if(stall_from_issue == 2)break;

  }
}
  if(current == NWAY){
    current = 0;
  }

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
    // stall_from_issue = 0;
    purgepipe();
  }

  return;
}
