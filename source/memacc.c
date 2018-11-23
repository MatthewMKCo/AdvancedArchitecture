#include "run.h"

void mem_acc(){
  if(first_execute < 2)return;
  if(last_instruction == 1){
    if(current_cycle > last_instruction_cycle + 3)return;
  }
  if(first_mem_access < 2)first_mem_access++;
  if(mem_access == 1){
    if(mem_store){
      memory_sformat();
      mem_store = 0;
    }
    else if(mem_load){
      memory_iformat();
      mem_load = 0;
      // exit(1);
    }
    else{
      printf("ERROR\n");
      exit(1);
    }
    mem_access = 0;
  }

  if(mem_rdestination != 0 && first_mem_access >= 1){
    // printf("hidsahosdfhsdoifsdfio\n");
    registers[mem_rdestination] = mem_acc_val;
    // printf("desitnation:%d\n",execute_rdestination);
    // printf("total:%d\n", execute_val);
  }
}
