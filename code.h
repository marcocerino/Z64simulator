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
}code_t;

//initialize the istruction
void init_code(code_t* code);

//gets the size of a string
int is_instruction(const char* c);

//transform a sting (must be in the correct form) into an instruction
size_t get_code(const char* c,code_t* code);

//return 1 if the istruction is valid and exist in the Z64 instructions; 0 is it doesn't
int is_valid_code(code_t* code);

void print_code(code_t* code);

