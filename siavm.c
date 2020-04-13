
/* Michael Sollazzo | 404 | Assignment 3 | Virtual Machine */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

unsigned char memory[1000];
int registers[16];
int current_instruction[4]; //keeps track of bytes
int result_register;
int op1, op2;
int pc = 0;
int halt_flag = 1; //  1 = VM is running  0 = Halt VM
int opcode;
int temp; //helper varaible

void printMemory(){
  int counter = 0;

  printf("Memory:");
  for(int i = 0; i < 100; i++){
    if(counter%16 == 0){       //New line every 16 poistions in memory
      printf("\n");
  }
    counter++;
    printf("%02X", memory[i]); //print out hex value but only 2 characters, anyting more than 2 add 0's
    if(i%2!=0){
      printf(" ");
    }
  }
  printf("\n");
}

//Goes out to memory and gets bytes
void fetch() {
  opcode = memory[pc] >> 4;

   // 3R instructions LS instructions STACK instructions MOVE instructions
  if(opcode == 0 | opcode == 1 | opcode == 2 |
     opcode == 3 | opcode == 4 | opcode == 5 |
     opcode == 6 | opcode == 8 | opcode == 9 |
     opcode == 10 | opcode == 11 | opcode == 12){
      current_instruction[0] = memory[pc];
      current_instruction[1] = memory[++pc];
      pc++;
  } // BR1 and BR2 instructions
  if(opcode == 7){
     current_instruction[0] = memory[pc];
     current_instruction[1] = memory[++pc];
     current_instruction[2] = memory[++pc];
     current_instruction[3] = memory[++pc];
     pc++;
  }
}

  // Reads bytes and determines which function to use
void decode() {
    opcode = current_instruction[0] >> 4;
  //halt
  if(opcode == 0){
    op1 = 0;
    op2 = 0;
  }
  // 3R instructions
  if(opcode == 1 | opcode == 2 |opcode == 3 |
     opcode == 4 | opcode == 5 | opcode == 6){
       op1 = registers[(0x0F & current_instruction[0])];
       op2 = registers[current_instruction[1] >> 4];;
  }
  // BR1 and BR2 instructions
  if(opcode == 7){

  }
  // MOVE instructions
  if(opcode == 11){
    op1 = 0x0F & current_instruction[0];
    op2 = current_instruction[1];
  }
  // interrupt
  if(opcode == 12){
    temp = 0;
      if(current_instruction[1] == 0)
        while(temp<16)
      {
        printf("Register %d: %d\n", temp, registers[temp]);
        temp++;
      }
   }
}

// Computes op1 and op2
void execute() {
  opcode = current_instruction[0] >> 4;

  switch(opcode){
    case 0: //halt
      halt_flag = 0;
      break;
    case 1: //add
      result_register = op1 + op2;
      break;
    case 2: //and
      result_register = op1 & op2;
      break;
    case 3: //divide
      result_register = op1 / op2;
      break;
    case 4: //multiply
      result_register = op1 * op2;
      break;
    case 5: //subtract
      result_register = op1 - op2;
      break;
    case 6: //or
      result_register = op1 | op2;
      break;
    case 7: //Branches

      break;
    case 8: //load

      break;
    case 9: //store

      break;
    case 10: //pop

      break;
    case 11: //move
      result_register = op2;
      break;
  }
}

// Copys the result of op1 and op2 into a register
void store() {
 opcode = current_instruction[0] >> 4;

  switch(opcode){

    case 1: //add
    case 2: //and
    case 3: //divide
    case 4: //multiply
    case 5: //subtract
    case 6: //or
      temp = current_instruction[1] & 0x0F;
      registers[temp] = result_register;
      break;
    case 7: //Branches
      break;
    case 8: //load
      break;
    case 9: //store
      break;
    case 10: //pop
      break;
    case 11: //move
      temp = current_instruction[0] & 0x0F;
      registers[temp] = result_register;
      break;
  }
}

void load(char* file) {
  FILE *fp;
  fp = fopen(file, "r");
  fread(memory,1,1000,fp);
  fclose(fp);
}

int main(int argc, char**argv) {

  load(argv[1]);
  printMemory();

  //printf("current_instruction[0]%02X\n",current_instruction[0]);
  //printf("current_instruction[1]%02X\n",current_instruction[1]);
  //printf("op1: %02X\n",op1);
  //printf("op2: %02X\n",op2);

  //run event loop
  while(halt_flag) {
    fetch();
    decode();
    execute();
    store();
  }
}
