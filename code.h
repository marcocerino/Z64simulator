#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "error_handler.h"

//data structore to contain the instruction
typedef struct code{
	unsigned char opcode;
	unsigned char mode;
	unsigned char sib;
	unsigned char rm;
	unsigned int displ;
	unsigned long immediate;
}Code;

//initialize the istruction
Code* initCode();

//gets the size of a string
int isInstruction(const char* c);

//transform a sting (must be in the correct form) into an instruction
Code* getCode(const char* c);

//return 1 if the istruction is valid and exist in the Z64 instructions; 0 is it doesn't
int isValidCode(Code* code);

void printCode(Code* code);

