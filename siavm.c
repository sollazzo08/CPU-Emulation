
/* Michael Sollazzo | 404 | Assignment 3 | Virtual Machine */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MEM_SIZE 1000;

unsigned char memory[SIZE];
int registers[16];
int PC = 0;
int op1, op2;
int result_register;
int halt_flag = 1; //  1 = VM is running  0 = Halt VM

//Goes out to memory and gets bytes
void fetch() {

}

// Reads bytes and determines which function to use
void decode() {

}

// Computes op1 and op2
void execute() {

}

// Copys the result of op1 and op2 into a register
void store() {

}

void load(char* file) {
  FILE *fp;
  fp = fopen(file, "r");
  fread(memory,1, MEM_SIZE,fp);
  fclose(fp);
}


int main(int argc, char**argv) {

  load(argv[1]);

  //run event loop
  while(halt_flag) {
    fetch();
    decode();
    execute();
    store();
  }
}
