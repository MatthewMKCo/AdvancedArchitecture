#include "run.h"

void fetch(){
  printf("Current PC:%d\n",pc[0]);
  if(last_instruction == 1)return;
  fetch_next_instruction = ld(Icache, pc[0]);
  nextpc = pc[0];
  pc[0] = pc[0] + 4;
  // printf("fetchinstruction:%x\n",fetch_next_instruction);

  if(first_fetch < 2){
    first_fetch = first_fetch + 1;
  }
  if(fetch_next_instruction == -1 && last_instruction != 1){
    last_instruction = 1;
    last_instruction_cycle = current_cycle - 1;
  }
}
