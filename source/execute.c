#include "run.h"

//execute I format instructions
void execute_iformat(){
  if(execute_opcode == 0b0010011){
    switch(execute_funct3){
      case(0b000):
        printf("Instruction:Add Immediate\n");
        execute_val = addi(registers[execute_rsource1]);
        break;
      case(0b111):
        printf("Instruction:And Immediate\n");
        execute_val = andi(registers[execute_rsource1]);
        break;
      case(0b110):
        printf("Instruction:Or Immediate\n");
        execute_val = ori(registers[execute_rsource1]);
        break;
      case(0b100):
        printf("Instruction:Xor Immediate\n");
        execute_val = xori(registers[execute_rsource1]);
        break;
      case(0b010):
        printf("Instruction:STLI\n");
        execute_val = slti(registers[execute_rsource1]);
        break;
      case(0b011):
        printf("Instruction:STLIU\n");
        execute_val = sltiu(registers[execute_rsource1]);
        break;
      case(0b001):
        printf("Instruction:Logical Left Shift Immediate\n");
        execute_val = slli(registers[execute_rsource1]);
        break;
      case(0b101):
        if(execute_imm == 0b0000000){
          printf("Instruction:Logical Right Shift Immediate\n");
          execute_val = srli(registers[execute_rsource1]);
          break;
        }
        else if(execute_imm == 0b0100000){
          printf("Instruction:Arithmetic Right Shift Immediate\n");
          execute_val = srai(registers[execute_rsource1]);
          break;
        }
    }
  }
  else if(execute_opcode == 0b0000011){
    printf("Instruction:Offset Calculated for Load\n");
    execute_offset = execute_imm + registers[execute_rsource1];
    // printf("execute_imm:%d\n", execute_imm);
    // printf("register_imm:%d\n", registers[execute_rsource1]);

    execute_access = 1;
    execute_load = 1;
  }
  else if(execute_opcode == 0b1100111){
    printf("Instruction:Jump and Link Register\n");
    execute_val = jalr(registers[execute_rsource1]);
  }
  else{
    printf("Error\n");
    exit(1);
  }
  return;
}

void execute_uformat(){
  switch(execute_opcode){
    case(0b0110111):
      printf("Instruction:Load Upper Immediate\n");
      execute_val = lui();
      break;
    case(0b0010111):
      printf("Instruction:Add Upper Immediate to PC\n");
      auipc();
      break;
  }
  return;
}

void execute_rformat(){
  switch(execute_funct3){
    case(0b000):
      if(execute_funct7 == 0b0000000){
        printf("Instruction:Add\n");
        execute_val = add(registers[execute_rsource1], registers[execute_rsource2]);
      }
      else if(execute_funct7 == 0b0100000){
        printf("Instruction:Subtract\n");
        execute_val = sub(registers[execute_rsource1], registers[execute_rsource2]);
      }
      else if(execute_funct7 == 0b0000001){
        printf("Instruction:Multiply\n");
        execute_val = mul(registers[execute_rsource1], registers[execute_rsource2]);
      }
      break;
    case(0b001):
      printf("Instruction:Logical Left Shift\n");
      execute_val = sll(registers[execute_rsource1], registers[execute_rsource2]);
      break;
    case(0b010):
      printf("Instruction:SLT\n");
      execute_val = slt(registers[execute_rsource1], registers[execute_rsource2]);
      break;
    case(0b011):
      printf("Instruction:SLTU\n");
      execute_val = sltu(registers[execute_rsource1], registers[execute_rsource2]);
      break;
    case(0b100):
      if(execute_funct7 == 0b0000001){
        printf("Instruction:Divide\n");
        execute_val = divide(registers[execute_rsource1], registers[execute_rsource2]);
        break;
      }
      else if(execute_funct7 == 0b0000000){
        printf("Instruction:Xor\n");
        execute_val = xor(registers[execute_rsource1], registers[execute_rsource2]);
        break;
      }
    case(0b101):
      if(execute_funct7 == 0b0000000){
        printf("Instruction:Logical Right Shift\n");
        execute_val = srl(registers[execute_rsource1], registers[execute_rsource2]);
      }
      else if(execute_funct7 == 0b0100000){
        printf("Instruction:Logical Right Shift\n");
        execute_val = sra(registers[execute_rsource1], registers[execute_rsource2]);
      }
      break;
    case(0b110):
      printf("Instruction:Or\n");
      execute_val = or(registers[execute_rsource1], registers[execute_rsource2]);
      break;
    case(0b111):
      printf("Instruction:And\n");
      execute_val = and(registers[execute_rsource1], registers[execute_rsource2]);
      break;
  }
  return;
}

