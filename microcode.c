#include "microcode.h"

char* generateMicrocode(Inst* i){

	unsigned char type = i->opcode & 0xF;
	unsigned char class = i->opcode >> 4;


	if(class == 0){
		if(type == 0)
			return hlt();
		else if(type == 1){
			return nope();
		}
		else if(type == 2);
			return inte();
	}
	
	else if(class == 1){
		if(type == 0)
			return mov(i->dest,i->source);
		//TODO: come fare esenzione del segno
		//TODO: lea
		else if(type == 4)
			return push(F,i->source);
		if(type == 5)
			return pop(F,i->source);
		else if(type == 6)
			return push(T,NULL);
		if(type == 7)
			return pop(T,NULL);
		//TODO: movs and stos
	}
	else if (class == 2){
		if(type == 0)
			return add(i->dest,i->source);
		else if(type == 1)
			return sub(i->dest,i->source);
		else if(type == 2)
			return adc(i->dest,i->source);
		else if(type == 3)
			return sbb(i->dest,i->source);
		else if(type == 4)
			return cmp(i->dest,i->source);
		else if(type == 5)
			return test(i->dest,i->source);
		//TODO:  neg
		else if(type == 7)
			return and(i->dest,i->source);
		else if(type == 8)
			return or(i->dest,i->source);
		else if(type == 9)
			return xor(i->dest,i->source);
		else if(type == 10)
			return not(i->dest);
		else if(type == 11)
			return bt(i->dest,i->source);
	}
	else if(class == 3){
		return shift(i);
	}
	else if (class == 4){
		if(type <=6)
			return setFlags(type,T);
		else
			return setFlags(type-7,F);
	}
	else if(class == 5){
		if(type == 0)
			return jump(F,NULL);
		else if (type == 1){
			return jump(T,i->source);
		}
		else if(type == 2)
			return call(F,NULL);
		else if(type == 3)
			return call(T,i->source);
		else if(type == 4 || type == 5)
			return ret();
	}
	else if(class == 6)
		return condJump(i->opcode);
	return "ops";
}



//TODO: implement all the functions
void getAddress(FILE*f, char* SoD,Boolean hasBase, Boolean hasIndex, Boolean hasDispl){
	if(hasDispl && ! hasBase && ! hasIndex)
		fprintf(f, "MAR<-TEMP1\n");
	else if(!hasDispl && hasBase && !hasIndex)
		fprintf(f, "MAR<-%s_REG",SoD);
	else if(hasIndex){
		fprintf(f,"TEMP2<-INDEX_REG\nMAR<-Shifter_Out[SHL,SCALE]\nTEMP1<-MAR\n");
		if(hasDispl)
			fprintf(f, "TEMP2<-IR[0:31]\nMAR<-ALU_Out[ADD]\nTEMP1<-MAR\n" );
		if(hasBase)
			fprintf(f, "TEMP2<-%s_REG\nMAR<-ALU_Out[ADD]\n",SoD);
	}
}


char* hlt(){
	FILE * f = fopen("hlt.txt","w");
	fprintf(f,"la CPU si mette in modalità risparimo energetico");
	fclose(f);
	error_handler("la CPU si mette in modalità risparimo energetico");
	return "hlt.txt";
}
char* nope(){
	printf("nope\n");
	FILE * f = fopen("nope.txt","w");
	fprintf(f,"MAR<-RIP\nMDR<-(MAR);RIP<-RIP+8\nIR<-MDR\n"); //fetch phase
	fclose(f);
	return "nope.txt";
}

char* inte(){
	FILE * f = fopen("int.txt","w");
	fprintf(f,"la CPU si mette in ascolto di un interrupt");
	fclose(f);
	error_handler("la CPU si mette in ascolto di un interrupt");
	return "int.txt";
}

