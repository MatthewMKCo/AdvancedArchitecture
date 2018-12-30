#include "run.h"

int sequencenumber = 0;
int b5 = 0;

execute_unit lsu2[1];
reserve reservationlsu2[4];

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
          // sw(Dcache, value2.tagData.registerNumber, value2.value);
          // printf("%d\n",value2.tagData.registerNumber);
          // printf("%d\n",value2.value);
          //
          // exit_early();
          int empty = 1;
          for(int k = 0; k < 4; k++){
            if(reservationlsu2[k].inuse == 1)empty = 0;
            if(empty == 0)break;
          }
          if(empty && lsu2[0].ready == 1){
            lsu2[0].ready = 0;
            lsu2[0].destinationRegister = value2.tagData.registerNumber;
            lsu2[0].sourceRegister1 = value2.value;
            lsu2[0].cyclesNeeded = 4;
            lsu2[0].currentCycles = 0;
            lsu2[0].instructionid = sequencenumber;
          }
          else{
            int k = 0;
            for(; k < 4; k++){
              if(reservationlsu2[k].inuse == 0){
                reservationlsu2[k].inuse = 1;
                reservationlsu2[k].rdestination = value2.tagData.registerNumber;
                reservationlsu2[k].rsource1value = value2.value;
                reservationlsu2[k].instructionid = sequencenumber;
                break;
              }
            }
            if(k == 4){
              break;
            }
          }
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

  int x = -1;
  int idx = -1;
  if(lsu2[0].ready == 1){
    for(int j = 0; j < 4; j++){
      if(reservationlsu2[j].inuse && x == -1){
        x = j;
        idx = reservationlsu2[j].instructionid;
      }
      else if(reservationlsu2[j].inuse && idx > reservationlsu2[j].instructionid){
        x = j;
        idx = reservationlsu2[j].instructionid;
      }
    }
    if(x != -1){
      reservationlsu2[x].inuse = 0;
      lsu2[0].ready = 0;
      lsu2[0].destinationRegister = reservationlsu2[x].rdestination;
      lsu2[0].sourceRegister1 = reservationlsu2[x].rsource1value;
      lsu2[0].cyclesNeeded = 4;
      lsu2[0].currentCycles = 0;
      lsu2[0].instructionid = reservationlsu2[x].instructionid;
    }
  }
  if(lsu2[0].ready == 0){
    lsu2[0].currentCycles++;
    // exit_early();
    if(lsu2[0].currentCycles == lsu2[0].cyclesNeeded){
      // if(issue_instruction_struct[0].instructionid == 41 && current_cycle == 59){
        // sw(Dcache, -60, 6);
        sw(Dcache, lsu2[0].destinationRegister, lsu2[0].sourceRegister1);

      // }
      // printf("%d\n",lsu2[0].destinationRegister);
      // printf("%d\n",lsu2[0].sourceRegister1);
      // printf("%d\n",lsu2[0].instructionid);
      // if(b5== 20)exit_early();
      b5++;
      // sw(Dcache, value2.tagData.registerNumber, value2.value);

      lsu2[0].ready = 1;
      lsu2[0].currentCycles = 0;

      int j = 0;
      for(; j < STORE_RESERVATION_WIDTH; j++){
        if(reservationlsu[j].instructionid == lsu2[0].instructionid){
          reservationlsu[j].rdestination = -1;
          reservationlsu[j].inuse = 0;
          reservationlsu[j].inExecute = 0;
          reservationlsu[j].instructionid = -1;
          break;
        }
      }
      if(j == STORE_RESERVATION_WIDTH){
        printf("%d\n",lsu2[0].instructionid);
        for(int i = 0; i < STORE_RESERVATION_WIDTH; i++){
          if(reservationlsu[i].inuse == 1)printf("%d\n", reservationlsu[i].instructionid);
        }
        printf("ERROR when removing from load store reservation station in graduate\n");
        exit_early();
      }
    }
  }

  printf("B:%d\n",b5);

  // printring(allInOrder);
  int i = 0;
  for(; i < STORE_RESERVATION_WIDTH; i++){
    if(reservationlsu[i].inuse){
      continue;
    }
  }
  if(i == STORE_RESERVATION_WIDTH && b5 == 20){
    // exit_early();
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
