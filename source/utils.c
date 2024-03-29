#include "run.h"

//sets registers to 0
void set_register(){
  for(int i = 0; i < REG_NUM; i++){
    registers[i] = 0;
  }
  for(int i = 0; i < PHYSREG_NUM; i++){
    physRegisters[i].ready = 1;
    physRegisters[i].value = 0;
  }
  for(int i = 0; i < SIZE; i++){
    st(Icache, i, -1);
  }
  sp[0] = SIZE;
  pc[0] = 4;

  for(int i = 0; i < ALU_NUM; i++){
    alu[i].cyclesNeeded = 0;
    alu[i].currentCycles = 0;
    alu[i].ready = 1;
  }
  for(int i = 0; i < LSU_NUM; i++){
    lsu[i].cyclesNeeded = 0;
    lsu[i].currentCycles = 0;
    lsu[i].ready = 1;
  }
  for(int i = 0; i < LSU2_NUM; i++){
    lsu2[i].cyclesNeeded = 0;
    lsu2[i].currentCycles = 0;
    lsu2[i].ready = 1;
  }
  for(int i = 0; i < BRU_NUM; i++){
    bru[i].cyclesNeeded = 0;
    bru[i].currentCycles = 0;
    bru[i].ready = 1;
  }

  for(int i = 0; i < RESERVATION_WIDTH; i++){
    reservationalu[i].rsource1 = -1;
    reservationalu[i].rsource2 = -1;
    reservationalu[i].rsource1ready = 0;
    reservationalu[i].rsource2ready = 0;
  }

  for(int i = 0; i < sizeOfBranchCache; i++){
    branchCache[i].bit = 0;
    branchCache[i].history[0] = -1;
    branchCache[i].history[1] = -1;
  }

  for(int i = 0; i < NWAY; i++){
    fetch_inuse[i] = 0;
    fetch_branch[i] = 0;
    decode_inuse[i] = 0;
    decode_branch[i] = 0;
    issue_inuse[i] = 0;
    decode_decoded[i] = 0;
  }

}

void load_program(char* file){
  FILE* fp;
  fp = fopen(file, "r");
  unsigned int tinstruction, loadpc = 4;
  next_imm = 0;
  if(fp == NULL){
    printf("Error in loading program, file is null");
    exit(1);
  }
  while(fscanf(fp, "%x", &tinstruction) != EOF){
    st(Icache, loadpc, tinstruction);
    loadpc = loadpc + 4;
  }

  fclose(fp);
}

