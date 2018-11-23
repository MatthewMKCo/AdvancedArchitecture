#include "run.h"

void mem_acc(){
  if(first_execute < 2)return;
  if(last_instruction == 1){
    if(current_cycle > last_instruction_cycle + 3)return;
  }
  if(first_mem_access < 2)first_mem_access++;
  if(mem_access == 1){

    mem_access = 0;
  }
}
