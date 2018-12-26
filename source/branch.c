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
        pc[0] = branchCache[x].branchpc;
        x++;
        return 1;
      }

      int j = 0;
      for(; j < sizeOfBranchCache; j++){
        if(branchCache[j].inuse == 0){
          branchCache[j].pc = fedPC;
          branchCache[j].branchpc = fedPC + offset;
          branchCache[j].inuse = 1;
          pc[0] = branchCache[j].branchpc;
          return 1;
        }
      }

      if(j == sizeOfBranchCache){
        skip = 1;
      }

      branchCache[x].pc = fedPC;
      branchCache[x].branchpc = fedPC + offset;
      pc[0] = branchCache[j].branchpc;
      x++;
      return 1;


      }

      else{
        pc[0] = fedPC + 4;
        return 0;
      }
    }

    if(BRANCH_PREDICTOR == 2){
      if(is_decode){
        if(offset > 0){
          pc[0] = fedPC + 4;
          return 0;
        }
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
          pc[0] = branchCache[x].branchpc;
          x++;
          return 1;
        }

        int j = 0;
        for(; j < sizeOfBranchCache; j++){
          if(branchCache[j].inuse == 0){
            branchCache[j].pc = fedPC;
            branchCache[j].branchpc = fedPC + offset;
            branchCache[j].inuse = 1;
            pc[0] = branchCache[j].branchpc;
            return 1;
          }
        }

        if(j == sizeOfBranchCache){
          skip = 1;
        }

        branchCache[x].pc = fedPC;
        branchCache[x].branchpc = fedPC + offset;
        pc[0] = branchCache[j].branchpc;
        x++;
        return 1;


        }

        else{
          pc[0] = pc[0] + 4;
          return 0;
        }
      }

    else{
      printf("WRONG BRANCH PREDICTOR\n");
      exit(1);
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

int check_purge_accepted(int pc, int offset){
  if(BRANCH_PREDICTOR == 0)return 1;
  if(BRANCH_PREDICTOR == 1){
    return 0;
  }
  if(BRANCH_PREDICTOR == 2){
    if(offset < 0)return 0;
    else return 1;
  }
}

int check_purge_rejected(int pc, int offset){
  if(BRANCH_PREDICTOR == 0)return 0;
  if(BRANCH_PREDICTOR == 1){
    return 1;
  }
  if(BRANCH_PREDICTOR == 2){
    if(offset < 0)return 1;
    else return 0;
  }
}

int change_pc_execute(int pc, int offset){
  if(BRANCH_PREDICTOR == 0)return pc + offset;
  if(BRANCH_PREDICTOR == 1)return pc + 4;
  if(BRANCH_PREDICTOR == 2){
    if(offset < 0)return pc + 4;
    else return pc + offset;
  }
}
