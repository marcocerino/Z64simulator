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
char* push(Boolean flag,Operando* s);
char* pop(Boolean flag,Operando* s);
//TODO:movs and stos

//ALU instr
char * add(Operando* d, Operando* s);
char * sub(Operando* d, Operando* s);
char * adc(Operando* d, Operando* s);
char * sbb(Operando* d, Operando* s);
char * cmp(Operando * d, Operando* s);
char * test(Operando * d,Operando* s);
char * neg(Operando* d);
char * and(Operando* d,Operando* s);
char* or(Operando* d,Operando* s);
char* xor(Operando* d,Operando* s);
char * not(Operando* d);
char * bt(Operando* d, Operando* s);

//shift inst
char* shift(Inst* i);

//Flags register instruction
char* setFlags(int bit, Boolean setZero);

//uncondition jump inst
char* jump(Boolean isAbsolute,Operando* o);
char* call(Boolean isAbsolute, Operando* o);
char* ret();

//conditioned jump inst
char* condJump(unsigned char opcode);