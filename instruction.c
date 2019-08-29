#include "instruction.h"


Operando * initOperando(){
	Operando* o = (Operando*)malloc(sizeof(Operando));
	o->t = NONE;
	o->s = N;
	o->Reg = -1;
	o->immediate = -1;
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


Inst* codeToInst(Code* c){
	Inst* i = initInst();
	i->opcode = c->opcode;
	char SS,DS,DI,Mem;
	Mem = c->mode & 0x3;
	DI = (c->mode >>=2) & 0x3;
	DS = (c->mode >>=4) & 0x3;
	SS = (c->mode >>=6) & 0x3;

	//both operator are reg
	if(Mem == 0){

		char source = (c->rm >>=4);
		char dest = c->rm & 0x15;
		//create source reg
		i->source->t = REG;
		i->s = SS;
		i->reg = source;

		//create dest reg
		i->dest->t = REG;
		i->s = DS;
		i->reg = dest;
		return i;
	}
	//TODO complete

}