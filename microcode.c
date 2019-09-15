#include "microcode.h"

void generate_microcode(inst_t* i,ret_value_t* t){

	unsigned char type = i->opcode & 0xF;
	unsigned char class = i->opcode >> 4;


	if(class == 0){
		if(type == 0)
			 hlt(t);
		else if(type == 1){
			 nope(t);
		}
		else if(type == 2);
			 inte(t);
	}
	
	else if(class == 1){
		if(type == 0)
			 mov(i->dest,i->source,t);
		//TODO: come fare esenzione del segno
		//TODO: lea
		else if(type == 4)
			 push(F,i->source,t);
		if(type == 5)
			 pop(F,i->source,t);
		else if(type == 6)
			 push(T,NULL,t);
		if(type == 7)
			 pop(T,NULL,t);
		//TODO: movs and stos
	}
	else if (class == 2){
		if(type == 0)
			//add
			 alu(i->dest,i->source,t,"ADD",0,1);
		else if(type == 1)
			//sub
			 alu(i->dest,i->source,t,"SUB",0,1);
		else if(type == 2)
			//adc
			 alu(i->dest,i->source,t,"ADD",1,1);
		else if(type == 3)
			//sbb
			 alu(i->dest,i->source,t,"SUB",1,1);
		else if(type == 4)
			//cmp
			 alu(i->dest,i->source,t,"SUB",0,0);
		else if(type == 5)
			//test
			 alu(i->dest,i->source,t,"AND",0,0);
		//TODO:  neg*/
		else if(type == 7)
			//and
			 alu(i->dest,i->source,t,"AND",0,1);
		else if(type == 8)
			//or
			 alu(i->dest,i->source,t,"OR",0,1);
		else if(type == 9)
			//xor
			 alu(i->dest,i->source,t,"XOR",0,1);
			/*
		else if(type == 10)
			 not(i->dest,t);
		else if(type == 11)
			 bt(i->dest,i->source,t);
			 */
	}
	else if(class == 3){
		 shift(i,t);
	}
	else if (class == 4){
		if(type <=6)
			 setFlags(type,T,t);
		else
			 setFlags(type-7,F,t);
	}
	else if(class == 5){
		if(type == 0)
			 jump(F,NULL,t);
		else if (type == 1){
			 jump(T,i->source,t);
		}
		else if(type == 2)
			 call(F,NULL,t);
		else if(type == 3)
			 call(T,i->source,t);
		else if(type == 4 || type == 5)
			 ret(t);
	}
	else if(class == 6)
		 condJump(i->opcode,t);
	
}

void fetch(ret_value_t* ret, FILE* f){
	//MAR<-RIP
	fprintf(f,"MAR_RIP\n");
	//MDR<-(MAR); RIP<-RIP+8
	fprintf(f,"MDR_MAR_RIP\nMDR_MAR_1\nMDR_MAR_2\n");
	//IR<-MDR
	fprintf(f,"IR_MDR\n"); 
	ret->num_pass +=5;
}

void getAddress(FILE* f,char* SoD,boolean hasBase, boolean hasIndex, boolean hasDispl,ret_value_t* ret){
	if(hasDispl && ! hasBase && ! hasIndex){
		//MAR<-IR[0:31]
		fprintf(f, "MAR_IR\n" );
		ret->num_pass++;
	}
	else if(!hasDispl && hasBase && !hasIndex){
		//MAR<-B
		fprintf(f, "MAR_%s_REG\n", SoD);
		ret->num_pass++;
	}
	else if(hasIndex){
		//TEMP1<-I
		fprintf(f,"TEMP1_INDEX\n");
		//MAR<-SHIFTER_OUT[SHL,T]
		fprintf(f, "MAR_Shifter_Out\n");
		//TEMP!<-MAR
		fprintf(f, "TEMP1_MAR\n");
		ret->num_pass+=3;
		if(hasDispl){
			//TEMP2<-IR[0:31]
			fprintf(f,"TEMP2_IR\n");
			//MAR<-ALU_OUT[ADD]
			fprintf(f,"MAR_ALU_Out_ADD\n");
			//TEMP1<-MAR
			fprintf(f, "TEMP1_MAR\n");
			ret->num_pass+=3;	
		}
		if(hasBase){
			//TEMP2<-B
			fprintf(f, "TEMP2_%s_REG\n", SoD);
			//MAR<-ALU_OUT{ADD}
			fprintf(f, "MAR_ALU_Out_ADD\n");
			ret->num_pass+=2;
		}

	}
}
//TODO: implement all the functions

