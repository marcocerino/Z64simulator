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
	i->displ = 0;
	i->immediate = 0;
	return i;
}