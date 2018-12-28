#include "run.h"

int b3 = 0;
void writeintophysreg(int i){
    physRegisters[writebackalu[i].tag].value = writebackalu[i].value;
    physRegisters[writebackalu[i].tag].ready = 1;

    forward_reservation_stations(writebackalu[i].tag, writebackalu[i].value);

    writeback_destination = writebackalu[i].tag;

    tag orderTag;
    orderTag.tagNumber = writebackalu[i].tag;
    orderTag.registerNumber = get_register(inuseTags, orderTag.tagNumber);
    addafternodeinstruction(outOfOrderInstructions, writebackalu[i].instruction, writebackalu[i].instructionid, orderTag, 1, writebackalu[i].value, writebackalu[i].instruction_name);


    return;
}

void writebackbranch(int i){
  tag orderTag;
  orderTag.tagNumber = -2;
  orderTag.registerNumber = -2;
  addafternodeinstruction(outOfOrderInstructions, writebackbru[i].instruction, writebackbru[i].instructionid, orderTag, 2, writebackbru[i].value, writebackbru[i].instruction_name);
  return;
}

void writebackloadstore(int i){
  tag orderTag;

  if(writebacklsu[i].instruction_type == 1){
    orderTag.tagNumber = writebacklsu[i].tag;
    orderTag.registerNumber = get_register(inuseTags, orderTag.tagNumber);
    physRegisters[writebacklsu[i].tag].value = writebacklsu[i].value;
    physRegisters[writebacklsu[i].tag].ready = 1;
    forward_reservation_stations(writebacklsu[i].tag, writebacklsu[i].value);
  }
  else{
    orderTag.tagNumber = -2;

    orderTag.registerNumber = writebacklsu[i].tag;//do not remove, tag is for destination address
  }

  addafternodeinstruction(outOfOrderInstructions, writebacklsu[i].instruction, writebacklsu[i].instructionid, orderTag, 3, writebacklsu[i].value, writebacklsu[i].instruction_name);
  return;
}

void writebackjal(int i){
  tag orderTag;
  orderTag.tagNumber = -2;
  orderTag.registerNumber = 0;

  addafternodeinstruction(outOfOrderInstructions, writebackjlu[i].instruction, writebackjlu[i].instructionid, orderTag, 1, writebackjlu[i].value, writebackjlu[i].instruction_name);

  return;
}

void writeback(){
    if(first_execute < 2)return;
    if(first_writeback < 2)first_writeback++;
    if(writeback_finished)return;
    if(execute_finished){
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
  for(int i = 0; i < JLU_NUM; i++){
    if(writebackjlu[i].ready == 1){
      writebackjlu[i].ready = 0;
      writebackjal(i);
    }
  }
}