void hlt(ret_value_t* ret){
	char* filename = "hlt.txt";
	ret->filename=filename;
	FILE * f = fopen(filename,"w");
	printf("hlt\n");
	fprintf(f,"la CPU si mette in modalità risparimo energetico");
	fclose(f);
	error_handler("la CPU si mette in modalità risparimo energetico");
	ret->num_pass = 0;
}
void nope(ret_value_t* ret){
	char* filename = "nope.txt";
	int ctr = 0;
	ret->filename=filename;
	printf("nope\n");
	FILE * f = fopen(filename,"w");
	//fetch phase
	fetch(ret,f);
	fclose(f);
	ret->num_pass = ctr;
}

void inte(ret_value_t* ret){
	char* filename = "int.txt";
	ret->filename=filename;
	FILE * f = fopen(filename,"w");
	printf("int\n");
	fprintf(f,"la CPU si mette in ascolto di un interrupt");
	fclose(f);
	error_handler("la CPU si mette in ascolto di un interrupt");
	ret->num_pass =0;
}

void mov(operando_t* d, operando_t*s,ret_value_t* ret){
	int ctr=0;	
	char* filename = "mov.txt";
	ret->filename=filename;
	FILE * f = fopen(filename,"w");
	printf("mov");
	//fetch phase
	fetch(ret,f);

	//destination is a register
	if(d->t == REG){
		//source is an immediate
		if(s->t == IMM){ 
			//source is at max 2 bytes long
			if(s->s <2){
				//DEST_REG<-IR
				fprintf(f, "DEST_REG_IR");
				ctr++;
			}
			//source is longer than 2 bytes
			else{
				//DEST_REG<-MDR
					//MAR<-RIP
					fprintf(f,"MAR_RIP\n");
					//MDR<-(MAR); RIP<-RIP+8
					fprintf(f,"MDR_MAR_RIP\nMDR_MAR_1\nMDR_MAR_2\n");
					//IR<-MDR
					fprintf(f,"DEST_REG_MDR\n"); 
				ctr += 5;
			}
		}

		//source is a register
		else if(s->t == REG){
			//TEMP2<-SOURCE_REG
			//DEST_REG<-TEMP2
			fprintf(f, "TEMP2_SOURCE_REG\nDEST_REG_TEMP2" );
			ctr+=2;
		}

		//source is in memory
		else{
			//GET SOURCE ADDRESS IN MAR
			getAddress(f,"SOURCE",s->hasBase,s->hasIndex,s->hasDispl,ret);
			//MDR<-(MAR)
			fprintf(f, "MDR_MAR_0\nMDR_MAR_1\nMDR_MAR_2\n");
			//DEST_REG<-MDR
			fprintf(f,"DEST_REG_MDR");
			ctr +=4;
		}
	}

	//the destination is in memory
	else if(d->t == MEM){
		//GET DEST ADDRESS IN MAR
		getAddress(f,"DEST",d->hasBase,d->hasIndex,d->hasDispl,ret);
			
		//source is an immediate
		if(s->t == IMM){ 
			//source is at max 2 bytes long
			if(s->s <2){
				//MDR<-IR[0:31]
				//(MAR)<-MDR
				fprintf(f, "MDR_IR\nMAR_MDR_0\nMAR_MDR_1\nMAR_MDR_2\n");
				ctr+=4;
			}
			//source is longer than 2 bytes
			else{
				//(MAR)<-MDR
					//MAR<-RIP
					fprintf(f,"MAR_RIP\n");
					//MDR<-(MAR); RIP<-RIP+8
					fprintf(f,"MDR_MAR_RIP\nMDR_MAR_1\nMDR_MAR_2\n");
					//(MAR)<-MDR
					fprintf(f,"MAR_MDR_0\nMAR_MDR_1\nMAR_MDR_2\n"); 
				ctr+= 7;
			}
		}

		//source is a register
		else if(s->t == REG){
			//MDR<-SOURCE_REG
			//(MAR)<-MDR
			fprintf(f, "MDR_SOURCE_REG\nMAR_MDR_0\nMAR_MDR_1\nMAR_MDR_2\n" );
			ctr +=4;
		}
	}
	fclose(f);
	ret->num_pass += ctr;
}

void push(boolean flag, operando_t* o,ret_value_t* ret){
	int ctr = 0;
	FILE * f = fopen("push.txt","w");
	ret->filename = "push.txt";
	//fetch phase
	fetch(ret,f);
	//TODO: stack head on MAR
	if(flag == T){
		//MDR<-FLAGS
		//(MAR)<-MDR
		fprintf(f, "MDR_FLAGS\nMAR_MDR_0\nMAR_MDR_1\nMAR_MDR_2");
		ctr +=4;
	}
	else{
		//MDR<-SOURCE_REG
		//(MAR)<-MDR
		fprintf(f, "MDR_SOURCE_REG\nMAR_MDR_0\nMAR_MDR_1\nMAR_MDR_2");
		ctr +=4;
	}
	fclose(f);
	ret->num_pass += ctr;
}


