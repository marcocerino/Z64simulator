#include <stdlib.h>
#include <string.h>
#include "code.h"
//reg names
/*
const char reg_names[][4][6] = {{{"AL"},{"AX"},{"EAX"},{"RAX"}},
						{{"CL"},{"CX"},{"ECX"},{"RCX"}},
						{{"DL"},{"DX"},{"EDX"},{"RDX"}},
						{{"BL"},{"BX"},{"EBX"},{"RBX"}},
						{{"SPL"},{"SP"},{"ESP"},{"RSP"}},
						{{"BPL"},{"BP"},{"EBP"},{"RBP"}},
						{{"SIL"},{"SI"},{"ESI"},{"RSI"}},
						{{"DIL"},{"DI"},{"EDI"},{"RDI"}},
						{{"R8B"},{"R8W"},{"E8D"},{"R8"}},
						{{"R9B"},{"R9W"},{"E9D"},{"R9"}},
						{{"R10B"},{"R10W"},{"E10D"},{"R10"}},
						{{"R11B"},{"R11W"},{"E11D"},{"R11"}},
						{{"R12B"},{"R12W"},{"E12D"},{"R12"}},
						{{"R13B"},{"R13W"},{"E13D"},{"R13"}},
						{{"R14B"},{"R14W"},{"E14D"},{"R14"}},
						{{"R15B"},{"R15W"},{"E15D"},{"R15"}},
						{{"FLAGS"},{"EFLAGS"},{"RFALGS"}},
						{{"IP"},{"EIP"},{"RIP"}}};
const char inst_names [][13][4]= {{{"hlt"},{"nop"},{"int"}},
						{{"mov"},{"movs"},{"movz"},{"lea"},{"push"},{"pop"},{"pushf"},{"popf"},{"movs"},{"stos"}},
						{{"add"},{"sub"},{"adc"},{"sbb"},{"cmp"},{"test"},{"neg"},{"and"},{"or"},{"not"},{"bt"}},
						{{"sal"},{"shl"},{"sar"},{"shr"},{"rcl"},{"rcr"},{"rol"},{"ror"}},
						{{"clc"},{"clp"},{"clz"},{"cls"},{"cli"},{"cld"},{"clo"},{"stc"},{"stp"},{"stz"},{"sts"},{"sti"},{"std"},{"sto"}},
						{{"jmp"},{"call"},{"ret"},{"iret"}},
						{{"jc"},{"jp"},{"jz"},{"js"},{"jo"},{"jnc"},{"jnp"},{"jnz"},{"jns"},{"jno"}}};
*/
//data struct 
typedef enum Reg{RAX=0,RCX=1,RDX=2,RBX=3,RSP=4,RBP=5,RSI=6,RDI=7,R8=8,R9=9,R10=10,R11=11,R12=12,R13=13,R14=14,R15=15} Reg;
typedef enum Type {NONE=-1,REG=0, MEM=1,IMM =2} Type;
typedef enum Size {N=-1,B=0,W=1,L=2,Q=3} Size;

typedef enum Bool{F=0,T=1} Boolean;

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
