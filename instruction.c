#include "instruction.h"
char* zero = "0";
char* uno = "1";

int isInstruction(const char* c){
	int i = 0;
	while(c!= NULL){
		if(c[i] == zero[0] || c[i] == uno[0]){
			i++;
			c++;}
		else 
			return 0;
	}
	return 1;
}

//transform a sting (must be in the correct form) into an instruction
void getIst(const char* c, istruzione* ist){
	int i;
	char opc = 0;
	char mode = 0;
	char sib = 0;
	char rm = 0;
	short displ = 0;
	int imm = 0;
	int now;

	//opcode is a bit long
	for(i=0;i<8; i++){
		if(c[i]==zero[0])
			now = 0;
		else now = 1;
		opc += now *2^(7-i);
	}

	//mode camp is a bit long
	for(i=0;i<8; i++){
		if(c[i+8]==zero[0])
			now = 0;
		else now = 1;
		mode += now *2^(7-i);
	}

	//SIB is a bit long
	for(i=0;i<8; i++){
		if(c[i+16]==zero[0])
			now = 0;
		else now = 1;
		sib += now *2^(7-i);
	}

	// R/M is a bit long
	for(i=0;i<8; i++){
		if(c[i+24]==zero[0])
			now = 0;
		else now = 1;
		rm += now *2^(7-i);
	}

	//displache is 4 bit long
	for(i=0;i<32; i++){
		if(c[i+32]==zero[0])
			now = 0;
		else now = 1;
		displ += now *2^(31-i);
	}


	//immediate is 8 bit long
	for(i=0;i<64; i++){
		if(c[i+64]==zero[0])
			now = 0;
		else now = 1;
		imm += now *2^(63-i);
	}
	ist->opcode = opc;
	ist->mode = mode;
	ist->sib = sib;
	ist->rm = rm;
	ist->displ = displ;
	ist->immediate = imm;
	return;
}