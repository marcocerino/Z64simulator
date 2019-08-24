#include <stdlib.h>
//data structore to contain the instruction
typedef struct ist{
	char opcode;
	char mode;
	char sib;
	char rm;
	short displ;
	int immediate;
}istruzione;

//gets the size of a string
int isInstruction(const char* c);

//transform a sting (must be in the correct form) into an instruction
void getIst(const char* c, istruzione* ist);

