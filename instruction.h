#include <stdlib.h>
#include <string.h>
#include "code.h"


//data structures 
typedef enum Reg{RAX=0,RCX=1,RDX=2,RBX=3,RSP=4,RBP=5,RSI=6,RDI=7,R8=8,R9=9,R10=10,R11=11,R12=12,R13=13,R14=14,R15=15} reg_t;
typedef enum Type {NONE=-1,REG=0, MEM=1,IMM =2} type_t;
typedef enum Size {N=-1,B=0,W=1,L=2,Q=3} osize_t;

typedef enum Bool{F=0,T=1} boolean;

typedef struct Operando{
	type_t t;
	osize_t s;
	reg_t reg_base;
	boolean hasDispl;
	boolean hasBase;
	boolean hasIndex;
	reg_t reg_index;
	unsigned char scale;
	unsigned int displ;
	unsigned long immediate;
	
}operando_t;


//data struct for instructions
typedef struct inst{
	unsigned char opcode;
	operando_t* source;
	operando_t* dest;
}inst_t;

operando_t * init_operando(); 
inst_t* init_inst();
inst_t* code_to_inst(code_t* c);

void print_operando(operando_t* o);
void print_inst(inst_t* i);