void pop(boolean flag, operando_t* o,ret_value_t* ret){
	int ctr = 0;
	FILE * f = fopen("pop.txt","w");
	ret->filename = "pop.txt";
	//fetch phase
	fetch(ret,f);
	//TODO: stack head on MAR
	if(flag == T){
		//MDR<-(MAR)
		//FLAGS<-MDR
		fprintf(f, "MDR_MAR_0\nMDR_MAR_1\nMDR_MAR_2\nFLAGS_MDR");
		ctr +=4;
	}
	else{
		//MDR<-(MAR)
		//DEST_REG<-MDR
		fprintf(f, "MDR_MAR_0\nMDR_MAR_1\nMDR_MAR_2\nDEST_REG_MDR");
		ctr +=4;
	}
	fclose(f);
	ret->num_pass += ctr;
}	


void  alu(operando_t* d, operando_t* s,ret_value_t* ret,char* op,int cf, int save ){
	int ctr = 0;
	FILE * f = fopen("alu.txt","w");
	ret->filename = "alu.txt";
	//fetch phase
	fetch(ret,f);
	//source is an immediate
	if(s->t == IMM){
		if(s->s <2){
			//TEMP2<-IR
			fprintf(f,"TEMP2_IR\n");
			ctr++;
		}
		else{
			//MAR<-RIP
			fprintf(f,"MAR_RIP\n");
			//MDR<-(MAR); RIP<-RIP+8
			fprintf(f,"MDR_MAR_RIP\nMDR_MAR_1\nMDR_MAR_2\n");
			//TEMP2IR<-MDR
			fprintf(f,"TEMP2_MDR\n"); 
			ctr +=5;
		}
	}
	//source is a reg
	else if (s->t == REG){
		//TEMP2<-SOURCE_REG
		fprintf(f, "TEMP2_SOURCE_REG\n");
		ctr++;
	}
	//source is in memory
	else{
		//address in MAR
		getAddress(f,"SOURCE",d->hasBase,d->hasIndex,d->hasDispl,ret);
		//MDR<-(MAR)
		fprintf(f, "MDR_MAR_0\nMDR_MAR_1\nMDR_MAR_2");
		//TEMP2<-MDR
		fprintf(f, "TEMP2_MDR\n");
		ctr +=4;
	}
	//dest is a reg
	if(d->t == REG){
		//TEMP1<-DEST_REG
		fprintf(f, "TEMP1_DEST_REG\n" );
		ctr ++;
		if(cf){
			//MDR<-ALU_Out[op]
			fprintf(f, "MDR_ALU_Out_%s\n",op);
			//TEMP2<-FLAGS
			fprintf(f, "TEMP2_FLAGS\n");
			//TEMP1<-MDR
			fprintf(f, "TEMP1_MDR\n" );
			ctr +=3;
		}
		if(save){
			//DEST_REG<-ALU_Out[op]
			fprintf(f, "DEST_REG_ALU_Out_%s\n",op);
			ctr ++;
		}
	}
	//dest is in memory
	else{
		//address in MAR
		getAddress(f,"DEST",d->hasBase,d->hasIndex,d->hasDispl,ret);
		//MDR<-(MAR)
		fprintf(f, "MDR_MAR_0\nMDR_MAR_1\nMDR_MAR_2");
		//TEMP1<-MDR
		fprintf(f, "TEMP1_MDR\n");
		ctr +=4;
		if(cf){
			//MDR<-ALU_Out[op]
			fprintf(f, "MDR_ALU_Out_%s\n",op);
			//TEMP2<-FLAGS
			fprintf(f, "TEMP2_FLAGS\n");
			//TEMP1<-MDR
			fprintf(f, "TEMP1_MDR\n" );
			ctr +=3;

		}
		if(save){
			//MDR<-ALU_Out[op]
			fprintf(f, "MDR_ALU_Out_%s\n",op);
			//(MAR)<-MDR
			fprintf(f, "MAR_MDR_0\nMAR_MDR_1\nMAR_MDR_2");
			ctr += 4;
		}	

	}
	fclose(f);
	ret->num_pass += ctr;

}

