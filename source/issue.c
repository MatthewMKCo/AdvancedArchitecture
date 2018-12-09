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

void issue_rename(){

  if(issue_instruction_type  == 1 || issue_instruction_type == 2 || issue_instruction_type == 3 || issue_instruction_type == 4){
    issue_instruction_struct.tagDestination = movenode(unusedTags, inuseTags, issue_instruction_struct.rdestination);
    physRegisters[issue_instruction_struct.tagDestination].ready = 0;

  }
  if(issue_instruction_type == 4){
    issue_instruction_struct.pctag = movenode(unusedTags, inuseTags, issue_instruction_struct.pcDestination);
  }

  find foundtag = find_register_last(inuseTags, issue_instruction_struct.rsource1);

  if(foundtag.found == 1){
    issue_isPhys = 1;
    issue_instruction_struct.tagsource1 = foundtag.number;
    // physRegisters[issue_instruction_struct.tagsource1].ready = 0;
  }
  else{
    issue_isPhys = 0;
  }

  if(issue_instruction_type == 3 || issue_instruction_type == 5 || issue_instruction_type == 6){
    foundtag = find_register_last(inuseTags, issue_instruction_struct.rsource2);
    if(issue_instruction_struct.rsource2 == 11){
      exit_early();
    }

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
    exit_early();
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
          reservationalu[reservationIteratorALU].rsource1ready = 0;
          reservationalu[reservationIteratorALU].rsource1 = foundtag.number;
          issue_instruction_struct.tagsource1 = foundtag.number;
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

  if(issue_instruction_type == 3 || issue_instruction_type == 5 || issue_instruction_type == 6){
    printf("Iterator:%d\n",reservationIteratorALU);
    reservationalu[reservationIteratorALU].rsource2 = issue_instruction_struct.tagsource2;
    if(issue_isPhys2 == 0){
      printf("Architectural:%d\n\n",reservationalu[reservationIteratorALU].rsource2);
      reservationalu[reservationIteratorALU].rsource2 = issue_instruction_struct.rsource2;

      issue_instruction_struct.rsource2value = registers[issue_instruction_struct.rsource2];
      reservationalu[reservationIteratorALU].rsource2value = registers[issue_instruction_struct.rsource2];
      reservationalu[reservationIteratorALU].rsource2ready = 1;
    }
    else{
      printf("Physical:%d\n\n",physRegisters[9].ready);

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
            reservationalu[reservationIteratorALU].rsource2ready = 0;
            reservationalu[reservationIteratorALU].rsource2 = foundtag.number;
            issue_instruction_struct.tagsource2 = foundtag.number;
          }
          else{
            reservationalu[reservationIteratorALU].rsource2ready = 1;
            reservationalu[reservationIteratorALU].rsource2 = issue_instruction_struct.rsource2;
            reservationalu[reservationIteratorALU].rsource2value = registers[issue_instruction_struct.rsource2];
            issue_instruction_struct.rsource2value = registers[issue_instruction_struct.rsource2];
          }
        }
        else{
          printf("HELLO\n");
          reservationalu[reservationIteratorALU].rsource2ready = 0;
        }
      }
  }
}
  else{
    reservationalu[reservationIteratorALU].rsource2 = -1;
    reservationalu[reservationIteratorALU].rsource2ready = 1;
  }

  if(issue_instruction_struct.tagDestination == 10){
    printf("source1:%d ready%d\n",reservationalu[reservationIteratorALU].rsource1,reservationalu[reservationIteratorALU].rsource1ready);
    printf("source2:%d ready%d\n",reservationalu[reservationIteratorALU].rsource2,reservationalu[reservationIteratorALU].rsource2ready);

    exit_early();
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
  // if(last_instruction == 1){
  //   if(current_cycle > last_instruction_cycle + 2){
  //     print_execute_summary = 0;
  //     // return;
  //   }
  // }
  if(issue_finished)return;
  if(issue_instruction_type == 0){
    return;
  }
  if(issue_instruction_struct.instruction_type == 4 && issue_instruction_struct.rdestination == 0){
    issue_instruction_struct.instruction_type = 0;
    pc[0] = issue_instruction_struct.pc + issue_instruction_struct.imm;
    instructions_executed++;
    issue_finished = 1;
    return;
  }

  print_issue_summary = 1;

  if(stall_from_issue == 0){
    issue_rename();
  }

  issue_add_to_reservation();

}