char* mov(Operando* d, Operando*s){
	FILE* f = fopen("move.txt","w");

	 //fetch phase
	fprintf(f,"MAR<-RIP\nMDR<-(MAR);RIP<-RIP+8\nIR<-MDR\n");

	//the destination is a register
	if(d->t == REG){
		//source is an immediate
		if(s->t == IMM){ 
			//source is at max 2 bytes long
			if(s->s <2)
				fprintf(f, "DEST_REG<-IR[0:31]");
			//source is longer than 2 bytes
			else
				fprintf(f, "MAR<-RIP\nMDR<-(MAR);RIP<-RIP+8\nDEST_REG<-MDR");
		}

		//source is a register
		else if(s->t == REG){
			fprintf(f, "TEMP2<-SOURCE_REG\nDEST_REG<-TEMP2" );
		}

		//source is in memory
		else{
				getAddress(f,"SOURCE",s->hasBase,s->hasIndex,s->hasDispl);
				fprintf(f, "MDR<-(MAR)\nDEST_REG<-MDR");
			}
	}

	//the destination is in memory
	else if(d->t == MEM){
		
		getAddress(f,"DEST",d->hasBase,d->hasIndex,d->hasDispl);
			
		//source is an immediate
		if(s->t == IMM){ 
			//source is at max 2 bytes long
			if(s->s <2)
				fprintf(f, "(MAR)<-IR[0:31]");
			//source is longer than 2 bytes
			else
				fprintf(f, "MAR<-RIP\nMDR<-(MAR);RIP<-RIP+8\n(MAR)<-MDR");
		}

		//source is a register
		else if(s->t == REG){
			fprintf(f, "(MAR)<-SOURCE_REG" );
		}
	}
	fclose(f);
	return "move.txt";
}

char* push(Boolean flag, Operando* o){

	FILE * f = fopen("push.txt","w");
	fprintf(f,"MAR<-RIP\nMDR<-(MAR);RIP<-RIP+8\nIR<-MDR\n"); //fetch phase
	//MAR<-RSP
	fprintf(f, "MAR<-RSP\n");
	if(flag == T)
		fprintf(f, "MDR<-FLAGS\nMAR<-MDR;RSP<-RSP-8");
	else
		fprintf(f, "MDR<-SOURCE_REG\nMAR<-MDR;RSP<-RSP-8");
	fclose(f);
	return "push.txt";
}

char* pop(Boolean flag,Operando* o){
	FILE * f = fopen("pop.txt","w");
	fprintf(f,"MAR<-RIP\nMDR<-(MAR);RIP<-RIP+8\nIR<-MDR\n"); //fetch phase
	//MAR<-RSP
	fprintf(f, "MAR<-RSP\n");
	if(flag == T)
		fprintf(f, "MDR<-(MAR);RSP<-RSP-8\nFLAGS<-MDR\n");
	else
		fprintf(f, "MDR<-(MAR);RSP<-RSP-8\nDEST_REG<-MDR\n");
	fclose(f);
	return "pop.txt";
}


char * add(Operando* d, Operando* s){
	FILE * f = fopen("add.txt","w");
	fprintf(f,"MAR<-RIP\nMDR<-(MAR);RIP<-RIP+8\nIR<-MDR\n"); //fetch phase
	
	
	//source is an immediate
	if(s->t == IMM)
		fprintf(f,"TEMP2<-IR[0:31]\n");
	//source is a reg
	else if (s->t == REG)
		fprintf(f, "TEMP2<-SOURCE_REG\n");
	//source is in memory
	else{
		getAddress(f,"SOURCE",d->hasBase,d->hasIndex,d->hasDispl);
		fprintf(f, "TEMP2<-MDR\n");
	}
	//dest is a reg
	if(d->t == REG){
		fprintf(f, "TEMP1<-DEST_REG\n" );
		fprintf(f, "DEST_REG<-ALU_Out[ADD]\n" );
	}
	//dest is in memory
	else{
		getAddress(f,"DEST",d->hasBase,d->hasIndex,d->hasDispl);
		fprintf(f, "MDR<-(MAR)\nTEMP1<-MDR\n");
		fprintf(f,"MDR<-ALU_Out[ADD]\n(MAR)<-MDR\n");
	}
	fclose(f);
	return "add.txt";

}

