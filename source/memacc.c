#include "run.h"

void mem_acc(){
  if(first_execute < 2)return;
  if(last_instruction == 1){
    if(current_cycle > last_instruction_cycle + 3)return;
  }

  if(mem_rdestination != 0){
    // printf("hidsahosdfhsdoifsdfio\n");
    printf("MEM VALUE:%d\n",mem_acc_val);
    registers[mem_rdestination] = mem_acc_val;
    // printf("desitnation:%d\n",execute_rdestination);
    // printf("total:%d\n", execute_val);
  }
}
