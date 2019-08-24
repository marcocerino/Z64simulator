#include "instruction.h"

Inst* initInst(){
	Inst* i = (Inst*)malloc(sizeof(Inst));
	i->name = NULL;
	i->source = NONE;
	i->dest = NONE;
	i->displ = 0;
	i->immediate = 0;
	return i;
}