char * sub(Operando* d, Operando* s){
	FILE * f = fopen("sub.txt","w");
	fprintf(f,"MAR<-RIP\nMDR<-(MAR);RIP<-RIP+8\nIR<-MDR\n"); //fetch phase
	
	
	//source is an immediate
	if(s->t == IMM)
		fprintf(f,"TEMP2<-IR[0:31]\n");
	//source is a reg
	else if (s->t == REG)
		fprintf(f, "TEMP2<-SOURCE_REG\n");
	//source is in memory
	else{
		getAddress(f,"SOURCE",d->hasBase,d->hasIndex,d->hasDispl);
		fprintf(f, "TEMP2<-MDR\n");
	}
	//dest is a reg
	if(d->t == REG){
		fprintf(f, "TEMP1<-DEST_REG\n" );
		fprintf(f, "DEST_REG<-ALU_Out[SUB]\n" );
	}
	//dest is in memory
	else{
		getAddress(f,"DEST",d->hasBase,d->hasIndex,d->hasDispl);
		fprintf(f, "MDR<-(MAR)\nTEMP1<-MDR\n");
		fprintf(f,"MDR<-ALU_Out[SUB]\n(MAR)<-MDR\n");
	}
	fclose(f);
	return "sub.txt";

}


char * adc(Operando* d, Operando* s){
	FILE * f = fopen("adc.txt","w");
	fprintf(f,"MAR<-RIP\nMDR<-(MAR);RIP<-RIP+8\nIR<-MDR\n"); //fetch phase
	
	//extract CF into TEMP1
	fprintf(f,"TEMP2<-FLAGS\nTEMP1<-Shifter_Out[0000,7]\n")
	
	//source is an immediate
	if(s->t == IMM)
		fprintf(f,"TEMP2<-IR[0:31]\n");
	//source is a reg
	else if (s->t == REG)
		fprintf(f, "TEMP2<-SOURCE_REG\n");

	//source is in memory
	else{
		getAddress(f,"SOURCE",d->hasBase,d->hasIndex,d->hasDispl);
		fprintf(f, "TEMP2<-MDR\n");
	}

	//we add source and CF
	fprintf(f, "TEMP2<-ALU_Out[ADD]\n" );
	

	//dest is a reg
	if(d->t == REG){
		fprintf(f, "TEMP1<-DEST_REG\n" );
		fprintf(f, "DEST_REG<-ALU_Out[ADD]\n" );
	}
	//dest is in memory
	else{
		getAddress(f,"DEST",d->hasBase,d->hasIndex,d->hasDispl);
		fprintf(f, "MDR<-(MAR)\nTEMP1<-MDR\n");
		fprintf(f,"MDR<-ALU_Out[ADD]\n(MAR)<-MDR\n");
	}
	fclose(f);
	return "adc.txt";

}
char * adc(Operando* d, Operando* s){
	FILE * f = fopen("sbb.txt","w");
	fprintf(f,"MAR<-RIP\nMDR<-(MAR);RIP<-RIP+8\nIR<-MDR\n"); //fetch phase
	
	//extract CF into TEMP1
	fprintf(f,"TEMP2<-FLAGS\nTEMP1<-Shifter_Out[0000,7]\n")
	
	//source is an immediate
	if(s->t == IMM)
		fprintf(f,"TEMP2<-IR[0:31]\n");
	//source is a reg
	else if (s->t == REG)
		fprintf(f, "TEMP2<-SOURCE_REG\n");

	//source is in memory
	else{
		getAddress(f,"SOURCE",d->hasBase,d->hasIndex,d->hasDispl);
		fprintf(f, "TEMP2<-MDR\n");
	}

	//we add source and CF
	fprintf(f, "TEMP2<-ALU_Out[ADD]\n" );
	

	//dest is a reg
	if(d->t == REG){
		fprintf(f, "TEMP1<-DEST_REG\n" );
		fprintf(f, "DEST_REG<-ALU_Out[SUB]\n" );
	}
	//dest is in memory
	else{
		getAddress(f,"DEST",d->hasBase,d->hasIndex,d->hasDispl);
		fprintf(f, "MDR<-(MAR)\nTEMP1<-MDR\n");
		fprintf(f,"MDR<-ALU_Out[SUB]\n(MAR)<-MDR\n");
	}
	fclose(f);
	return "sbb.txt";

}
char * cmp(Operando * d, Operando* s){
	FILE * f = fopen("cmp.txt","w");
	fprintf(f,"MAR<-RIP\nMDR<-(MAR);RIP<-RIP+8\nIR<-MDR\n"); //fetch phase
	
	
	//source is an immediate
	if(s->t == IMM)
		fprintf(f,"TEMP2<-IR[0:31]\n");
	//source is a reg
	else if (s->t == REG)
		fprintf(f, "TEMP2<-SOURCE_REG\n");
	//source is in memory
	else{
		getAddress(f,"SOURCE",d->hasBase,d->hasIndex,d->hasDispl);
		fprintf(f, "TEMP2<-MDR\n");
	}
	//dest is a reg
	if(d->t == REG){
		fprintf(f, "TEMP1<-DEST_REG\n" );
		fprintf(f, "ALU_Out[SUB]\n" );
	}
	//dest is in memory
	else{
		getAddress(f,"DEST",d->hasBase,d->hasIndex,d->hasDispl);
		fprintf(f, "MDR<-(MAR)\nTEMP1<-MDR\n");
		fprintf(f,"ALU_Out[SUB]\n");
	}
	fclose(f);
	return "cmp.txt";
}
char * test(Operando* d,Operando* s){
	FILE * f = fopen("test.txt","w");
	fprintf(f,"MAR<-RIP\nMDR<-(MAR);RIP<-RIP+8\nIR<-MDR\n"); //fetch phase
	
	
	//source is an immediate
	if(s->t == IMM)
		fprintf(f,"TEMP2<-IR[0:31]\n");
	//source is a reg
	else if (s->t == REG)
		fprintf(f, "TEMP2<-SOURCE_REG\n");
	//source is in memory
	else{
		getAddress(f,"SOURCE",d->hasBase,d->hasIndex,d->hasDispl);
		fprintf(f, "TEMP2<-MDR\n");
	}
	//dest is a reg
	if(d->t == REG){
		fprintf(f, "TEMP1<-DEST_REG\n" );
		fprintf(f, "ALU_Out[AND]\n" );
	}
	//dest is in memory
	else{
		getAddress(f,"DEST",d->hasBase,d->hasIndex,d->hasDispl);
		fprintf(f, "MDR<-(MAR)\nTEMP1<-MDR\n");
		fprintf(f,"ALU_Out[AND]\n");
	}
	fclose(f);
	return "test.txt";
}
char* neg(Operando* d){
	if(d->t == IMM){
		error_handler("L'istruzione neg non ammette immediati");
		return NULL;
	}
	FILE * f = fopen("neg.txt","w");
	fprintf(f,"MAR<-RIP\nMDR<-(MAR);RIP<-RIP+8\nIR<-MDR\n"); //fetch phase
	//TODO: xor with all 1 bits
	//TODO: add the result with 1
	fclose(f);
	return "neg.txt";
}

