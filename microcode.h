#include "instruction.h"
#include <stdio.h>

void generateMicrocode(Inst* i);//wrapper calls the function that handle the specific instruction

void hlt();
void nope();
void int();
void mov(Operando* d,Operando*s);
void movs(Operando* d,Operando*s);
void movz (Operando* d,Operando*s);
void lea(Operando* d,Operando*s);
void push(Operando* s);
void pop(Operando* s);
void popf();
void movs();
void stos();
//TODO : define all the functions