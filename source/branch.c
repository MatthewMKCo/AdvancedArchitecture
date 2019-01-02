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
          return 0;
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
        for(int j = 0; j < sizeOfBranchCache; j++){
          if(branchCache[j].pc == fedPC){
            pc[0] = branchCache[j].branchpc;
            return 1;
          }
        }
        pc[0] = fedPC + 4;
        return 0;
      }
    }

    if(BRANCH_PREDICTOR == 2){
      if(is_decode){
        int return_flag = 0;
        for(int i = 0; i < sizeOfBranchCache; i++){
          if(branchCache[i].pc == fedPC){
            return 0;
          }
        }
        if(offset > 0){
          pc[0] = pc[0];
          return_flag = 1;
        }
        for(int i = 0; i < sizeOfBranchCache; i++){
          if(branchCache[i].pc == fedPC){
            pc[0] = branchCache[i].branchpc;
            if(return_flag == 0)return 1;
            else return 0;
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
          if(return_flag == 0)return 1;
          else return 0;
        }

        int j = 0;
        for(; j < sizeOfBranchCache; j++){
          if(branchCache[j].inuse == 0){
            branchCache[j].pc = fedPC;
            branchCache[j].branchpc = fedPC + offset;
            branchCache[j].inuse = 1;
            if(return_flag == 0){
              pc[0] = branchCache[j].branchpc;
              return 1;
            }
            else return 0;
          }
        }

        if(j == sizeOfBranchCache){
          skip = 1;
        }

        branchCache[x].pc = fedPC;
        branchCache[x].branchpc = fedPC + offset;
        x++;
        if(return_flag == 0){
          pc[0] = branchCache[j].branchpc;
          return 1;
        }
        else return 0;


        }

        else{
          for(int j = 0; j < sizeOfBranchCache; j++){
            if(branchCache[j].pc == fedPC){
              if(branchCache[j].branchpc < fedPC){
                pc[0] = branchCache[j].branchpc;
                return 1;
              }
              else break;
            }
          }
          pc[0] = pc[0] + 4;
          return 0;
        }
      }

      if(BRANCH_PREDICTOR == 3){
        if(is_decode){
          for(int j = 0; j < sizeOfBranchCache; j++){
            if(branchCache[j].pc == fedPC){
              return 0;
              if(branchCache[j].accepted_before == 2 || branchCache[j].accepted_before == 3){
                pc[0] = branchCache[j].branchpc;
                return 1;
              }
              else return 0;
            }
          }
          if(offset > 0){
            pc[0] = pc[0];
            for(int i = 0; i < sizeOfBranchCache; i++){
              if(branchCache[i].inuse == 0){
                branchCache[i].inuse = 1;
                branchCache[i].accepted_before = 0;
                branchCache[i].branchpc = fedPC + offset;
                branchCache[i].pc = fedPC;
                break;
              }
            }
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
            branchCache[x].accepted_before = 3;
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
              branchCache[j].accepted_before = 3;
              pc[0] = branchCache[j].branchpc;
              return 1;
            }
          }

          if(j == sizeOfBranchCache){
            skip = 1;
          }

          branchCache[x].pc = fedPC;
          branchCache[x].branchpc = fedPC + offset;
          branchCache[x].accepted_before = 3;
          pc[0] = branchCache[j].branchpc;
          x++;
          return 1;


          }

          else{
            for(int j = 0; j < sizeOfBranchCache; j++){
              if(branchCache[j].pc == fedPC){
                if(branchCache[j].accepted_before == 2 || branchCache[j].accepted_before == 3){
                  pc[0] = branchCache[j].branchpc;
                  return 1;
                }
                else break;
              }
            }
            pc[0] = pc[0] + 4;
            return 0;
          }
        }

      if(BRANCH_PREDICTOR == 4){
        if(is_decode){
          for(int j = 0; j < sizeOfBranchCache; j++){
            if(branchCache[j].inuse == 1){
              if(branchCache[j].pc == fedPC){
                return 0;


                  if(branchCache[j].history[0] == -1){
                    if(branchCache[j].history[1] == 1){
                      if(branchCache[j].accepted_twolevel[1][0] == 2 || branchCache[j].accepted_twolevel[1][0] == 3 || branchCache[j].accepted_twolevel[3][0] == 2 || branchCache[j].accepted_twolevel[3][0] == 3){
                        pc[0] = branchCache[j].branchpc;
                        return 1;
                      }
                    }
                    else if(branchCache[j].history[1] == 0){
                      if(branchCache[j].accepted_twolevel[0][0] == 2 || branchCache[j].accepted_twolevel[0][0] == 3 || branchCache[j].accepted_twolevel[2][0] == 2 || branchCache[j].accepted_twolevel[2][0] == 3){
                        pc[0] = branchCache[j].branchpc;
                        return 1;
                      }
                    }
                    else return 0;
                  }
                  else if(branchCache[j].history[0] == 0 && branchCache[j].history[1] == 0){
                    if(branchCache[j].accepted_twolevel[0][0] == 2 || branchCache[j].accepted_twolevel[0][0] == 3){
                      pc[0] = branchCache[j].branchpc;
                      return 1;
                    }
                    else return 0;
                  }
                  else if(branchCache[j].history[0] == 0 && branchCache[j].history[1] == 1){
                    if(branchCache[j].accepted_twolevel[1][0] == 2 || branchCache[j].accepted_twolevel[1][0] == 3){
                      pc[0] = branchCache[j].branchpc;
                      return 1;
                    }
                    else return 0;
                  }
                  else if(branchCache[j].history[0] == 1 && branchCache[j].history[1] == 0){
                    if(branchCache[j].accepted_twolevel[2][0] == 2 || branchCache[j].accepted_twolevel[2][0] == 3){
                      pc[0] = branchCache[j].branchpc;
                      return 1;
                    }
                    else return 0;
                  }
                  else if(branchCache[j].history[0] == 1 && branchCache[j].history[1] == 1){
                    if(branchCache[j].accepted_twolevel[3][0] == 2 || branchCache[j].accepted_twolevel[3][0] == 3){
                      pc[0] = branchCache[j].branchpc;
                      return 1;
                    }
                    else return 0;
                  }
                  else{
                    pc[0] = pc[0];
                    return 0;
                  }
              }
            }
          }
          if(offset > 0){
            pc[0] = pc[0];
            for(int i = 0; i < sizeOfBranchCache; i++){
              if(branchCache[i].inuse == 0){
                branchCache[i].inuse = 1;
                branchCache[i].accepted_twolevel[0][0] = 0;
                branchCache[i].accepted_twolevel[1][0] = 0;
                branchCache[i].accepted_twolevel[2][0] = 0;
                branchCache[i].accepted_twolevel[3][0] = 0;
                branchCache[i].branchpc = fedPC + offset;
                branchCache[i].pc = fedPC;
                break;
              }
            }
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
            branchCache[x].accepted_twolevel[0][0] = 3;
            branchCache[x].accepted_twolevel[1][0] = 3;
            branchCache[x].accepted_twolevel[2][0] = 3;
            branchCache[x].accepted_twolevel[3][0] = 3;
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
              branchCache[j].accepted_twolevel[0][0] = 3;
              branchCache[j].accepted_twolevel[1][0] = 3;
              branchCache[j].accepted_twolevel[2][0] = 3;
              branchCache[j].accepted_twolevel[3][0] = 3;
              pc[0] = branchCache[j].branchpc;
              return 1;
            }
          }

          if(j == sizeOfBranchCache){
            skip = 1;
          }

          branchCache[x].pc = fedPC;
          branchCache[x].branchpc = fedPC + offset;
          branchCache[x].accepted_twolevel[0][0] = 3;
          branchCache[x].accepted_twolevel[1][0] = 3;
          branchCache[x].accepted_twolevel[2][0] = 3;
          branchCache[x].accepted_twolevel[3][0] = 3;
          pc[0] = branchCache[j].branchpc;
          x++;
          return 1;


          }

          else{
            for(int j = 0; j < sizeOfBranchCache; j++){
              if(branchCache[j].pc == fedPC){
                if(branchCache[j].history[0] == -1){
                  if(branchCache[j].branchpc < fedPC){
                    pc[0] = branchCache[j].branchpc;
                    return 1;
                  }
                  else{
                    pc[0] = pc[0] + 4;
                    return 0;
                  }
                }
                else if(branchCache[j].history[0] == 0 && branchCache[j].history[1] == 0){
                  if(branchCache[j].accepted_twolevel[0][0] == 2 || branchCache[j].accepted_twolevel[0][0] == 3){
                    pc[0] = branchCache[j].branchpc;
                    return 1;
                  }
                  else{
                    pc[0] = pc[0] + 4;
                    return 0;
                  }
                }
                else if(branchCache[j].history[0] == 0 && branchCache[j].history[1] == 1){
                  if(branchCache[j].accepted_twolevel[1][0] == 2 || branchCache[j].accepted_twolevel[1][0] == 3){
                    pc[0] = branchCache[j].branchpc;
                    return 1;
                  }
                  else{
                    pc[0] = pc[0] + 4;
                    return 0;
                  }
                }
                else if(branchCache[j].history[0] == 1 && branchCache[j].history[1] == 0){
                  if(branchCache[j].accepted_twolevel[2][0] == 2 || branchCache[j].accepted_twolevel[2][0] == 3){
                    pc[0] = branchCache[j].branchpc;
                    return 1;
                  }
                  else{
                    pc[0] = pc[0] + 4;
                    return 0;
                  }
                }
                else if(branchCache[j].history[0] == 1 && branchCache[j].history[1] == 1){
                  if(branchCache[j].accepted_twolevel[3][0] == 2 || branchCache[j].accepted_twolevel[3][0] == 3){
                    pc[0] = branchCache[j].branchpc;
                    return 1;
                  }
                  else{
                    pc[0] = pc[0] + 4;
                    return 0;
                  }
                }
                else break;
              }
            }
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

int check_purge_accepted(int pc, int offset, int accepted){
  if(BRANCH_PREDICTOR == 0)return 1;
  if(BRANCH_PREDICTOR == 1){
    return 0;
  }
  if(BRANCH_PREDICTOR == 2){
    if(offset < 0)return 0;
    else return 1;
  }
  if(BRANCH_PREDICTOR == 3){
        if(accepted)return 0;
        else return 1;
  }
  if(BRANCH_PREDICTOR == 4){
        if(accepted)return 0;
        else return 1;
      }

}

int check_purge_rejected(int pc, int offset, int accepted){
  if(BRANCH_PREDICTOR == 0)return 0;
  if(BRANCH_PREDICTOR == 1){
    return 1;
  }
  if(BRANCH_PREDICTOR == 2){
    if(offset < 0)return 1;
    else return 0;
  }
  if(BRANCH_PREDICTOR == 3){
        if(accepted)return 1;
        else return 0;
  }
  if(BRANCH_PREDICTOR == 4){
        if(accepted)return 1;
        else return 0;
  }
}


void check_purge_accepted2(int pc){
  if(BRANCH_PREDICTOR == 3){
    for(int i = 0; i < sizeOfBranchCache; i++){
      if(branchCache[i].pc == pc){
        if(branchCache[i].accepted_before != 3)branchCache[i].accepted_before++;
        return;
      }
    }
    printf("ERROR IN BRANCH PREDICTOR 3 ON ACCEPTED\n");
    exit_early();
  }
  if(BRANCH_PREDICTOR == 4){
    for(int i = 0; i < sizeOfBranchCache; i ++){
      if(branchCache[i].pc == pc){
        if(branchCache[i].bit == 2){
            if(branchCache[i].history[0] == 1 && branchCache[i].history[1] == 1){
              if(branchCache[i].accepted_twolevel[3][0] != 3)branchCache[i].accepted_twolevel[3][0]++;
            }
            else if(branchCache[i].history[0] == 0 && branchCache[i].history[1] == 0){
              if(branchCache[i].accepted_twolevel[0][0] != 3)branchCache[i].accepted_twolevel[0][0]++;
            }
            else if(branchCache[i].history[0] == 1 && branchCache[i].history[1] == 0){
              if(branchCache[i].accepted_twolevel[2][0] != 3)branchCache[i].accepted_twolevel[2][0]++;
            }
            else if(branchCache[i].history[0] == 0 && branchCache[i].history[1] == 1){
              if(branchCache[i].accepted_twolevel[1][0] != 3)branchCache[i].accepted_twolevel[1][0]++;
            }
            else{
              printf("%d\n",branchCache[i].history[1]);
              printf("%d\n",branchCache[i].history[0]);

              printf("ERROR IN BRANCH PREDICTOR 4 ON ACCEPTED\n");
              exit_early();
            }
        }
        if(branchCache[i].bit == 1 || branchCache[i].bit == 2){
          branchCache[i].history[0] = branchCache[i].history[1];
        }
        branchCache[i].history[1] = 1;
        if(branchCache[i].bit == 1)branchCache[i].bit = 2;
        if(branchCache[i].bit == 0)branchCache[i].bit = 1;
        return;
      }
    }

    printf("ERROR IN BRANCH PREDICTOR 4 ON ACCEPTED\n");
    exit_early();
  }

}

void check_purge_rejected2(int pc){
  if(BRANCH_PREDICTOR == 3){
    for(int i = 0; i < sizeOfBranchCache; i++){
      if(branchCache[i].pc == pc){
        if(branchCache[i].accepted_before != 0)branchCache[i].accepted_before--;
        return;
      }
    }
    printf("ERROR IN BRANCH PREDICTOR 3 ON REJECTED\n");
    exit_early();
  }
  if(BRANCH_PREDICTOR == 4){
    for(int i = 0; i < sizeOfBranchCache; i ++){
      if(branchCache[i].pc == pc){
        if(branchCache[i].bit == 2){
          if(branchCache[i].history[0] == 1 && branchCache[i].history[1] == 0){
            if(branchCache[i].accepted_twolevel[2][0] != 0)branchCache[i].accepted_twolevel[2][0]--;
          }
          else if(branchCache[i].history[0] == 1 && branchCache[i].history[1] == 1){
            if(branchCache[i].accepted_twolevel[3][0] != 0)branchCache[i].accepted_twolevel[3][0]--;
          }
          else if(branchCache[i].history[0] == 0 && branchCache[i].history[1] == 1){
            if(branchCache[i].accepted_twolevel[1][0] != 0)branchCache[i].accepted_twolevel[1][0]--;
          }
          else if(branchCache[i].history[0] == 0 && branchCache[i].history[1] == 0){
            if(branchCache[i].accepted_twolevel[0][0] != 0)branchCache[i].accepted_twolevel[0][0]--;
          }
          else{
            printf("ERROR IN BRANCH PREDICTOR 4 ON REJECTED\n");
            exit_early();
          }
        }
        if(branchCache[i].bit == 1 || branchCache[i].bit == 2){
          branchCache[i].history[0] = branchCache[i].history[1];
        }
        branchCache[i].history[1] = 0;
        if(branchCache[i].bit == 1)branchCache[i].bit = 2;
        if(branchCache[i].bit == 0)branchCache[i].bit = 1;
        return;
      }
    }
    printf("ERROR IN BRANCH PREDICTOR 4 ON REJECTED\n");
    exit_early();
  }
}




int change_pc_execute(int pc, int offset, int accepted){
  if(BRANCH_PREDICTOR == 0)return pc + offset;
  if(BRANCH_PREDICTOR == 1)return pc + 4;
  if(BRANCH_PREDICTOR == 2){
    if(offset < 0)return pc + 4;
    else return pc + offset;
  }
  if(BRANCH_PREDICTOR == 3){
    if(accepted == 1){
      return pc + 4;
    }
    else{
      return pc + offset;
    }
  }
  if(BRANCH_PREDICTOR == 4){
    if(accepted == 1){
      return pc + 4;
    }
    else{
      return pc + offset;
    }
  }
}
