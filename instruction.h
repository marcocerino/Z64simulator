#include <stdlib.h>
#include "useful.h"

//data struct 
typedef enum Type {NONE=-1,REG=0, MEM=1} Type;
typedef enum Size {N=0,B=1,W=2,L=4,Q=8} Size;

typedef struct Operando{
	Type t;
	Size s;
}Operando;


//data struct for instructions
typedef struct inst{
	char opcode;
	Operando* source;
	Operando* dest;
	short displ;
	int immediate;

}Inst;

Operando * initOperando();
Inst* initInst();
//TODO function that transform a code into an instruction
//TODO function that generate a path from a code
