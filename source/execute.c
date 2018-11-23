#include "run.h"

//execute I format instructions
void execute_iformat(){
  if(execute_opcode == 0b0010011){
    switch(current_funct3){
      case(0b000):
        printf("Instruction:Add Immediate\n");
        next_val = addi(registers[execute_rsource1]);
        break;
      case(0b111):
        printf("Instruction:And Immediate\n");
        next_val = andi(registers[execute_rsource1]);
        break;
      case(0b110):
        printf("Instruction:Or Immediate\n");
        next_val = ori(registers[execute_rsource1]);
        break;
      case(0b100):
        printf("Instruction:Xor Immediate\n");
        next_val = xori(registers[execute_rsource1]);
        break;
      case(0b010):
        printf("Instruction:STLI\n");
        next_val = slti(registers[execute_rsource1]);
        break;
      case(0b011):
        printf("Instruction:STLIU\n");
        next_val = sltiu(registers[execute_rsource1]);
        break;
      case(0b001):
        printf("Instruction:Logical Left Shift Immediate\n");
        next_val = slli(registers[execute_rsource1]);
        break;
      case(0b101):
        if(current_imm == 0b0000000){
          printf("Instruction:Logical Right Shift Immediate\n");
          next_val = srli(registers[execute_rsource1]);
          break;
        }
        else if(current_imm == 0b0100000){
          printf("Instruction:Arithmetic Right Shift Immediate\n");
          next_val = srai(registers[execute_rsource1]);
          break;
        }


    }
  }
  else if(execute_opcode == 0b0000011){
    switch(current_funct3){
      case(0b000):
        printf("Instruction:Load 8-bit\n");
        next_val = lb(Dcache, registers[execute_rsource1]);
        break;
      case(0b001):
        printf("Instruction:Load 16-bit\n");
        next_val = lh(Dcache, registers[execute_rsource1]);
        break;
      case(0b010):
        printf("Instruction:Load 32-bit\n");
        next_val = lw(Dcache, registers[execute_rsource1]);
        break;
      case(0b100):
        printf("Instruction:Load 8-bit Unsigned\n");
        next_val = lbu(Dcache, registers[execute_rsource1]);
        break;
      case(0b101):
        printf("Instruction:Load 16-bit Unsigned\n");
        next_val = lhu(Dcache, registers[execute_rsource1]);
        break;
    }
  }
  else if(execute_opcode == 0b1100111){
    printf("Instruction:Jump and Link Register\n");
    next_val = jalr(registers[execute_rsource1]);
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
      next_val = lui();
      break;
    case(0b0010111):
      printf("Instruction:Add Upper Immediate to PC\n");
      auipc();
      break;
  }
  return;
}

void execute_rformat(){
  switch(current_funct3){
    case(0b000):
      if(current_funct7 == 0b0000000){
        printf("Instruction:Add\n");
        next_val = add(registers[execute_rsource1], registers[execute_rsource2]);
      }
      else if(current_funct7 == 0b0100000){
        printf("Instruction:Subtract\n");
        next_val = sub(registers[execute_rsource1], registers[execute_rsource2]);
      }
      else if(current_funct7 == 0b0000001){
        printf("Instruction:Multiply\n");
        next_val = mul(registers[execute_rsource1], registers[execute_rsource2]);
      }
      break;
    case(0b001):
      printf("Instruction:Logical Left Shift\n");
      next_val = sll(registers[execute_rsource1], registers[execute_rsource2]);
      break;
    case(0b010):
      printf("Instruction:SLT\n");
      next_val = slt(registers[execute_rsource1], registers[execute_rsource2]);
      break;
    case(0b011):
      printf("Instruction:SLTU\n");
      next_val = sltu(registers[execute_rsource1], registers[execute_rsource2]);
      break;
    case(0b100):
      if(current_funct7 == 0b0000001){
        printf("Instruction:Divide\n");
        next_val = divide(registers[execute_rsource1], registers[execute_rsource2]);
        break;
      }
      else if(current_funct7 == 0b0000000){
        printf("Instruction:Xor\n");
        next_val = xor(registers[execute_rsource1], registers[execute_rsource2]);
        break;
      }
    case(0b101):
      if(current_funct7 == 0b0000000){
        printf("Instruction:Logical Right Shift\n");
        next_val = srl(registers[execute_rsource1], registers[execute_rsource2]);
      }
      else if(current_funct7 == 0b0100000){
        printf("Instruction:Logical Right Shift\n");
        next_val = sra(registers[execute_rsource1], registers[execute_rsource2]);
      }
      break;
    case(0b110):
      printf("Instruction:Or\n");
      next_val = or(registers[execute_rsource1], registers[execute_rsource2]);
      break;
    case(0b111):
      printf("Instruction:And\n");
      next_val = and(registers[execute_rsource1], registers[execute_rsource2]);
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
  switch(current_funct3){
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

void execute_sformat(){
  switch(current_funct3){
    case(0b000):
      printf("Instruction:Store 8-bit\n");
      sb(Dcache, registers[execute_rsource1], registers[execute_rsource2]);
      break;
    case(0b001):
      printf("Instruction:Store 16-bit\n");
      sh(Dcache, registers[execute_rsource1], registers[execute_rsource2]);
      break;
    case(0b010):
      printf("Instruction:Store 32-bit\n");
      sw(Dcache, registers[execute_rsource1], registers[execute_rsource2]);
      break;
  }
  return;
}

void execute_sj(){
  if(current_instruction_type == 4){
    execute_jformat();
  }
  else if(current_instruction_type == 6){
    execute_sformat();
  }
  return;
}

void execute(){

  if(first_decode < 2)return;
  if(first_execute == 0)first_execute++;
  if(last_instruction == 1){
    if(current_cycle > last_instruction_cycle + 2)return;
  }

  if(current_instruction_type == -1){
    printf("End of Program\n");
    exit(1);
  }
  instructions_executed++;

  printf("Instruction Type:%c\n", instruction_type_char);
  printf("Instruction Executed:0x%.8X\n", executed_instruction);
  if(current_instruction_type == 4 || current_instruction_type == 6){
    execute_sj();
    return;
  }

  if(current_instruction_type == 1){
    execute_iformat();
  }
  else if(current_instruction_type == 2){
    execute_uformat();
  }
  else if(current_instruction_type == 3){
    execute_rformat();
  }
  else if(current_instruction_type == 5){
    execute_bformat();
  }
  return;
}