char * and(Operando* d,Operando* s){
	FILE * f = fopen("and.txt","w");
	fprintf(f,"MAR<-RIP\nMDR<-(MAR);RIP<-RIP+8\nIR<-MDR\n"); //fetch phase

	
	//source is an immediate
	if(s->t == IMM)
		fprintf(f,"TEMP2<-IR[0:31]\n");
	//source is a reg
	else if (s->t == REG)
		fprintf(f, "TEMP2<-SOURCE_REG\n");
	//source is in memory
	else{
		getAddress(f,"SOURCE",d->hasBase,d->hasIndex,d->hasDispl);
		fprintf(f, "TEMP2<-MDR\n");
	}
	//dest is a reg
	if(d->t == REG){
		fprintf(f, "TEMP1<-DEST_REG\n" );
		fprintf(f, "DEST_REG<-ALU_Out[AND]\n" );
	}
	//dest is in memory
	else{
		getAddress(f,"DEST",d->hasBase,d->hasIndex,d->hasDispl);
		fprintf(f, "MDR<-(MAR)\nTEMP1<-MDR\n");
		fprintf(f,"MDR<-ALU_Out[AND]\n(MAR)<-MDR\n");
	}
	fclose(f);
	return "and.txt";
}

char* or(Operando* d,Operando* s){
	FILE * f = fopen("or.txt","w");
	fprintf(f,"MAR<-RIP\nMDR<-(MAR);RIP<-RIP+8\nIR<-MDR\n"); //fetch phase
	
	
	//source is an immediate
	if(s->t == IMM)
		fprintf(f,"TEMP2<-IR[0:31]\n");
	//source is a reg
	else if (s->t == REG)
		fprintf(f, "TEMP2<-SOURCE_REG\n");
	//source is in memory
	else{
		getAddress(f,"SOURCE",d->hasBase,d->hasIndex,d->hasDispl);
		fprintf(f, "TEMP2<-MDR\n");
	}
	//dest is a reg
	if(d->t == REG){
		fprintf(f, "TEMP1<-DEST_REG\n" );
		fprintf(f, "DEST_REG<-ALU_Out[OR]\n" );
	}
	//dest is in memory
	else{
		getAddress(f,"DEST",d->hasBase,d->hasIndex,d->hasDispl);
		fprintf(f, "MDR<-(MAR)\nTEMP1<-MDR\n");
		fprintf(f,"MDR<-ALU_Out[OR]\n(MAR)<-MDR\n");
	}
	fclose(f);
	return "or.txt";
}

