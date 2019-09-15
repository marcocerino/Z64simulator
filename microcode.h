#include "instruction.h"
#include <stdio.h>
typedef struct ret{
	char* filename;
	int num_pass;
}ret_value_t;

void generate_microcode(inst_t* i,ret_value_t* ret);//wrapper calls the function that handle the specific instruction

//function to generate the microcode to get the address of an operator
void getAddress(FILE* f,char* SoD,boolean hasBase, boolean hasIndex, boolean hasDispl,ret_value_t* ret);//SoD must be either SOURCE or DEST

void fetch(ret_value_t* ret,FILE* f);
//HW instruction
void hlt(ret_value_t* ret);
void nope(ret_value_t* ret);
void inte(ret_value_t* ret);

//data move instruction
void mov(operando_t* d,operando_t*s,ret_value_t* ret);
//TODO: mov with sign extension
//TODO: lea
void push(boolean flag,operando_t* s,ret_value_t* ret);
void pop(boolean flag,operando_t* s,ret_value_t* ret);
//TODO:movs and stos

//ALU instr
//op must be in [ADD,SUB,AND,OR,XOR]
//cf is a boolena if cf is involved
//save is a boolean if the result should be saved in dest
void  alu(operando_t* d, operando_t* s,ret_value_t* ret,char* op,int cf,int save);
void  not(operando_t* d,ret_value_t* ret);
void  bt(operando_t* d, operando_t* s,ret_value_t* ret);

//shift inst
void shift(inst_t* i,ret_value_t* ret);

//Flags register instruction
void setFlags(int bit, boolean setZero,ret_value_t* ret);

//uncondition jump inst
void jump(boolean isAbsolute,operando_t* o,ret_value_t* ret);
void call(boolean isAbsolute, operando_t* o,ret_value_t* ret);
void ret(ret_value_t* ret);

//conditioned jump inst
void condJump(unsigned char opcode,ret_value_t* ret);