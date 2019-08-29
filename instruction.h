#include <stdlib.h>
#include "useful.h"
#include "code.h"

//data struct 
typedef enum Reg{RAX=0,RCX=1,RDX=2,RBX=3,RSP=4,RBP=5,RSI=6,RDI=7,R8=8,R9=9,R10=10,R11=11,R12=12,R13=13,R14=14,R15=15} Reg;
typedef enum Type {NONE=-1,REG=0, MEM=1,IMM =2} Type;
typedef enum Size {N=-1,B=0,W=1,L=2,Q=3} Size;


typedef struct Operando{
	Type t;
	Size s;
	Reg reg_base;
	Boolean hasDispl;
	Boolean hasBase;
	Boolean hasIndex;
	Reg reg_index;
	unsigned char scale;
	unsigned int displ;
	unsigned long immediate;
	
}Operando;


//data struct for instructions
typedef struct inst{
	unsigned char opcode;
	Operando* source;
	Operando* dest;
}Inst;

Operando * initOperando(); 
Inst* initInst();
Inst* codeToInst(Code* c);
//TODO function that generate a path from a code

void printOperando(Operando* o);
void printInst(Inst* i);
