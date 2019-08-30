#include "microcode.h"

void generateMicrocode(Inst* i){

	unsigned char type = i->opcode & 0xF;
	unsigned char class = i->opcode >> 4;


	if(class == 0){
		if(type == 0)
			hlt();
		else if(type == 1){
			printf("hello\n");
			nope();
		}
		else if(type == 2);
			inte();
	}
	
	else if(class == 1){
		if(type == 0)
			mov(i->dest,i->source);
		/*if(type == 1)
			movs(i->dest,i->source);
		if(type == 2)
			movz(i->dest,i->source);
		if(type == 3)
			lea(i->dest,i->source);
		if(type == 4)
			push(i->source);
		if(type == 5)
			pop(i->source);
		if(type == 6)
			popf();
		if(type == 7)
			//movs();
		if(type == 8)
			stos();*/
	}
	else if(class == 6)
		condJump(i->opcode);
}

//TODO: implement all the functions
void getAddress(FILE*f, char* SoD,Boolean hasBase, Boolean hasIndex, Boolean hasDispl){
	/*if(hasBase){
				fprintf(f, "TEMP2<-%s_REG\nTEMP1<-Shifter_Out[SHL,SCALE]\n",SoD );
				if(hasIndex){
					fprintf(f,"TEMP2<-INDEX_REG\nMAR<-ALU_Out[ADD]\n");
					if(hasDispl)
						fprintf(f, "TEMP2<-IR[0:31]\nTEMP1<-MAR\nMAR<-ALU_Out[ADD]\n");
				}
				else if (hasDispl){
					fprintf(f, "TEMP2<-IR[0:31]\nMAR<-ALU_Out[ADD]\n");
				}
				else
					fprintf(f, "MAR<-TEMP1\n");
			}*/
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


void hlt(){
	printf("hlt\n");
	FILE * f = fopen("hlt.txt","w");
	fprintf(f,"la CPU si mette in modalità risparimo energetico");
	fclose(f);
	error_handler("la CPU si mette in modalità risparimo energetico");
}
void nope(){
	printf("nope\n");
	FILE * f = fopen("nope.txt","w");
	fprintf(f,"MAR<-RIP\nMDR<-(MAR);RIP<-RIP+8\nIR<-MDR"); //fetch phase
	fclose(f);
}

void inte(){
	printf("int\n");
	FILE * f = fopen("int.txt","w");
	fprintf(f,"la CPU si mette in ascolto di un interrupt");
	fclose(f);
	error_handler("la CPU si mette in ascolto di un interrupt");
}

void mov(Operando* d, Operando*s){
	printf("move\n");
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
		fprintf(f, "MDR<-DEST_REG\n(MAR)<-MDR");
			
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
	}
	fclose(f);
}

void condJump(unsigned char opcode){
	Boolean set;
	int bit;
	char ** bits ={{"CF"},{"PF"},{"ZF"},{"SF"},{"OF"}};
	char type = opcode & 0xF;
	if(opcode>4){
		set = F;
		bit = opcode -4;
	}
	else {
		set = T;
		bit = opcode;
	}
	FILE * f = fopen("j.txt","w");

	//fetch phase
	fprintf(f,"MAR<-RIP\nMDR<-(MAR);RIP<-RIP+8\nIR<-MDR");

	//condition
	fprintf(f, "IF FLAG[%s] == %d THEN\n",bits[bit], set);

	//new address
	fprintf(f, "TEMP1<-RIP\nTEMP2<-IR[0:31]\nRIP<-ALU_Out[ADD]");

	fclose(f);
}