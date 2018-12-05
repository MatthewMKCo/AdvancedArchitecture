#include "run.h"

void writeintophysreg(int i){


    physRegisters[writebackalu[i].tag].value = writebackalu[i].value;
    physRegisters[writebackalu[i].tag].ready = 1;
    // printf("%d\n",alu[i].wbDestinationRegister);
    // exit_early();

    // printf("desitnation:%d\n",execute_rdestination);
    // printf("total:%d\n", execute_val);
    // exit_early();
    writeback_destination = writebackalu[i].tag;
    // exit_early();

    tag orderTag;
    orderTag.tagNumber = writebackalu[i].tag;
    orderTag.registerNumber = -1;
    addafternode(outOfOrderInstructions, orderTag);
}

void writeback(){
    if(first_execute < 2)return;
    if(first_writeback < 2)first_writeback++;
    // if(last_instruction == 1){
    //   if(current_cycle > last_instruction_cycle + 3)return;
    // }

  for(int i = 0; i < ALU_NUM; i++){
    if(writebackalu[i].ready == 1){
      writebackalu[i].ready = 0;
      writeintophysreg(i);
    }
  }
}
