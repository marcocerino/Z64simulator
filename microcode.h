#include "instruction.h"
#include <stdio.h>


char* generateMicrocode(Inst* i);//wrapper calls the function that handle the specific instruction

//function to generate the microcode to get the address of an operator
void getAddress(FILE* f,char* SoD,Boolean hasBase, Boolean hasIndex, Boolean hasDispl);//SoD must be either SOURCE or DEST

//HW instruction
char* hlt();
char* nope();
char* inte();

//data move instruction
char* mov(Operando* d,Operando*s);
//TODO: mov with sign extension
//TODO: lea

//push and pop instructions
char* push(Boolean flag,Operando* s);
char* pop(Boolean flag,Operando* s);

//TODO:movs and stos

//TODO: ALU instr

//shift inst
char* shift(Inst* i);

//Flags register instruction
char* setFlags(int bit, Boolean setZero);

//uncondition jump inst
char* jump(Boolean isAbsolute,Operando* o);
//call inst
char* call(Boolean isAbsolute, Operando* o);
char* ret();
//conditioned jump inst
char* condJump(unsigned char opcode);