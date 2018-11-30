#include "run.h"

int tagIterator = 0;
int reservationIteratorALU = 0;
int reservationIteratorLSU = 0;
int reservationIteratorBRU = 0;


void issue_rename(){
  tags[tagIterator].registerNumber = issue_rdestination;
  tags[tagIterator].ready = 0;
  issue_rdestination = tagIterator;
  tagIterator++;

  int tagChecker = tagIterator;
  while(tagChecker != -1){
    if(tags[tagChecker].registerNumber == issue_rsource1){
      issue_rsource1 = tagChecker;
      break;
    }
    tagChecker = tagChecker - 1;
  }
  if(tagChecker == -1){
    tags[tagIterator].registerNumber = issue_rsource1;
    tagIterator++;
  }

  if(issue_instruction_type == 3 || issue_instruction_type == 5 || issue_instruction_type == 6){
    int tagChecker = tagIterator;
    while(tagChecker != -1){
      if(tags[tagChecker].registerNumber == issue_rsource2){
        issue_rsource2 = tagChecker;
        break;
      }
      tagChecker = tagChecker - 1;
    }
    if(tagChecker == -1){
      tags[tagIterator].registerNumber = issue_rsource2;
      tagIterator++;
    }
  }
  return;
}

void issue_add_to_reservation(){
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
    printf("ERROR, PUT A STALL FUNCTION IN MATTHEW");
    exit(1);
  }
  reservationalu[reservationIteratorALU].rdestination = issue_rdestination;

  reservationalu[reservationIteratorALU].rsource1 = issue_rsource1;
  if(physRegisters[reservationalu[reservationIteratorALU].rsource1].ready == 1){
    reservationalu[reservationIteratorALU].rsource1value = physRegisters[reservationalu[reservationIteratorALU].rsource1].value;
    reservationalu[reservationIteratorALU].rsource1ready = 1;
  }
  else{
    reservationalu[reservationIteratorALU].rsource1ready = 0;
  }

  if(issue_instruction_type == 3 || issue_instruction_type == 5 || issue_instruction_type == 6){
    reservationalu[reservationIteratorALU].rsource2 = issue_rsource2;
    if(physRegisters[reservationalu[reservationIteratorALU].rsource2].ready == 1){
      reservationalu[reservationIteratorALU].rsource2value = physRegisters[reservationalu[reservationIteratorALU].rsource2].value;
      reservationalu[reservationIteratorALU].rsource2ready = 1;
    }
    else{
      reservationalu[reservationIteratorALU].rsource2ready = 0;
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

  reservationIteratorALU++;
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
      return;
    }
  }
  print_issue_summary = 1;

  issue_rename();

  issue_add_to_reservation();
}
