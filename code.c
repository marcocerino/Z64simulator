#include "code.h"
char* zero = "0";
char* uno = "1";

Code* initCode(){
	Code* code = (Code*)malloc(sizeof(Code));
	Code->opcode = 0;
	Code->mode = 0;
	Code->sib = 0;
	Code->rm = 0;
	Code->displ = 0;
	Code->immediate = 0;
	return Code;
}

int isInstruction(const char* c){
	if(strlen(c) != 128)
		return 0;
	for(int i=0; i<128; i++){
		if(c[i] != zero[0] && c[i]!=uno[0])
			return 0;
	}
	return 1;
}

//transform a sting (must be in the correct form) into an instruction
Code* getCode(const char* c){
	Code* code = initCode();
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
	code->opcode = opc;
	code->mode = mode;
	code->sib = sib;
	code->rm = rm;
	code->displ = displ;
	code->immediate = imm;
	return code;
}

int isValidCode(Code* code){
	char oc = code->opcode;
	char type = oc & 0x15; //extracts the 4 less significant bits
	char mode = oc >>= 4; //extracts the 4 more significant bits

	char mem = code->mode & 0x3; //extrects the 2 less significant bits in the mode byte
	if(mem == 3){
		error_handler("L'istruzione genera un eccezione a runtime poichè entrambi gli operandi sono in memoria");
		return 0;
	}

	if(mode == 0 && (type = 0 || type > 3)){//HW class
		error_handler("L'istruzione non esiste nell'instruction set dello Z64");	
		return 0;
	}
	else if(mode == 1 && type > 9){
		error_handler("L'istruzione non esiste nell'instruction set dello Z64");	
		return 0;
	}
	else if(mode == 2 && type > 11){
		error_handler("L'istruzione non esiste nell'instruction set dello Z64");	
		return 0;
	}
	else if(mode == 3 && type > 13){
		error_handler("L'istruzione non esiste nell'instruction set dello Z64");	
		return 0;
	}
	else if(mode == 4 && type > 13){
		error_handler("L'istruzione non esiste nell'instruction set dello Z64");	
		return 0;
	}
	else if(mode == 5 && type > 5){
		error_handler("L'istruzione non esiste nell'instruction set dello Z64");	
		return 0;
	}
	else if(mode == 6 && type > 9){
		error_handler("L'istruzione non esiste nell'instruction set dello Z64");	
		return 0;
	}

	else{
		error_handler("L'istruzione non esiste nell'instruction set dello Z64");	
		return 0;
	}

	return 1;

	
}