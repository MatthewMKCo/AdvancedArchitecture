#include "run.h"

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
  int instructionid1;
  find foundid;
  instructionid1 = getinstructionid(inOrderInstructions);
  // printf("\nInOrderInstructions\n");
  // printring(inOrderInstructions);
  // printf("===\n");
  // printf("OutOfOrderInstructions\n");
  // printring(outOfOrderInstructions);
  // printf("\n");
  // printf("AllInOrder\n");
  // printring(allInOrder);
  while(1){
    foundid = find_id(outOfOrderInstructions, instructionid1);
    if(foundid.found == 1){
      // if(instructionid1 == 113)exit_early();
      everything value = get_everything(inOrderInstructions);
      everything value2 = get_everything(outOfOrderInstructions);
      if(value2.ready != 1)break;
      if(value.tagData.tagNumber != -2){
        movenode(inuseTags, unusedTags, -1);
        if(value.tagData.registerNumber != 0){
          // printf("%d\n",value.tagData.registerNumber);
          // printf("%d\n",value.tagData.tagNumber);
          // printf("%d\n",physRegisters[value.tagData.tagNumber].value);
          registers[value.tagData.registerNumber] = physRegisters[value.tagData.tagNumber].value;
          // stop = 1;
          // exit_early();
          // print_reg_summary();
      }
    }
      // printf("%d\n",getinstructionid(inOrderInstructions));
      // printf("%x\n",value.instruction);
      // if(instructionid1 == 115)exit_early();
      // deletenode(inOrderInstructions);
      // printring(inOrderInstructions);
      // printring(allInOrder);
      // printring(allInOrder);
      // if(instructionid1 == 115)exit_early();
      movenode(inOrderInstructions, allInOrder, value.tagData.registerNumber);
      addvaluetolast(allInOrder, registers[value.tagData.registerNumber]);
      deletenode(outOfOrderInstructions);
      start(outOfOrderInstructions);
      start(inOrderInstructions);

      instructionid1 = getinstructionid(inOrderInstructions);

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
