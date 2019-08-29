#include "instruction.h"


Operando * initOperando(){
	Operando* o = (Operando*)malloc(sizeof(Operando));
	o->t = NONE;
	o->s = N;
	o->reg_base = -1;
	o->hasDispl = F;
	o->hasBase = F;
	o->hasIndex = F;
	o->reg_index = -1;
	o->scale = 0;
	o->displ = 0;
	o->immediate = 0;

}

Inst* initInst(){
	Inst* i = (Inst*)malloc(sizeof(Inst));
	i->opcode = -1;
	i->source = initOperando();
	i->dest = initOperando();
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

	//both operator are reg || source might be an immediate
	if(Mem == 0){
		char dest = c->rm & 0xF;
		// dest reg
		i->dest->t = REG;
		i->dest->s = DS;
		i->dest->reg_base = dest;
		

		//source might be a reg or an immediate
		if(DI == 0){ //source is a reg
			char source = c->rm >>=4;
			i->source->t = REG;
			i->source->s = SS;
			i->source->reg_base = source;
		}
		else{
			//source is an immediate
			i->source->t = IMM;
			i->source->s = DS;
			if(DS < 2)
				i->source->immediate = c->displ;
			else
				i->source->immediate = c->immediate;

		}
	}

	// source in mem
	else if(Mem == 1){}

	//dest in mem
	else if(Mem == 2){}
	printInst(i);
	return i;
}




void printOperando(Operando* o){
	printf("type: %d\n",o->t);
	printf("size: %d\n",o->s);
	printf("reg_base: %d\n",o->reg_base);
	printf("hasDispl: %d\n",o->hasDispl);
	printf("hasIndex: %d\n",o->hasIndex);
	printf("hasBase: %d\n",o->hasBase);
	printf("scale: %u\n",o->scale);
	printf("displ: %u\n",o->displ );
	printf("immediate: %lu\n", o->immediate);
}

void printInst(Inst* i){
	printf("opcode: %u\n",i->opcode);
	printf("source:\n");
	printOperando(i->source);
	printf("dest:\n");
	printOperando(i->dest);

}
