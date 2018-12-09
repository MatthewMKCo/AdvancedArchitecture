#include "run.h"

void fetch(){
  if(first_fetch < 2){
    first_fetch = first_fetch + 1;
  }
  if(pc[0] == 0){
    fetch_finished = 1;
    if(last_instruction != 1)last_instruction = 1;
    return;
  }
  if(pc[0] != 0 && fetch_finished == 1){
    fetch_finished = 0;
    decode_finished = 0;
    issue_finished = 0;
    execute_finished = 0;
    writeback_finished = 0;
    graduate_finished = 0;
  }
  if(last_instruction == 1)return;
  if(stall_from_issue != 0)return;
  fetch_instruction = ld(Icache, pc[0]);
  fetchpc = pc[0];
  pc[0] = pc[0] + 4;
  // printf("fetchinstruction:%x\n",fetch_next_instruction);

  if(fetch_instruction == -1 && last_instruction != 1){
    // last_instruction = 1;
    // last_instruction_cycle = current_cycle - 1;
  }
}
