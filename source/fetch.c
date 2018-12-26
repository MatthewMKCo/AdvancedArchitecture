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
    // if(last_instruction != 1)last_instruction = 1;
    return;
  }

  if(first_fetch < 2){
    first_fetch = first_fetch + 1;
  }
  // if(current_cycle == 288){
  //   for(int j = 0; j < NWAY; j++){
  //     printf("%d\n", fetchpc[j]);
  //   }
  //   printf("%d\n",stall_from_issue);
  //   exit_early();
  // }

  // if(last_instruction == 1)return;
  if(stall_from_issue != 0){
    // first_fetch = 0;
    return;
  }

  int i = 0;
  for(; i < NWAY; i++){
  fetch_instruction[i] = ld(Icache, pc[0]);
  fetchpc[i] = pc[0];
  // pc[0] = pc[0] + 4;

  int branch_taken = branch_predictor(pc[0], 0, 0);
  if(branch_taken)break;
  if(fetch_instruction[i] == -1 && last_instruction != 1){
    // last_instruction = 1;
    // last_instruction_cycle = current_cycle - 1;
  }
}

// if(current_cycle == 288){
//   for(int j = 0; j < NWAY; j++){
//     printf("%d\n", fetchpc[j]);
//   }
//     exit_early();
// }
  for(i = i + 1; i < NWAY; i++){
    fetch_instruction[i] = -1;
  }
}