void print_reg_summary(){
  printf("Cycle:%d\n", current_cycle);
  if(last_instruction != 1){
    printf("Current PC in Fetch:%d\n",fetchpc[NWAY - 1]);
  }
  if(print_decode_summary){
    for(int i = 0; i < NWAY; i++){
    printf("Decode Instruction:%x\n",decode_instruction[i]);
  }
    // printf("Decode Opcode:%d\n",decode_opcode);
  }
  if(print_execute_summary){
    for(int i = 0; i < numberOfExecutedInstructions; i++){
      printf("Instruction Being Executed:%s\n", executed_instruction_name[i]);
    }
  }

  if(NOTPRINT != 1){
  printf("Architectural Registers\n");
  //Print all register values
  for(int i = 0; i < 31; i++){
    switch(i){
      case(0):
        printf("zero:%-6d\t", registers[i]);
        break;
      case(1):
        printf("ra:%-6d\t", registers[i]);
        break;
      case(2):
        printf("sp:%-6d\t", registers[i]);
        break;
      case(3):
        printf("gp:%-6d\t", registers[i]);
        break;
      case(4):
        printf("tp:%-6d\t", registers[i]);
        break;
      case(5):
        printf("t0:%-6d\t", registers[i]);
        break;
      case(6):
        printf("t1:%-6d\t", registers[i]);
        break;
      case(7):
        printf("t2:%-6d\t", registers[i]);
        break;
      case(8):
        printf("s0:%-6d\t", registers[i]);
        break;
      case(9):
        printf("s1:%-6d\t", registers[i]);
        break;
      case(10):
        printf("a0:%-6d\t", registers[i]);
        break;
      case(11):
        printf("a1:%-6d\t", registers[i]);
        break;
      case(12):
        printf("a2:%-6d\t", registers[i]);
        break;
      case(13):
        printf("a3:%-6d\t", registers[i]);
        break;
      case(14):
        printf("a4:%-6d\t", registers[i]);
        break;
      case(15):
        printf("a5:%-6d\t", registers[i]);
        break;
      case(16):
        printf("a6:%-6d\t", registers[i]);
        break;
      case(17):
        printf("a7:%-6d\t", registers[i]);
        break;
      case(18):
        printf("s2:%-6d\t", registers[i]);
        break;
      case(19):
        printf("s3:%-6d\t", registers[i]);
        break;
      case(20):
        printf("s4:%-6d\t", registers[i]);
        break;
      case(21):
        printf("s5:%-6d\t", registers[i]);
        break;
      case(22):
        printf("s6:%-6d\t", registers[i]);
        break;
      case(23):
        printf("s7:%-6d\t", registers[i]);
        break;
      case(24):
        printf("s8:%-6d\t", registers[i]);
        break;
      case(25):
        printf("s9:%-6d\t", registers[i]);
        break;
      case(26):
        printf("s10:%-6d\t", registers[i]);
        break;
      case(27):
        printf("s11:%-6d\t", registers[i]);
        break;
      case(28):
        printf("t3:%-6d\t", registers[i]);
        break;
      case(29):
        printf("t4:%-6d\t", registers[i]);
        break;
      case(30):
        printf("t5:%-6d\t", registers[i]);
        break;
      case(31):
        printf("t6:%-d\t", registers[i]);
        break;
    }
    if((i+1) % 5 == 0)printf("\n");
  }
  printf("pc:%-d\t", pc[0]);
  printf("\n");
  printf("Physical Registers\n");
  for(int i = 0; i < PHYSREG_NUM; i++){
    printf("Register %d", i);
    if(i < 10)printf(" ");
    printf(": %-6d\t", physRegisters[i].value);
    if((i + 1) % 5 == 0)printf("\n");
  }
  printf("\n");
  printf("Execution Units\n");
  for(int i = 0; i < ALU_NUM; i++){
    if(alu[i].ready == 1)printf("ALU %d:FREE\t", i);
    if(alu[i].ready == 0)printf("ALU %d:INUSE\t", i);
  }
  printf("\n");
  for(int i = 0; i < BRU_NUM; i++){
    if(bru[i].ready == 1)printf("BRU %d:FREE\t", i);
    if(bru[i].ready == 0)printf("BRU %d:INUSE\t", i);
  }
  printf("\n");
  for(int i = 0; i < LSU_NUM; i++){
    if(lsu[i].ready == 1)printf("LSU %d:FREE\t", i);
    if(lsu[i].ready == 0)printf("LSU %d:INUSE\t", i);
  }
  printf("\n");
  // for(int i = 0; i < NWAY; i++){
  //   printf("FETCH INUSE %d:%d\t", i, fetch_inuse[i]);
  // }
  // printf("\n");
  // for(int i = 0; i < NWAY; i++){
  //   printf("DECODE INUSE %d:%d ID:%-3d\t", i, decode_inuse[i], decode_instruction_struct[i].instructionid);
  // }
  // printf("\n");
  // for(int i = 0; i < NWAY; i++){
  //   printf("ISSUE INUSE %d :%d ID:%-3d\t", i, issue_inuse[i], issue_instruction_struct[i].instructionid);
  // }
  // printf("\n");

  // printf("%d\n", fetch_finished);
  // printf("%d\n", decode_finished);
  // printf("%d\n", issue_finished);
  // printf("%d\n", execute_finished);
  // printf("%d\n", writeback_finished);
  // printf("%d\n", graduate_finished);

  // for(int i = 0; i < BRANCH_RESERVATION_WIDTH; i++){
  //   if(reservationbru[i].instructionid == 21){
  //     printf("%d\n",i);
  //     exit_early();
  //   }
  // }
  // for(int i = 0; i < RESERVATION_WIDTH; i++){
  //   if(reservationalu[i].instructionid == 50){
  //     printf("%d\n", i );
  //     exit_early();
  //   }
  // }
}
}

