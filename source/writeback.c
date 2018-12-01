#include "run.h"

void writeintophysreg(int i){

    // printf("hidsahosdfhsdoifsdfio\n");
    printf("MEM VALUE:%d\n",alu[i].wbValueInside);
    physRegisters[alu[i].wbDestinationRegister].value = alu[i].wbValueInside;
    // printf("desitnation:%d\n",execute_rdestination);
    // printf("total:%d\n", execute_val);
    // exit_early();
}

void writeback(){
    if(first_execute < 2)return;
    // if(last_instruction == 1){
    //   if(current_cycle > last_instruction_cycle + 3)return;
    // }

  for(int i = 0; i < ALU_NUM; i++){
    if(alu[i].readyForWriteback == 2){
      alu[i].readyForWriteback = 0;
      if(alu[i].shouldWriteback == 1){
        alu[i].shouldWriteback = 0;
        writeintophysreg(i);
      }
    }
  }
}
