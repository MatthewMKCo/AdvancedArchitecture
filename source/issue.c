#include "run.h"

int tagIterator = 0;
int reservationIteratorALU = 0;
int reservationIteratorLSU = 0;
int reservationIteratorBRU = 0;

int issue_isPhys;
int issue_isPhys2;

int issue_realsource1;
int issue_realsource2;
int issue_realdestination;

int instructionid = 0;
int stall_from_issue = 0;

void issue_rename(){

  if(issue_instruction_type  == 1 || issue_instruction_type == 2 || issue_instruction_type == 3 || issue_instruction_type == 4){
    issue_realdestination = issue_instruction_struct.rdestination;
    issue_instruction_struct.tagDestination = movenode(unusedTags, inuseTags, issue_instruction_struct.rdestination);
  }

  find foundtag = find_register(inuseTags, issue_rsource1);

  if(foundtag.found == 1){
    issue_isPhys = 1;
    issue_realsource1 = issue_rsource1;
    issue_rsource1 = foundtag.number;
    physRegisters[issue_rsource1].ready = 0;
  }
  else{
    issue_isPhys = 0;
  }

  if(issue_instruction_type == 3 || issue_instruction_type == 5 || issue_instruction_type == 6){
    foundtag = find_register(inuseTags, issue_rsource2);
    if(foundtag.found == 1){
      issue_isPhys2 = 1;
      issue_realsource2 = issue_rsource2;
      issue_rsource2 = foundtag.number;
      physRegisters[issue_rsource2].ready = 0;
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
  if(reservationIteratorALU > RESERVATION_WIDTH)reservationIteratorALU = 0;
  int i;
  for(i = 0; i < RESERVATION_WIDTH; i++){
    if(reservationalu[reservationIteratorALU].inuse == 1){
      reservationIteratorALU++;
    }
    else{
      break;
    }
  }
  if(i == RESERVATION_WIDTH){
    //TODO:STALL FUNCTION
    stall_from_issue = 1;
    // printf("ERROR, PUT A STALL FUNCTION IN MATTHEW");
    // exit(1);
    return;
  }
  reservationalu[reservationIteratorALU].rdestination = issue_instruction_struct.tagDestination;
  reservationalu[reservationIteratorALU].rsource1 = issue_rsource1;

  if(issue_isPhys == 0){
    reservationalu[reservationIteratorALU].rsource1value = registers[issue_rsource1];
    reservationalu[reservationIteratorALU].rsource1ready = 1;
  }
  else{
    if(physRegisters[issue_rsource1].ready == 1){
      reservationalu[reservationIteratorALU].rsource1value = physRegisters[issue_rsource1].value;
      reservationalu[reservationIteratorALU].rsource1ready = 1;
    }
    else{
      if(issue_rsource1 == issue_instruction_struct.tagDestination){
        foundtag = second_last(inuseTags, issue_realsource1);
        if(foundtag.found == 1){
          reservationalu[reservationIteratorALU].rsource1ready = 0;
          reservationalu[reservationIteratorALU].rsource1 = foundtag.number;
        }
        else{
          reservationalu[reservationIteratorALU].rsource1ready = 1;
          reservationalu[reservationIteratorALU].rsource1 = issue_realsource1;
          reservationalu[reservationIteratorALU].rsource1value = registers[issue_realsource1];
        }
      }
      else{
        reservationalu[reservationIteratorALU].rsource1ready = 0;
      }
    }
  }

  if(issue_instruction_type == 3 || issue_instruction_type == 5 || issue_instruction_type == 6){
    reservationalu[reservationIteratorALU].rsource2 = issue_rsource2;
    if(issue_isPhys2 == 0){
      reservationalu[reservationIteratorALU].rsource2value = registers[issue_rsource2];
      reservationalu[reservationIteratorALU].rsource2ready = 1;
    }
    else{
      if(physRegisters[issue_rsource2].ready == 1){
        reservationalu[reservationIteratorALU].rsource2value = physRegisters[issue_rsource2].value;
        reservationalu[reservationIteratorALU].rsource2ready = 1;
      }
      else{
        if(issue_rsource2 == issue_instruction_struct.tagDestination){
          foundtag = second_last(inuseTags, issue_realsource2);
          if(foundtag.found == 1){
            reservationalu[reservationIteratorALU].rsource2ready = 0;
            reservationalu[reservationIteratorALU].rsource2 = foundtag.number;
          }
          else{
            reservationalu[reservationIteratorALU].rsource2ready = 1;
            reservationalu[reservationIteratorALU].rsource2 = issue_realsource2;
            reservationalu[reservationIteratorALU].rsource2value = registers[issue_realsource2];
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

  reservationalu[reservationIteratorALU].opcode = issue_opcode;
  reservationalu[reservationIteratorALU].funct3 = issue_funct3;
  reservationalu[reservationIteratorALU].funct7 = issue_funct7;
  reservationalu[reservationIteratorALU].shamt = issue_shamt;
  reservationalu[reservationIteratorALU].imm = issue_imm;
  reservationalu[reservationIteratorALU].pc = issuepc;
  reservationalu[reservationIteratorALU].instruction_type = issue_instruction_type;
  reservationalu[reservationIteratorALU].inuse = 1;
  reservationalu[reservationIteratorALU].inExecute = 0;
  reservationalu[reservationIteratorALU].instruction = issue_instruction;
  reservationalu[reservationIteratorALU].instructionid = instructionid;
  reservationIteratorALU++;

  tag tagData;
  tagData.tagNumber = issue_instruction_struct.tagDestination;
  tagData.registerNumber = issue_instruction_struct.rdestination;
  addafternodeinstruction(inOrderInstructions, issue_instruction, instructionid, tagData);
  instructionid++;
}

}

void issue(){
  if(first_decode < 2){
    print_issue_summary = 0;
    return;
  }
  if(first_issue < 2)first_issue++;
  if(last_instruction == 1){
    if(current_cycle > last_instruction_cycle + 2){
      print_execute_summary = 0;
      // return;
    }
  }
  if(issue_instruction_type == 0){
    return;
  }
  print_issue_summary = 1;

  if(stall_from_issue == 0){
    issue_rename();
  }

  issue_add_to_reservation();

}
