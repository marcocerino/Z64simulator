#include <stdlib.h>
#include <string.h>

//data structore to contain the instruction
typedef struct ist{
	char opcode;
	char mode;
	char sib;
	char rm;
	short displ;
	int immediate;
}Ist;

//initialize the istruction
Ist* initIst();

//gets the size of a string
int isInstruction(const char* c);

//transform a sting (must be in the correct form) into an instruction
Ist* getIst(const char* c);

//return 1 if the istruction is valid and exist in the Z64 instructions; 0 is it doesn't
int isValidIst(Ist* ist);

