#include "run.h"

void decode(){
  if(first_fetch < 2){
    print_decode_summary = 0;
    return;
  }
  if(first_decode < 2){
    first_decode = first_decode + 1;
  }
  if(last_instruction == 1){
    if(current_cycle > last_instruction_cycle + 1){
      print_decode_summary = 0;
      return;
    }
  }
  print_decode_summary = 1;

  decode_opcode = (decode_instruction & 0x0000007F);
  switch(decode_opcode){
    case 0b0010011:
      decode_instruction_type = 1;
      instruction_type_char = 'I';
      break;
    case 0b0000011:
      decode_instruction_type = 1;
      instruction_type_char = 'I';
      break;
    case 0b0110111:
      decode_instruction_type = 2;
      instruction_type_char = 'U';
      break;
    case 0b0010111:
      decode_instruction_type = 2;
      instruction_type_char = 'U';
      break;
    case 0b0110011:
      decode_instruction_type = 3;
      instruction_type_char = 'R';
      break;
    case 0b1101111:
      decode_instruction_type = 4;
      instruction_type_char = 'J';
      break;
    case 0b1100011:
      decode_instruction_type = 5;
      instruction_type_char = 'B';
      break;
    case 0b0100011:
      decode_instruction_type = 6;
      instruction_type_char = 'S';
      break;
    default:
      decode_instruction_type = -1;
      break;
  }

  //I type instructions
  if(decode_instruction_type == 1){
    decode_rdestination = (decode_instruction & 0x00000F80) >> 7;
    decode_funct3 = (decode_instruction & 0x00007000) >> 12;
    decode_rsource1 = registers[(decode_instruction & 0x000F8000) >> 15];
    if(decode_funct3 == 0b101 || decode_funct3 == 0b001){
      decode_shamt = (decode_instruction & 0x01F00000) >> 20;
      decode_imm = (decode_instruction & 0xFE000000) >> 25;
    }
    else if(decode_opcode == 0b1100111 && decode_funct3 == 0b000){
      decode_imm = (decode_instruction & 0xFFF00000) >> 20;
      decode_imm = (decode_imm & 0x00000001);
    }
    else{
      decode_imm = (decode_instruction & 0xFFF00000) >> 20;
      if((decode_imm & 0x00000800)){
        decode_imm = (decode_imm | 0xFFFFF000);
      }
    }
  }
  //U type instructions
  else if(decode_instruction_type == 2){
    decode_rdestination = (decode_instruction & 0x00000F80) >> 7;
    decode_imm = (decode_instruction & 0xFFFFF000);
  }
  //R type instructions
  else if(decode_instruction_type == 3){
    decode_rdestination = (decode_instruction & 0x00000F80) >> 7;
    decode_funct3 = (decode_instruction & 0x00007000) >> 12;
    decode_rsource1 = registers[(decode_instruction & 0x000F8000) >> 15];
    decode_rsource2 = registers[(decode_instruction & 0x01F00000) >> 20];
    decode_funct7 = (decode_instruction & 0xFE000000) >> 25;
  }
  //J type instructions
  else if(decode_instruction_type == 4){
    decode_rdestination = (decode_instruction & 0x00000F80) >> 7;
    decode_imm = (((decode_instruction & 0x000FF000) >> 12 << 12) | ((decode_instruction & 0x00100000) >> 20 << 11) | ((decode_instruction & 0x7FE00000) >> 21 << 1) | ((decode_instruction & 0x80000000) >> 31 << 20));
    if((decode_imm & 0x00080000)){
      decode_imm = (decode_imm | 0xFFF00000);
    }
  }
  //B type instructions
  else if(decode_instruction_type == 5){
    decode_funct3 = (decode_instruction & 0x00007000) >> 12;
    decode_rsource1 = registers[(decode_instruction & 0x000F8000) >> 15];
    decode_rsource2 = registers[(decode_instruction & 0x01F00000) >> 20];
    decode_imm = (((decode_instruction & 0x00000080) >> 7 << 11) | ((decode_instruction & 0x00000F00) >> 8 << 1) | ((decode_instruction & 0x7E000000) >> 25 << 5) | ((decode_instruction & 0x10000000) >> 31 << 5));
    if((decode_imm & 0x00000800)){
      decode_imm = (decode_imm | 0xFFFFF000);
    }
  }
  //S type instructions
  else if(decode_instruction_type == 6){
    decode_funct3 = (decode_instruction & 0x00007000) >> 12;
    decode_rsource1 = registers[(decode_instruction & 0x000F8000) >> 15];
    decode_rsource2 = registers[(decode_instruction & 0x01F00000) >> 20];
    decode_imm = (((decode_instruction & 0x00000F80) >> 7) | ((decode_instruction & 0xFE000000) >> 25 << 5));
    if((decode_imm & 0x00000800)){
      decode_imm = (decode_imm | 0xFFFFF000);
    }
  }
}