void move_next_to_current(){
  // mem_rdestination = execute_rdestination;
  // mem_rsource1 = execute_rsource1;
  // mem_rsource2 = execute_rsource2;
  // mem_funct3 = execute_funct3;
  // mem_offset = execute_offset;
  // mem_access = execute_access;
  // mem_store = execute_store;
  // mem_load = execute_load;
  // printf("EXECUTE VALUE IN UTILS:%d\n",execute_val);
  // mem_acc_val = execute_val;
  // printf("MEMORY VALUE IN UTILS:%d\n",mem_acc_val);
  if(stall_from_issue != 0){
    stall_from_issue--;
    // exit_early();
  }
  change_to_ready(outOfOrderInstructions);
  graduate_destination = writeback_destination;

  send_for_writeback();

  if(first_decode >= 1){
    for(int i = 0; i < NWAY; i++){
      if(decode_inuse[i] == 1){
        for(int j = 0; j < NWAY; j++){
          if(issue_inuse[j] == 0){
            decode_decoded[i] = 0;
            decode_inuse[i] = 0;
            issue_inuse[j] = 1;
            issue_unit_type[j] = decode_unit_type[i];
            issue_instruction_struct[j] = decode_instruction_struct[i];
            break;
          }
        }
      }
    }

    for(int i = 0; i < NWAY; i++){
      if(decode_inuse[i] == 0){
        for(int j = i + 1; j < NWAY; j++){
          if(decode_inuse[j] == 1){

            decode_inuse[i] = 1;
            decode_inuse[j] = 0;
            decode_branch[i] = decode_branch[j];
            decode_instruction_struct[i] = decode_instruction_struct[j];
            decode_unit_type[i] = decode_unit_type[j];
            decode_decoded[i] = decode_decoded[j];
            decode_decoded[j] = 0;
            // if(current_cycle == 14 && j == 3)exit_early();
            break;
          }
        }
      }
    }

  }

  for(int i = 0; i < RESERVATION_WIDTH; i++){
    if(reservationalu[i].inuse == 1)reservationalu[i].inExecute = 1;
  }

  for(int i = 0; i < BRANCH_RESERVATION_WIDTH; i++){
    if(reservationbru[i].inuse == 1)reservationbru[i].inExecute = 1;
  }

  for(int i = 0; i < STORE_RESERVATION_WIDTH; i++){
    if(reservationlsu[i].inuse == 1)reservationlsu[i].inExecute = 1;
  }

    if((first_fetch >= 1) && block_fetch_to_decode == 0){
      for(int i = 0; i < NWAY; i++){
        if(fetch_inuse[i] == 1){
          for(int j = 0; j < NWAY; j++){
            if(decode_inuse[j] == 0){
              decode_inuse[j] = 1;
              fetch_inuse[i] = 0;
              decode_instruction[j] = fetch_instruction[i];
              decodepc[j] = fetchpc[i];
              decode_branch[j] = fetch_branch[i];
              break;
            }
          }
        }
      }

      for(int i = 0; i < NWAY; i++){
        if(fetch_inuse[i] == 0){
          for(int j = i + 1; j < NWAY; j++){
            if(fetch_inuse[j] == 1){
              fetch_inuse[i] = 1;
              fetch_inuse[j] = 0;
              fetch_instruction[i] = fetch_instruction[j];
              fetchpc[i] = fetchpc[j];
              fetch_branch[i] = fetch_branch[j];
              break;
            }
          }
        }
      }
    }

  return;
}

