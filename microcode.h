#include "instruction.h"
#include <stdio.h>

void generateMicrocode(Inst* i);//wrapper calls the function that handle the specific instruction

//function to generate the microcode to get the address of an operator
void getAddress(FILE* f,char* SoD,Boolean hasBase, Boolean hasIndex, Boolean hasDispl);//SoD must be either SOURCE or DEST

void hlt();
void nope();
void inte();

void mov(Operando* d,Operando*s);
void movs(Operando* d,Operando*s);
void movz (Operando* d,Operando*s);
void lea(Operando* d,Operando*s);
void push(Operando* s);
void pop(Operando* s);
void popf();
//void movs();
void stos();

void condJump(unsigned char opcode);