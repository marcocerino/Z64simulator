#include "instruction.h"
#include <stdio.h>

char* generateMicrocode(Inst* i);//wrapper calls the function that handle the specific instruction

//function to generate the microcode to get the address of an operator
void getAddress(FILE* f,char* SoD,Boolean hasBase, Boolean hasIndex, Boolean hasDispl);//SoD must be either SOURCE or DEST

char* hlt();
char* nope();
char* inte();

char* mov(Operando* d,Operando*s);
/*char* movs(Operando* d,Operando*s);
char* movz (Operando* d,Operando*s);
char* lea(Operando* d,Operando*s);
*/
char* push(Boolean flag,Operando* s);
char* pop(Boolean flag,Operando* s);

/*char* movs();
char* stos();
*/



char* jump(Boolean isAbsolute,Operando* o);
char* call(Boolean isAbsolute, Operando* o);

char* condJump(unsigned char opcode);