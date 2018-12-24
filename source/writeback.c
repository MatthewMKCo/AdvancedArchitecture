#include "run.h"
void writeintophysreg(int i){
    physRegisters[writebackalu[i].tag].value = writebackalu[i].value;
    physRegisters[writebackalu[i].tag].ready = 1;

    forward_reservation_stations(writebackalu[i].tag, writebackalu[i].value);

    writeback_destination = writebackalu[i].tag;

    tag orderTag;
    orderTag.tagNumber = writebackalu[i].tag;
    orderTag.registerNumber = get_register(inuseTags);
    addafternodeinstruction(outOfOrderInstructions, writebackalu[i].instruction, writebackalu[i].instructionid, orderTag, 1, writebackalu[i].value);
    return;
}

void writebackbranch(int i){
  tag orderTag;
  orderTag.tagNumber = -2;
  orderTag.registerNumber = get_register(inuseTags);
  // printring(outOfOrderInstructions);
  addafternodeinstruction(outOfOrderInstructions, writebackbru[i].instruction, writebackbru[i].instructionid, orderTag, 2, writebackbru[i].value);
  // printring(outOfOrderInstructions);
  // exit_early();
  return;
}

void writebackloadstore(int i){
  tag orderTag;
  printf("%d\n",writebacklsu[i].value);
  // if(writebacklsu[i].instructionid == 109)exit_early();
  // printf("%d\n",);
  // if(writebacklsu[i].instructionid == 366)exit_early();

  if(writebacklsu[i].instruction_type == 1){
    orderTag.tagNumber = writebacklsu[i].tag;
    orderTag.registerNumber = get_register(inuseTags);
    physRegisters[writebacklsu[i].tag].value = writebacklsu[i].value;
    physRegisters[writebacklsu[i].tag].ready = 1;
    forward_reservation_stations(writebacklsu[i].tag, writebacklsu[i].value);
  }
  else{
    // orderTag.tagNumber = writebacklsu[i].tag;
    orderTag.tagNumber = -2;

    orderTag.registerNumber = get_register(inuseTags);
  }

  addafternodeinstruction(outOfOrderInstructions, writebacklsu[i].instruction, writebacklsu[i].instructionid, orderTag, 3, writebacklsu[i].value);
  return;
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
  for(int i = 0; i < BRU_NUM; i++){
    if(writebackbru[i].ready == 1){
      writebackbru[i].ready = 0;
      writebackbranch(i);
    }
  }
  for(int i = 0; i < ALU_NUM; i++){
    if(writebackalu[i].ready == 1){
      writebackalu[i].ready = 0;
      writeintophysreg(i);
    }
  }
  for(int i = 0; i < LSU_NUM; i++){
    if(writebacklsu[i].ready == 1){
      writebacklsu[i].ready = 0;
      writebackloadstore(i);
    }
  }
}
