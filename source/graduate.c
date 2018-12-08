#include "run.h"

void graduate(){
  if(first_writeback < 2)return;
  // tag first = get(inuseTags);
  start(outOfOrderInstructions);
  start(inOrderInstructions);
  int instructionid1;
  find foundid;
  instructionid1 = getinstructionid(inOrderInstructions);
  printf("\nInOrderInstructions\n");
  printring(inOrderInstructions);
  printf("===\n");
  printf("OutOfOrderInstructions\n");
  printring(outOfOrderInstructions);
  printf("\n");
  while(1){
    foundid = find_id(outOfOrderInstructions, instructionid1);
    if(foundid.found == 1){
      everything value = get_everything(inOrderInstructions);
      if(value.tagData.tagNumber != -2){
        movenode(inuseTags, unusedTags, -1);
        registers[value.tagData.registerNumber] = physRegisters[value.tagData.tagNumber].value;
      }
      deletenode(inOrderInstructions);
      deletenode(outOfOrderInstructions);
      start(outOfOrderInstructions);
      instructionid1 = getinstructionid(inOrderInstructions);
      registers[value.tagData.registerNumber] = physRegisters[value.tagData.tagNumber].value;

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