char* xor(Operando* d,Operando* s){
	FILE * f = fopen("xor.txt","w");
	fprintf(f,"MAR<-RIP\nMDR<-(MAR);RIP<-RIP+8\nIR<-MDR\n"); //fetch phase
	
	
	//source is an immediate
	if(s->t == IMM)
		fprintf(f,"TEMP2<-IR[0:31]\n");
	//source is a reg
	else if (s->t == REG)
		fprintf(f, "TEMP2<-SOURCE_REG\n");
	//source is in memory
	else{
		getAddress(f,"SOURCE",d->hasBase,d->hasIndex,d->hasDispl);
		fprintf(f, "TEMP2<-MDR\n");
	}
	//dest is a reg
	if(d->t == REG){
		fprintf(f, "TEMP1<-DEST_REG\n" );
		fprintf(f, "DEST_REG<-ALU_Out[XOR]\n" );
	}
	//dest is in memory
	else{
		getAddress(f,"DEST",d->hasBase,d->hasIndex,d->hasDispl);
		fprintf(f, "MDR<-(MAR)\nTEMP1<-MDR\n");
		fprintf(f,"MDR<-ALU_Out[XOR]\n(MAR)<-MDR\n");
	}
	fclose(f);
	return "xor.txt";
}

char * not(Operando* d){
	if(d->t == IMM){
		error_handler("L'istruzione not non ammette immediati");
		return NULL;
	}
	FILE * f = fopen("not.txt","w");
	fprintf(f,"MAR<-RIP\nMDR<-(MAR);RIP<-RIP+8\nIR<-MDR\n"); //fetch phase
	//TODO: xor with all 1 bits
	fclose(f);
	return "not.txt";
}

char * bt(Operando* d, Operando* s){
	if(s->t != IMM){
		error_handler("L'istruzione bt supporta solo un immediato come tipo di dato nella source");
		return NULL;
	}
	FILE * f = fopen("bt.txt","w");
	fprintf(f,"MAR<-RIP\nMDR<-(MAR);RIP<-RIP+8\nIR<-MDR\n"); //fetch phase
	
	//TODO: how to write the correct bit on Flags reg
	fclose(f);
	return "bt.txt"
}

