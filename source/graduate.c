#include "run.h"

int sequencenumber = 0;

void graduate(){
  if(first_writeback < 2)return;
  if(graduate_finished)return;
  if(execute_finished){
    if(current_cycle == (execute_cycle_finished + 2)){
      graduate_finished = 1;
    }
  }
  // tag first = get(inuseTags);
  start(outOfOrderInstructions);
  start(inOrderInstructions);
  // int instructionid1;
  find foundid;
  // printf("%d\n",sequencenumber);
  // instructionid1 = getinstructionid(inOrderInstructions);
  // printf("\nInOrderInstructions\n");
  // printring(inOrderInstructions);
  // printf("===\n");
  // printf("OutOfOrderInstructions\n");
  // printring(outOfOrderInstructions);
  // printf("\n");
  // printf("AllInOrder\n");
  // printring(allInOrder);
  while(1){
    foundid = find_id(outOfOrderInstructions, sequencenumber);
    if(foundid.found == 1){
      // if(instructionid1 == 113)exit_early();
      everything value = get_everything(inOrderInstructions);
      everything value2 = get_everything(outOfOrderInstructions);
      if(value2.ready != 1)break;
      // if(value.unit_type == 1){
      //   movenode(inuseTags, unusedTags, -1);
      //   if(value.tagData.registerNumber != 0){
      //     // printf("%d\n",value.tagData.registerNumber);
      //     // printf("%d\n",value.tagData.tagNumber);
      //     // printf("%d\n",physRegisters[value.tagData.tagNumber].value);
      //     registers[value.tagData.registerNumber] = physRegisters[value.tagData.tagNumber].value;
      //     // stop = 1;
      //     // print_reg_summary();
      // }

      if(value2.unit_type == 1){
        if(value2.tagData.tagNumber != -2){
          movenode(inuseTags, unusedTags, -1, 0, 0);
          if(value2.tagData.registerNumber != 0){
            // printf("%d\n",value.tagData.registerNumber);
            // printf("%d\n",value.tagData.tagNumber);
            // printf("%d\n",physRegisters[value.tagData.tagNumber].value);
            registers[value.tagData.registerNumber] = physRegisters[value2.tagData.tagNumber].value;
        }
      }
    }


      // if(value2.unit_type == 2){
      //   if(value2.value == 1){
      //     for(int j = 0; j < SIZE; j++){
      //       Dcache[j] = Dmem[j];
      //     }
      //     // exit_early();
      //   }
      // }
      if(value2.unit_type == 3){
        if(value2.tagData.tagNumber != -2){
          // if(value2.tagData.tagNumber != get_register2(inuseTags))exit_early();
          movenode(inuseTags, unusedTags, -1, 0, 0);
          if(value2.tagData.registerNumber != 0){
            registers[value.tagData.registerNumber] = physRegisters[value2.tagData.tagNumber].value;
          }
        }
        else{
          int j = 0;
          for(; j < STORE_RESERVATION_WIDTH; j++){
            if(reservationlsu[j].instructionid == sequencenumber){
              reservationlsu[j].rdestination = -1;
              reservationlsu[j].inuse = 0;
              reservationlsu[j].inExecute = 0;
              break;
            }
          }
          if(j == STORE_RESERVATION_WIDTH){
            printf("ERROR when removing from load store reservation station in graduate\n");
          }
          sw(Dcache, value2.tagData.tagNumber, value2.value);
        }
      }
    // }
      // printf("%d\n",getinstructionid(inOrderInstructions));
      // printf("%x\n",value.instruction);
      // if(instructionid1 == 115)exit_early();
      // deletenode(inOrderInstructions);
      // printring(inOrderInstructions);
      // printring(allInOrder);
      // printring(allInOrder);
      // if(instructionid1 == 115)exit_early();
      movenode(outOfOrderInstructions, allInOrder, value2.tagData.registerNumber, sequencenumber, 1);
      deletenode(inOrderInstructions);
      start(outOfOrderInstructions);
      start(inOrderInstructions);

      // instructionid1 = getinstructionid(inOrderInstructions);
      sequencenumber++;
      instructions_executed++;
      rob--;

    }
    else break;
  }

  // while(1){
  //   // tag value = getSelected(outOfOrderInstructions);
  //   if(value.tagNumber == -1)break;
  //   if(first.tagNumber == -1){
  //     printf("Error in graduate");
  //     exit(1);
  //   }
  //   if(first.tagNumber == value.tagNumber){
  //     registers[first.registerNumber] = physRegisters[first.tagNumber].value;
  //     // physRegisters[first.tagNumber].ready = 1;
  //
  //     deletenode(outOfOrderInstructions);
  //
  //     start(outOfOrderInstructions);
  //
  //     movenode(inuseTags, unusedTags, -1);
  //     first = get(inuseTags);
  //
  //   }
  //   else next(outOfOrderInstructions);
  // }
}