void exit_early(){
  printf("EXIT EARLY\n");
  exit(1);
}
int b = 0;
void purgepipe(){
  for(int i = 0; i < RESERVATION_WIDTH; i++){
    if(reservationalu[i].instruction.instructionid > purgeid){
      reservationalu[i].inuse = 0;
      reservationalu[i].inExecute = 0;
    }
  }
  for(int i = 0; i < BRANCH_RESERVATION_WIDTH; i++){
    if(reservationbru[i].instruction.instructionid > purgeid){
      reservationbru[i].inuse = 0;
      reservationbru[i].inExecute = 0;
    }
  }
  for(int i = 0; i < STORE_RESERVATION_WIDTH; i++){
    if(reservationlsu[i].instruction.instructionid > purgeid){
      reservationlsu[i].inuse = 0;
      reservationlsu[i].inExecute = 0;
    }
  }
  for(int i = 0; i < ALU_NUM; i++){
    if((alu[i].ready == 0 || alu[i].readyForWriteback == 1) && alu[i].instruction.instructionid > purgeid){
      alu[i].ready = 1;
      alu[i].readyForWriteback = 0;
      alu[i].shouldWriteback = 0;
      alu[i].currentCycles = 0;
    }
  }
  for(int i = 0; i < BRU_NUM; i++){
    if((bru[i].ready == 0 || bru[i].readyForWriteback == 1) && bru[i].instruction.instructionid > purgeid){
      bru[i].ready = 1;
      bru[i].readyForWriteback = 0;
      bru[i].shouldWriteback = 0;
      bru[i].currentCycles = 0;

    }
  }
  for(int i = 0; i < LSU_NUM; i++){
    if((lsu[i].ready == 0 || lsu[i].readyForWriteback == 1) && lsu[i].instruction.instructionid > purgeid){
      lsu[i].ready = 1;
      lsu[i].readyForWriteback = 0;
      lsu[i].shouldWriteback = 0;
      lsu[i].currentCycles = 0;

    }
  }
  for(int i = 0; i < JLU_NUM; i++){
    if((jlu[i].ready == 0 || jlu[i].readyForWriteback == 1) && jlu[i].instruction.instructionid > purgeid){
      jlu[i].ready = 1;
      jlu[i].readyForWriteback = 0;
      jlu[i].shouldWriteback = 0;
      jlu[i].currentCycles = 0;

    }
  }
  for(int i = 0; i < ALU_NUM; i++){
    if(writebackalu[i].ready == 1 && writebackalu[i].instructionid > purgeid){
      writebackalu[i].ready = 0;
    }
  }
  for(int i = 0; i < BRU_NUM; i++){
    if(writebackbru[i].ready == 1 && writebackbru[i].instructionid > purgeid){
      writebackbru[i].ready = 0;
    }
  }
  for(int i = 0; i < LSU_NUM; i++){
    if(writebacklsu[i].ready == 1 && writebacklsu[i].instructionid > purgeid){
      writebacklsu[i].ready = 0;
    }
  }
  for(int i = 0; i < JLU_NUM; i++){
    if(writebackjlu[i].ready == 1 && writebackjlu[i].instructionid > purgeid){
      writebackjlu[i].ready = 0;
    }
  }
  for(int i = 0; i < NWAY; i++){
    // fetch_instruction[i] = -1;
    // decode_instruction_struct[i].instruction_type = 0;
    // decode_instruction[i] = -1;
    // issue_instruction_struct[i].instruction_type = 0;
    issue_instruction_struct[i].rdestination = 0;
    decode_instruction_struct[i].instructionid = -1;
    issue_instruction_struct[i].instructionid = -1;
    fetch_inuse[i] = 0;
    decode_inuse[i] = 0;
    issue_inuse[i] = 0;
    decode_decoded[i] = 0;
    fetch_branch[i] = 0;
    decode_branch[i] = 0;

  }

  deletenodeswithgreaterthanid(purgeid);
  moveselectednode(inuseTags, unusedTags, purgeid);
  // exit_early();
  // printring(allInOrder);
  // printf("%d\n",purgeid);
// exit_early();
  // printring(outOfOrderInstructions);
  // if(b==1)exit_early();
  b++;
}