char* shift(Inst* i){
	FILE * f = fopen("shift.txt","w");
	fprintf(f,"MAR<-RIP\nMDR<-(MAR);RIP<-RIP+8\nIR<-MDR\n"); //fetch phase

	//the dest value must be put in TEMP2
	if(i->dest->t == REG)
		fprintf(f,"TEMP2<-DEST_REG\n");
	else{
		getAddress(f,"DEST",i->dest->hasBase,i->dest->hasIndex,i->dest->hasDispl);
		fprintf(f,"MDR<-(MAR)\nTEMP2<-MDR\n");
	}

	
	//the instructions can have an immediate, overwise the number of time the source must be shifted is in RCX
	if(i->source->t == IMM){
		if(i->dest->t == REG)
			fprintf(f, "DEST_REG<-Shifter_Out[%d,immediate]",(i->opcode&0xF));
		else
			fprintf(f, "(MAR)<-Shifter_Out[%d,immediate]",(i->opcode&0xF) );
	}
	else{
		if(i->dest->t == REG)
			fprintf(f, "DEST_REG<-Shifter_Out[%d,RCX]",(i->opcode&0xF));
		else
			fprintf(f, "(MAR)<-Shifter_Out[%d,RCX]",(i->opcode&0xF) );
	
	}
	fclose(f);
	return "shift.txt";
}

char* setFlags(int bit,Boolean setZero){
	char * bits [5] ;
	bits[0]="CF";
	bits[1]="PF";
	bits[2]="ZF";
	bits[3]="SF";
	bits[4]="OF";
	FILE * f = fopen("flags.txt","w");
	fprintf(f,"MAR<-RIP\nMDR<-(MAR);RIP<-RIP+8\nIR<-MDR\n"); //fetch phase
	//TODO: how to write the correct bit on Flags reg
	if(setZero);
	else;
	fclose(f);
	return "flags.txt";
}


char* jump(Boolean isAbsolute,Operando* o){
	FILE * f = fopen("jump.txt","w");
	//fetch phase
	fprintf(f,"MAR<-RIP\nMDR<-(MAR);RIP<-RIP+8\nIR<-MDR\n");

	//relative jump
	if(isAbsolute == F)
		fprintf(f, "TEMP1<-RIP\nTEMP2<-IR[0:31]\nRIP<-ALU_Out[ADD]");
	else;
		//TODO:fprintf(f, "", );
	fclose(f);
	return "jump.txt";
}


char* call(Boolean isAbsolute, Operando* o){
	FILE * f = fopen("call.txt","w");
	//fetch phase
	fprintf(f,"MAR<-RIP\nMDR<-(MAR);RIP<-RIP+8\nIR<-MDR\n");

	//the call function has to copy the rip register in the stack
	fprintf(f,"MAR<-RSP\nMDR<-RIP;RSP<-RSP-8\nMAR<-MDR\n");

	//relative
	if(isAbsolute == F)
		fprintf(f, "TEMP1<-RIP\nTEMP2<-IR[0:31]\nRIP<-ALU_Out[ADD]");
	else;
	fclose(f);
	return "call.txt";
}
char * ret(){
	FILE * f = fopen("ret.txt","w");
	//fetch phase
	fprintf(f,"MAR<-RIP\nMDR<-(MAR);RIP<-RIP+8\nIR<-MDR\n");

	//have to put the return address from the stack into RIP
	fprintf(f, "MAR<-RSP\nMDR<-(MAR);RSP<-RSP+8\nRIP<-MDR" );
	fclose(f);
	return "ret.txt";

}
char* condJump(unsigned char opcode){
	Boolean set;
	int bit;
	char * bits [5] ;
	bits[0]="CF";
	bits[1]="PF";
	bits[2]="ZF";
	bits[3]="SF";
	bits[4]="OF";
	int type = opcode & 0xF;
	if(opcode>4){
		set = T;
		bit = opcode -5;
	}
	else {
		set = F;
		bit = opcode;
	}
	FILE * f = fopen("j.txt","w");

	//fetch phase
	fprintf(f,"MAR<-RIP\nMDR<-(MAR);RIP<-RIP+8\nIR<-MDR\n");

	//condition
	fprintf(f, "IF FLAG[%s] == %d THEN\n",bits[bit], set);

	//new address
	fprintf(f, "TEMP1<-RIP\nTEMP2<-IR[0:31]\nRIP<-ALU_Out[ADD]");

	fclose(f);
	return "j.txt";
}