#include <stdlib.h>
#include "useful.h"

//data struct 
typedef enum Reg{RAX=0,RCX=1,RDX=2,RBX=3,RSP=4,RBP=5,RSI=6,RDI=7,R8=8,R9=9,R10=10,R11=11,R12=12,R13=13,R14=14,R15=15} Reg;
typedef enum Type {NONE=-1,REG=0, MEM=1} Type;
typedef enum Size {N=0,B=1,W=2,L=4,Q=8} Size;

typedef struct Operando{
	Type t;
	Size s;
	Reg reg,
	int immediate;
}Operando;


//data struct for instructions
typedef struct inst{
	char opcode;
	Operando* source;
	Operando* dest;
	char DI; // if there is a displacement or an immediate;
	char BI; // if there is a base or an index reg;
	Reg index;
	char scale;
	short displ;
	int immediate;
}Inst;

Operando * initOperando();
Inst* initInst();
//TODO function that transform a code into an instruction
//TODO function that generate a path from a code
