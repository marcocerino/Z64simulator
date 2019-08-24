#include <stdlib.h>

typedef enum Memory {NONE=-1,REG=0, MEM=1} Memory;

//TODO: matrix of instructions names

typedef struct inst{
	char* name;
	Memory source;
	Memory dest;
	short displ;
	int immediate;

}Inst;

Inst* initInst();
//TODO function that transform a code into an instruction
