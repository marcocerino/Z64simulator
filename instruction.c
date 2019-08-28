#include "instruction.h"


Operando * initOperando(){
	Operando* o = (Operando*)malloc(sizeof(Operando));
	o->t = NONE;
	o->s = N;
}

Inst* initInst(){
	Inst* i = (Inst*)malloc(sizeof(Inst));
	i->name = NULL;
	i->source = initOperando();
	i->dest = initOperando();
	i->DI = -1;
	i->BI = -1;
	i->index =-1;
	i->scale = -1
	i->displ = -1;
	i->immediate = -1;
	return i;
}