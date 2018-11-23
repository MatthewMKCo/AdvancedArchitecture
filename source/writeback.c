#include "run.h"

void writeback(){
  if(first_execute < 2)return;
  if(writeback_rdestination != 0){
    registers[writeback_rdestination] = writeback_val;
  }
}