void shift(inst_t* i,ret_value_t* ret){
	int ctr;
	FILE * f = fopen("shift.txt","w");
	fetch(ret,f);

	//the dest value must be put in TEMP2
	if(i->dest->t == REG){
		//TEMP2<-DEST_REG;
		fprintf(f,"TEMP2_DEST_REG\n");
		ctr ++;
	}
	else{
		//GET DEST ADDRESS ON MAR;
		getAddress(f,"DEST",i->dest->hasBase,i->dest->hasIndex,i->dest->hasDispl,ret);
		//MDR<-(MAR)
		//TEMP2<-MDR
		fprintf(f,"MDR_MAR_0\nMDR_MAR_1\nMDR_MAR_2\nTEMP2_MDR\n");
		ctr += 4;
	}

	
	//the instructions is an immediate
	if(i->source->t == IMM){
		//dest is a reg
		if(i->dest->t == REG){
			//DEST_REG<-SHIFTER_OUT
			fprintf(f, "DEST_REG_Shifter_Out");
			ctr++;
		}
		//dest in memory
		else{
			//MDR<-SHIFTER_OUT
			//(MAR)<-MDR
			fprintf(f, "MDR_Shifter_Out\nMAR_MDR_0\nMAR_MDR_1\nMAR_MDR_2");
			ctr += 4;
		}
	}
	//the number of time the source must be shifted is in RCX
	else{
		//dest is a reg
		if(i->dest->t == REG){
			//DEST_REG<-SHIFTER_OUT
			fprintf(f, "DEST_REG_Shifter_Out");
			ctr++;
		}
		//dest in memory
		else{
			//MDR<-SHIFTER_OUT
			//(MAR)<-MDR
			fprintf(f, "MDR_Shifter_Out\nMAR_MDR_0\nMAR_MDR_1\nMAR_MDR_2");
			ctr += 4;
		}
	
	}
	fclose(f);
	ret->num_pass +=ctr;

}


void setFlags(int bit,boolean setZero,ret_value_t* ret){
	int ctr = 0;
	char * bits [5] ;
	bits[0]="CF";
	bits[1]="PF";
	bits[2]="ZF";
	bits[3]="SF";
	bits[4]="OF";
	FILE * f = fopen("flags.txt","w");
	ret->filename = "flags.txt";
	//fetch phase
		fetch(ret,f);
	//TODO: how to write the correct bit on Flags reg
	if(setZero);
	else;
	fclose(f);
	ret->num_pass += ctr;
}


void jump(boolean isAbsolute,operando_t* o,ret_value_t* ret){
	FILE * f = fopen("jump.txt","w");
	ret->filename = "jump.txt";
	int ctr=0;
	//fetch phase
	fetch(ret,f);
	//relative jump
	if(isAbsolute == F){
		//TEMP1<-RIP
		//TEMP2<-IR[0:31]
		//RIP<-ALU_OUT[ADD]
		fprintf(f, "TEMP1_RIP\nTEMP2_IR\nRIP_ALU_Out_ADD");
		ctr+=3;
	}
	else{
		//RIP<-IR[0:31]
		fprintf(f, "RIP_IR");
		ctr++;
	}
	fclose(f);
	ret->num_pass +=ctr;
}


void call(boolean isAbsolute, operando_t* o,ret_value_t* ret){
	FILE * f = fopen("call.txt","w");
	ret->filename = "call.txt";
	int ctr=0;
	//fetch phase
	fetch(ret,f);
	
	//the call function has to copy the rip register in the stack
	//TODO: cima dello stak su MAR
	//MDR<-RIP
	fprintf(f, "MDR_RIP\n" );
	//(MAR)<-MDR
	fprintf(f, "MAR_MDR_0\nMAR_MDR_1\nMAR_MDR_2");
	ctr+=4;
	//relative
	if(isAbsolute == F){
		//TEMP1<-RIP
		//TEMP2<-IR[0:31]
		//RIP<-ALU_OUT[ADD]
		fprintf(f, "TEMP1_RIP\nTEMP2_IR\nRIP_ALU_Out_ADD");
		ctr+=3;
	}
	else{
		//RIP<-IR[0:31]
		fprintf(f, "RIP_IR");
		ctr++;
	}
	fclose(f);
	ret->num_pass +=ctr;
}

void ret(ret_value_t* ret){
	FILE * f = fopen("ret.txt","w");
	ret->filename = "ret.txt";
	int ctr =0;
	//fetch phase
	fetch(ret,f);
	//TODO: cima dello stack su MAR
	//MDR<-(MAR)
	fprintf(f, "MDR_MAR_0\nMDR_MAR_1\nMDR_MAR_2");
	//RIP<-MDR
	fprintf(f, "RIP_MDR\n");
	ctr += 4;
	fclose(f);
	ret->num_pass+=ctr;

}


void condJump(unsigned char opcode, ret_value_t * ret){
	boolean set;
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
	ret->filename = "j.txt";
	int ctr = 0;

	//fetch phase
	fetch(ret,f);

	//condition
	fprintf(f, "IF FLAG[%s] == %d THEN\n",bits[bit], set);
	ctr ++;

	//new address
	//TEMP1<-RIP
	//TEMP2<-IR[0:31]
	//RIP<-ALU_OUT[ADD]
	fprintf(f, "TEMP1_RIP\nTEMP2_IR\nRIP_ALU_Out_ADD");
	ctr+=3;

	fclose(f);
	ret->num_pass += ctr;
}