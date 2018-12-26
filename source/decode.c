#include "run.h"

int decode_pcDestination;
int block_fetch_to_decode = 0;
// instruction decode_instruction[i];

void decode(){
  if(decode_finished)return;
  if(pc[0] == 0){
    if(current_cycle > last_instruction_cycle + 1){
      decode_finished = 1;
      for(int i = 0; i < NWAY; i ++){
        decode_instruction_struct[i].instruction_type = 0;
      }
      first_decode = 0;
      return;
    }
  }
  if(first_fetch < 2){
    print_decode_summary = 0;
    return;
  }
  if(first_decode < 2){
    first_decode = first_decode + 1;
  }
  if(stall_from_issue != 0){
    // first_decode = 0;
    return;
  }
  if(block_fetch_to_decode){
    block_fetch_to_decode = 0;
    block_decode_to_issue = 1;
    return;
  }
  print_decode_summary = 1;

  for(int i = 0; i < NWAY; i++){
  decode_opcode = (decode_instruction[i] & 0x0000007F);
  switch(decode_opcode){
    case 0b0010011:
      decode_instruction_type = 1;
      instruction_type_char = 'I';
      decode_unit_type[i] = 1;
      break;
    case 0b0000011:
      decode_instruction_type = 1;
      instruction_type_char = 'I';
      decode_unit_type[i] = 2;
      break;
    case 0b0110111:
      decode_instruction_type = 2;
      instruction_type_char = 'U';
      decode_unit_type[i] = 1;
      break;
    case 0b0010111:
      decode_instruction_type = 2;
      instruction_type_char = 'U';
      decode_unit_type[i] = 1;
      break;
    case 0b0110011:
      decode_instruction_type = 3;
      instruction_type_char = 'R';
      decode_unit_type[i] = 1;
      break;
    case 0b1101111:
      decode_instruction_type = 4;
      instruction_type_char = 'J';
      decode_unit_type[i] = 1;
      break;
    case 0b1100011:
      decode_instruction_type = 5;
      instruction_type_char = 'B';
      decode_unit_type[i] = 3;
      break;
    case 0b0100011:
      decode_instruction_type = 6;
      instruction_type_char = 'S';
      decode_unit_type[i] = 2;
      break;
    default:
      decode_instruction_type = -1;
      decode_unit_type[i] = -1;
      break;
  }

  if(decode_instruction[i] == -1){
    decode_unit_type[i] = 0;
  }

  //I type instructions
  if(decode_instruction_type == 1){
    decode_rdestination = (decode_instruction[i] & 0x00000F80) >> 7;
    decode_funct3 = (decode_instruction[i] & 0x00007000) >> 12;
    decode_rsource1 = (decode_instruction[i] & 0x000F8000) >> 15;
    if(decode_funct3 == 0b101 || decode_funct3 == 0b001){
      decode_shamt = (decode_instruction[i] & 0x01F00000) >> 20;
      decode_imm = (decode_instruction[i] & 0xFE000000) >> 25;
    }
    else if(decode_opcode == 0b1100111 && decode_funct3 == 0b000){
      decode_imm = (decode_instruction[i] & 0xFFF00000) >> 20;
      decode_imm = (decode_imm & 0x00000001);
    }
    else{
      decode_imm = (decode_instruction[i] & 0xFFF00000) >> 20;
      if((decode_imm & 0x00000800)){
        decode_imm = (decode_imm | 0xFFFFF000);
      }
    }
  }
  //U type instructions
  else if(decode_instruction_type == 2){
    decode_rdestination = (decode_instruction[i] & 0x00000F80) >> 7;
    decode_imm = (decode_instruction[i] & 0xFFFFF000);
  }
  //R type instructions
  else if(decode_instruction_type == 3){
    decode_rdestination = (decode_instruction[i] & 0x00000F80) >> 7;
    decode_funct3 = (decode_instruction[i] & 0x00007000) >> 12;
    decode_rsource1 = (decode_instruction[i] & 0x000F8000) >> 15;
    decode_rsource2 = (decode_instruction[i] & 0x01F00000) >> 20;
    decode_funct7 = (decode_instruction[i] & 0xFE000000) >> 25;
  }
  //J type instructions
  else if(decode_instruction_type == 4){
    decode_rdestination = (decode_instruction[i] & 0x00000F80) >> 7;
    decode_imm = (((decode_instruction[i] & 0x000FF000) >> 12 << 12) | ((decode_instruction[i] & 0x00100000) >> 20 << 11) | ((decode_instruction[i] & 0x7FE00000) >> 21 << 1) | ((decode_instruction[i] & 0x80000000) >> 31 << 20));
    if((decode_imm & 0x00080000)){
      decode_imm = (decode_imm | 0xFFF00000);
    }
    decode_pcDestination = REG_NUM;
  }
  //B type instructions
  else if(decode_instruction_type == 5){
    decode_funct3 = (decode_instruction[i] & 0x00007000) >> 12;
    decode_rsource1 = (decode_instruction[i] & 0x000F8000) >> 15;
    decode_rsource2 = (decode_instruction[i] & 0x01F00000) >> 20;
    // decode_rdestination = REG_NUM;
    decode_imm = (((decode_instruction[i] & 0x00000080) >> 7 << 11) | ((decode_instruction[i] & 0x00000F00) >> 8 << 1) | ((decode_instruction[i] & 0x7E000000) >> 25 << 5) | ((decode_instruction[i] & 0x10000000) >> 31 << 5));
    if((decode_imm & 0x00000800)){
      decode_imm = (decode_imm | 0xFFFFF000);
    }
  }
  //S type instructions
  else if(decode_instruction_type == 6){
    decode_funct3 = (decode_instruction[i] & 0x00007000) >> 12;
    decode_rsource1 = (decode_instruction[i] & 0x000F8000) >> 15;
    decode_rsource2 = (decode_instruction[i] & 0x01F00000) >> 20;
    decode_imm = (((decode_instruction[i] & 0x00000F80) >> 7) | ((decode_instruction[i] & 0xFE000000) >> 25 << 5));
    if((decode_imm & 0x00000800)){
      decode_imm = (decode_imm | 0xFFFFF000);
    }
  }
  decode_instruction_struct[i].rdestination = decode_rdestination;
  decode_instruction_struct[i].rsource1 = decode_rsource1;
  decode_instruction_struct[i].rsource2 = decode_rsource2;
  decode_instruction_struct[i].opcode = decode_opcode;
  decode_instruction_struct[i].funct3 = decode_funct3;
  decode_instruction_struct[i].funct7 = decode_funct7;
  decode_instruction_struct[i].shamt = decode_shamt;
  decode_instruction_struct[i].imm = decode_imm;
  decode_instruction_struct[i].pc = decodepc[i];
  decode_instruction_struct[i].instruction_type = decode_instruction_type;
  decode_instruction_struct[i].instruction_hex = decode_instruction[i];
  decode_instruction_struct[i].pcDestination = decode_pcDestination;
}

  int branch_taken = 0;
  for(int i = 0; i < NWAY; i++){
    if(branch_taken){
      decode_instruction_struct[i].instruction_type = 0;
    }
    if(decode_instruction_struct[i].instruction_type == 5){
      branch_taken = branch_predictor(decode_instruction_struct[i].pc, 1, decode_instruction_struct[i].imm);
      if(branch_taken){
        block_fetch_to_decode = 1;
      }
    }
  }

}
