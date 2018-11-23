#include "run.h"

void print_reg_summary(){
  //Print all register values
  for(int i = 0; i < 31; i++){
    switch(i){
      case(0):
        printf("zero:%d\t", registers[i]);
        break;
      case(1):
        printf("ra:%d\t", registers[i]);
        break;
      case(2):
        printf("sp:%d\t", registers[i]);
        break;
      case(3):
        printf("gp:%d\t", registers[i]);
        break;
      case(4):
        printf("tp:%d\t", registers[i]);
        break;
      case(5):
        printf("t0:%d\t", registers[i]);
        break;
      case(6):
        printf("t1:%d\t", registers[i]);
        break;
      case(7):
        printf("t2:%d\t", registers[i]);
        break;
      case(8):
        printf("s0:%d\t", registers[i]);
        break;
      case(9):
        printf("s1:%d\t", registers[i]);
        break;
      case(10):
        printf("a0:%d\t", registers[i]);
        break;
      case(11):
        printf("a1:%d\t", registers[i]);
        break;
      case(12):
        printf("a2:%d\t", registers[i]);
        break;
      case(13):
        printf("a3:%d\t", registers[i]);
        break;
      case(14):
        printf("a4:%d\t", registers[i]);
        break;
      case(15):
        printf("a5:%d\t", registers[i]);
        break;
      case(16):
        printf("a6:%d\t", registers[i]);
        break;
      case(17):
        printf("a7:%d\t", registers[i]);
        break;
      case(18):
        printf("s2:%d\t", registers[i]);
        break;
      case(19):
        printf("s3:%d\t", registers[i]);
        break;
      case(20):
        printf("s4:%d\t", registers[i]);
        break;
      case(21):
        printf("s5:%d\t", registers[i]);
        break;
      case(22):
        printf("s6:%d\t", registers[i]);
        break;
      case(23):
        printf("s7:%d\t", registers[i]);
        break;
      case(24):
        printf("s8:%d\t", registers[i]);
        break;
      case(25):
        printf("s9:%d\t", registers[i]);
        break;
      case(26):
        printf("s10:%d\t", registers[i]);
        break;
      case(27):
        printf("s11:%d\t", registers[i]);
        break;
      case(28):
        printf("t3:%d\t", registers[i]);
        break;
      case(29):
        printf("t4:%d\t", registers[i]);
        break;
      case(30):
        printf("t5:%d\t", registers[i]);
        break;
      case(31):
        printf("t6:%d\t", registers[i]);
        break;
    }
    if((i+1) % 5 == 0)printf("\n");
  }
  printf("\n");
}

void move_next_to_current(){
  //fetch
  executed_instruction = decode_instruction;
  decode_instruction = fetch_instruction;
  executepc = decodepc;
  decodepc = fetchpc;

  //decode
  execute_rsource1 = decode_rsource1;
  execute_rsource2 = decode_rsource2;
  execute_rdestination = decode_rdestination;
  execute_instruction_type = decode_instruction_type;
  execute_opcode = decode_opcode;
  execute_funct3 = decode_funct3;
  execute_funct7 = decode_funct7;
  execute_shamt = decode_shamt;
  execute_imm = decode_imm;

  //execute
  mem_acc_val = execute_val;
  //memory access

  //write-back
  return;
}
