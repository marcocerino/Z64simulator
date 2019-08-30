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
	unsigned char SS,DS,DI,Mem,Bp,Ip,Scale,Index;
	Mem = c->mode & 0x3;
	DI = (c->mode >>2) & 0x3;
	DS = (c->mode >>4) & 0x3;
	SS = (c->mode >>6) & 0x3;
	Bp = c->sib >>7;
	Ip = (c->sib >>6) & 0x1;
	Scale =(c->sib >>4) & 0x3;
	Index = c->sib & 0xF;

	printf("Mem: %u\n",Mem);
	printf("DI; %u\n",DI);
	printf("DS; %u\n",DS);
	printf("SS; %u\n",SS);
	printf("Bp; %u\n",Bp);
	printf("Ip; %u\n",Ip);
	printf("scale; %u\n",Scale);
	printf("Index; %u\n",Index);

	//both operator are reg || source might be an immediate
	if(Mem == 0){
		char dest = c->rm & 0xF;
		// dest reg
		i->dest->t = REG;
		i->dest->s = DS;
		i->dest->reg_base = dest;
		

		//source might be a reg or an immediate
		if(DI  == 0){ //source is a reg
			char source = c->rm >>4;
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

	// dest in mem
	else if(Mem == 1){
		i->dest->t = MEM;
		i->dest->s = DS;
		i->dest->hasDispl = DI >>1;
		i->dest->hasBase = Bp;
		i->dest->hasIndex = Ip;
		i->dest->scale = pow(2,Scale);
		if(i->dest->hasDispl)
			i->dest->displ = c->displ;
		if(i->dest->hasBase)
			i->dest->reg_base = c->rm & 0xF;
		if(i->dest->hasIndex)
			i->dest->reg_index = Index;


		//source might be a reg or an immediate
		if(DI & 0x1 == 0){ //source is a reg
			char source = c->rm >>4;
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

	//source in mem
	else if(Mem == 2){
		char dest = c->rm & 0xF;
		//dest reg
		i->dest->t = REG;
		i->dest->s = DS;
		i->dest->reg_base = dest;

		//TODO: create source
		i->source->t = MEM;
		i->source->s = DS;
		i->source->hasDispl = DI >>1;
		i->source->hasBase = Bp;
		i->source->hasIndex = Ip;
		i->source->scale = pow(2,Scale);
		if(i->source->hasDispl)
			i->source->displ = c->displ;
		if(i->source->hasBase)
			i->source->reg_base = c->rm >>4;
		if(i->source->hasIndex)
			i->source->reg_index = Index;

	}
	return i;
}




void printOperando(Operando* o){
	printf("*********************************\n");
	printf("type: %d\n",o->t);
	printf("size: %d\n",o->s);
	printf("reg_base: %d\n",o->reg_base);
	printf("hasDispl: %d\n",o->hasDispl);
	printf("hasIndex: %d\n",o->hasIndex);
	printf("hasBase: %d\n",o->hasBase);
	printf("reg_index: %d\n",o->reg_index);
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

