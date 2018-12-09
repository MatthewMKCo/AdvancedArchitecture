#include "run.h"

void writeintophysreg(int i){

    physRegisters[writebackalu[i].tag].value = writebackalu[i].value;
    physRegisters[writebackalu[i].tag].ready = 1;

    if(writebackalu[i].tag == 9){
      // exit_early();
}
    forward_reservation_stations(writebackalu[i].tag, writebackalu[i].value, 1);

    writeback_destination = writebackalu[i].tag;

    tag orderTag;
    orderTag.tagNumber = writebackalu[i].tag;
    orderTag.registerNumber = -1;
    addafternodeinstruction(outOfOrderInstructions, writebackalu[i].instruction, writebackalu[i].instructionid, orderTag);
}

void writeback(){
    if(first_execute < 2)return;
    if(first_writeback < 2)first_writeback++;
    if(writeback_finished)return;
    if(execute_finished){
      // printf("%d\n",current_cycle);
      // printf("%d\n",execute_cycle_finished);

      // exit_early();
      if(current_cycle == (execute_cycle_finished + 1)){
        writeback_finished = 1;
      }
    }
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
