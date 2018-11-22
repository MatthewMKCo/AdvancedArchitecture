#include "run.h"

void decode(){
  if(first_fetch < 2)return;
  if(last_instruction == 1){
    if(current_cycle > last_instruction_cycle + 1)return;
  }
  if(first_decode < 2){
    first_decode = first_decode + 1;
  }

  printf("instruction:%x\n",fetch_current_instruction);

  next_opcode = (fetch_current_instruction & 0x0000007F);
  printf("Opcode:%d\n",next_opcode);
  switch(next_opcode){
    case 0b0010011:
      next_instruction_type = 1;
      instruction_type_char = 'I';
      break;
    case 0b0000011:
      next_instruction_type = 1;
      instruction_type_char = 'I';
      break;
    case 0b0110111:
      next_instruction_type = 2;
      instruction_type_char = 'U';
      break;
    case 0b0010111:
      next_instruction_type = 2;
      instruction_type_char = 'U';
      break;
    case 0b0110011:
      next_instruction_type = 3;
      instruction_type_char = 'R';
      break;
    case 0b1101111:
      next_instruction_type = 4;
      instruction_type_char = 'J';
      break;
    case 0b1100011:
      next_instruction_type = 5;
      instruction_type_char = 'B';
      break;
    case 0b0100011:
      next_instruction_type = 6;
      instruction_type_char = 'S';
      break;
    default:
      next_instruction_type = -1;
      break;
  }

  //I type instructions
  if(next_instruction_type == 1){
    next_rdestination = (fetch_current_instruction & 0x00000F80) >> 7;
    next_funct3 = (fetch_current_instruction & 0x00007000) >> 12;
    next_rsource1 = (fetch_current_instruction & 0x000F8000) >> 15;
    if(next_funct3 == 0b101 || next_funct3 == 0b001){
      next_shamt = (fetch_current_instruction & 0x01F00000) >> 20;
      next_imm = (fetch_current_instruction & 0xFE000000) >> 25;
    }
    else if(next_opcode == 0b1100111 && next_funct3 == 0b000){
      next_imm = (fetch_current_instruction & 0xFFF00000) >> 20;
      next_imm = (next_imm & 0x00000001);
    }
    else{
      next_imm = (fetch_current_instruction & 0xFFF00000) >> 20;
      if((next_imm & 0x00000800)){
        next_imm = (next_imm | 0xFFFFF000);
      }
    }
  }
  //U type instructions
  else if(next_instruction_type == 2){
    next_rdestination = (fetch_current_instruction & 0x00000F80) >> 7;
    next_imm = (fetch_current_instruction & 0xFFFFF000);
  }
  //R type instructions
  else if(next_instruction_type == 3){
    next_rdestination = (fetch_current_instruction & 0x00000F80) >> 7;
    next_funct3 = (fetch_current_instruction & 0x00007000) >> 12;
    next_rsource1 = (fetch_current_instruction & 0x000F8000) >> 15;
    next_rsource2 = (fetch_current_instruction & 0x01F00000) >> 20;
    next_funct7 = (fetch_current_instruction & 0xFE000000) >> 25;
  }
  //J type instructions
  else if(next_instruction_type == 4){
    next_rdestination = (fetch_current_instruction & 0x00000F80) >> 7;
    next_imm = (((fetch_current_instruction & 0x000FF000) >> 12 << 12) | ((fetch_current_instruction & 0x00100000) >> 20 << 11) | ((fetch_current_instruction & 0x7FE00000) >> 21 << 1) | ((fetch_current_instruction & 0x80000000) >> 31 << 20));
    if((next_imm & 0x00080000)){
      next_imm = (next_imm | 0xFFF00000);
    }
  }
  //B type instructions
  else if(next_instruction_type == 5){
    next_funct3 = (fetch_current_instruction & 0x00007000) >> 12;
    next_rsource1 = (fetch_current_instruction & 0x000F8000) >> 15;
    next_rsource2 = (fetch_current_instruction & 0x01F00000) >> 20;
    next_imm = (((fetch_current_instruction & 0x00000080) >> 7 << 11) | ((fetch_current_instruction & 0x00000F00) >> 8 << 1) | ((fetch_current_instruction & 0x7E000000) >> 25 << 5) | ((fetch_current_instruction & 0x10000000) >> 31 << 5));
    if((next_imm & 0x00000800)){
      next_imm = (next_imm | 0xFFFFF000);
    }
  }
  //S type instructions
  else if(next_instruction_type == 6){
    next_funct3 = (fetch_current_instruction & 0x00007000) >> 12;
    next_rsource1 = (fetch_current_instruction & 0x000F8000) >> 15;
    next_rsource2 = (fetch_current_instruction & 0x01F00000) >> 20;
    next_imm = (((fetch_current_instruction & 0x00000F80) >> 7) | ((fetch_current_instruction & 0xFE000000) >> 25 << 5));
    if((next_imm & 0x00000800)){
      next_imm = (next_imm | 0xFFFFF000);
    }
  }
}
