#include "instruction.h"
#include <stdio.h>


char* generate_microcode(inst_t* i);//wrapper calls the function that handle the specific instruction

//function to generate the microcode to get the address of an operator
void getAddress(FILE* f,char* SoD,boolean hasBase, boolean hasIndex, boolean hasDispl);//SoD must be either SOURCE or DEST

//HW instruction
char* hlt();
char* nope();
char* inte();

//data move instruction
char* mov(operando_t* d,operando_t*s);
//TODO: mov with sign extension
//TODO: lea
char* push(boolean flag,operando_t* s);
char* pop(boolean flag,operando_t* s);
//TODO:movs and stos

//ALU instr
char * add(operando_t* d, operando_t* s);
char * sub(operando_t* d, operando_t* s);
char * adc(operando_t* d, operando_t* s);
char * sbb(operando_t* d, operando_t* s);
char * cmp(operando_t * d, operando_t* s);
char * test(operando_t * d,operando_t* s);
char * neg(operando_t* d);
char * and(operando_t* d,operando_t* s);
char* or(operando_t* d,operando_t* s);
char* xor(operando_t* d,operando_t* s);
char * not(operando_t* d);
char * bt(operando_t* d, operando_t* s);

//shift inst
char* shift(inst_t* i);

//Flags register instruction
char* setFlags(int bit, boolean setZero);

//uncondition jump inst
char* jump(boolean isAbsolute,operando_t* o);
char* call(boolean isAbsolute, operando_t* o);
char* ret();

//conditioned jump inst
char* condJump(unsigned char opcode);