void execute_jformat(){
  printf("Instruction:Jump and Link\n");
  jal();
  return;
}

void execute_bformat(){
  switch(execute_funct3){
    case(0b000):
      printf("Instruction:Branch if Equal\n");
      beq(registers[execute_rsource1], registers[execute_rsource2]);
      break;
    case(0b001):
      printf("Instruction:Branch if Not Equal\n");
      bne(registers[execute_rsource1], registers[execute_rsource2]);
      break;
    case(0b100):
      printf("Instruction:Branch if Less Than\n");
      blt(registers[execute_rsource1], registers[execute_rsource2]);
      break;
    case(0b101):
      printf("Instruction:Branch if Greater Than\n");
      bge(registers[execute_rsource1], registers[execute_rsource2]);
      break;
    case(0b110):
      printf("Instruction:Branch if Less Than Unsigned\n");
      bltu(registers[execute_rsource1], registers[execute_rsource2]);
      break;
    case(0b111):
      printf("Instruction:Branch if Greater Than Unsigned\n");
      bgeu(registers[execute_rsource1], registers[execute_rsource2]);
      break;
  }
  return;
}

void memory_iformat(){
  printf("load offset:%d\n",mem_offset);
    switch(mem_funct3){
      case(0b000):
        printf("Instruction:Load 8-bit\n");
        execute_val = lb(Dcache, mem_offset);
        break;
      case(0b001):
        printf("Instruction:Load 16-bit\n");
        execute_val = lh(Dcache, mem_offset);
        break;
      case(0b010):
        printf("Instruction:Load 32-bit\n");
        execute_val = lw(Dcache, mem_offset);
        break;
      case(0b100):
        printf("Instruction:Load 8-bit Unsigned\n");
        execute_val = lbu(Dcache, mem_offset);
        break;
      case(0b101):
        printf("Instruction:Load 16-bit Unsigned\n");
        execute_val = lhu(Dcache, mem_offset);
        break;
    }
  return;
}

void memory_sformat(){
  printf("s offset:%d\n",mem_offset);
  switch(mem_funct3){
    case(0b000):
      printf("Instruction:Store 8-bit\n");
      sb(Dcache, mem_offset, registers[mem_rsource2]);
      break;
    case(0b001):
      printf("Instruction:Store 16-bit\n");
      sh(Dcache, mem_offset, registers[mem_rsource2]);
      break;
    case(0b010):
      printf("Instruction:Store 32-bit\n");
      sw(Dcache, mem_offset, registers[mem_rsource2]);
      break;
  }
  return;
}

void execute_sformat(){
  printf("Instruction:Offset Calculated for Store\n");
  execute_offset = execute_imm + registers[execute_rsource1];
  execute_store = 1;
  execute_access = 1;
}

void execute_sj(){
  if(execute_instruction_type == 4){
    execute_jformat();
  }
  else if(execute_instruction_type == 6){
    execute_sformat();
  }
  return;
}

void execute(){

  if(first_decode < 2)return;
  if(last_instruction == 1){
    if(current_cycle > last_instruction_cycle + 2)return;
  }
  if(first_execute < 2)first_execute++;

  if(execute_instruction_type == -1){
    printf("End of Program\n");
    exit(1);
  }
  instructions_executed++;

  printf("Instruction Type:%c\n", instruction_type_char);
  printf("Instruction Executed:0x%.8X\n", executed_instruction);
  if(execute_instruction_type == 4 || execute_instruction_type == 6){
    execute_sj();
    return;
  }

  if(execute_instruction_type == 1){
    execute_iformat();
  }
  else if(execute_instruction_type == 2){
    execute_uformat();
  }
  else if(execute_instruction_type == 3){
    execute_rformat();
  }
  else if(execute_instruction_type == 5){
    execute_bformat();
  }
  return;
}
