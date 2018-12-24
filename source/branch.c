#include "run.h"

branchDict branchCache[sizeOfBranchCache];
int x = 0;
int skip = 0;

int branch_predictor(int fedPC, int is_decode, int offset){
  //Does not accept branch
  if(BRANCH_PREDICTOR == 0){
    if(is_decode == 0)pc[0] = fedPC + 4;
    return 0;
  }
  //Always accepts branch
  if(BRANCH_PREDICTOR == 1){
    if(is_decode){
      for(int i = 0; i < sizeOfBranchCache; i++){
        if(branchCache[i].pc == fedPC){
          pc[0] = branchCache[i].branchpc;
          return 1;
        }
      }

      if(skip == 1){
        if(x == sizeOfBranchCache){
          x = 0;
        }
        branchCache[x].pc = fedPC;
        branchCache[x].branchpc = fedPC + offset;
        x++;
        return 1;
      }

      int j = 0;
      for(; j < sizeOfBranchCache; j++){
        if(branchCache[j].inuse == 0){
          branchCache[j].pc = fedPC;
          branchCache[j].branchpc = fedPC + offset;
          branchCache[j].inuse = 1;
          return 1;
        }
      }

      if(j == sizeOfBranchCache){
        skip = 1;
      }

      branchCache[x].pc = fedPC;
      branchCache[x].branchpc = fedPC + offset;
      x++;
      return 1;


      }

      else{
        pc[0] = pc[0] + 4;
        return 0;
      }
    }


    else{
      pc[0] = pc[0] + 4;
      return 0;
    }
    // else{
    //   for(int i = 0; i < sizeOfBranchCache; i++){
    //     if(branchCache[i].pc == fedPC){
    //
    //     }
    //   }
    // }
  }
