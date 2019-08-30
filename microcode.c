#include "microcode.h";

void generateMicrocode(Inst* i){
	unsigned char class = i->opcode >>= 4;
	unsigned char type = i->opcode & 0xF;

	if(class == 0){
		if(type == 0)
			hlt();
		else if(type == 1)
			nop();
		else if(type == 2)
			int();
	}
	else if(class == 1){
		if(type == 0)
			mov(i->dest,i->source);
		if(type == 1)
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
			movs();
		if(type == 8)
			stos();
	}

	//TODO: complete
}

//TODO: implement all the functions
void nope(){
	File * f = fopen("hlt.txt","w");
	fprintf(f,"MAR<-RIP\nMDR<-(MAR);RIP<-RIP+8\nIR<-MDR");
	fclose(f);
}