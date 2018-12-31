#include "run.h"

void fetch(){
  if(pc[0] != 0 && fetch_finished == 1){
    fetch_finished = 0;
    decode_finished = 0;
    issue_finished = 0;
    execute_finished = 0;
    writeback_finished = 0;
    graduate_finished = 0;
  }
  if(fetch_finished)return;
  if(pc[0] == 0){
    fetch_finished = 1;
    last_instruction_cycle = current_cycle - 1;
    first_fetch = 0;
    return;
  }

  if(first_fetch < 2){
    first_fetch = first_fetch + 1;
  }
  if(stall_from_issue != 0){
    return;
  }

  int i = 0;
  for(; i < NWAY; i++){
  fetch_instruction[i] = ld(Icache, pc[0]);
  fetchpc[i] = pc[0];
  // pc[0] = pc[0] + 4;

  int branch_taken = branch_predictor(pc[0], 0, 0);
  if(branch_taken){
    fetch_branch[i] = 1;
    // break;
  }
  else fetch_branch[i] = 0;

}

  for(i = i + 1; i < NWAY; i++){
    fetch_instruction[i] = -1;
  }
}
