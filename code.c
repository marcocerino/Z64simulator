#include "code.h"
char* zero = "0";
char* uno = "1";

Code* initCode(){
	Code* code = (Code*)malloc(sizeof(Code));
	code->opcode = 0;
	code->mode = 0;
	code->sib = 0;
	code->rm = 0;
	code->displ = 0;
	code->immediate = 0;
	return code;
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
	unsigned char opc = 0;
	unsigned char mode = 0;
	unsigned char sib = 0;
	unsigned char rm = 0;
	unsigned int displ = 0;
	unsigned long imm = 0;
	int now;

	//opcode is a bit long
	for(i=0;i<8; i++){
		if(c[i]==zero[0])
			now = 0;
		else now = 1;
		opc += now * pow(2,7-i);
	}
	//mode camp is a bit long
	for(i=0;i<8; i++){
		if(c[i+8]==zero[0])
			now = 0;
		else now = 1;
		mode += now *pow(2,7-i);
	}

	//SIB is a bit long
	for(i=0;i<8; i++){
		if(c[i+16]==zero[0])
			now = 0;
		else now = 1;
		sib += now *pow(2,7-i);
	}

	// R/M is a bit long
	for(i=0;i<8; i++){
		if(c[i+24]==zero[0])
			now = 0;
		else now = 1;
		rm += now *pow(2,7-i);
	}

	int	byte = 0;

	//displacemant is 4 bit long
	for(i=0; i<32;i++){
		if(c[i+32]==zero[0])
			now = 0;
		else now = 1;
		byte += now * pow(2,7-(i%8));
		if(i%8==0){
			displ += byte * pow(2,(i-32)%8);
			byte = 0;
		}
	}

	for(i=0; i<64;i++){
		if(c[i+64]==zero[0])
			now = 0;
		else now = 1;
		byte += now * pow(2,7-(i%8));
		if(i%8==0){
			displ += byte * pow(2,(i-64)%8);
			byte = 0;
		}
	}



	//immediate is 8 bit long
	

	code->opcode = opc;
	code->mode = mode;
	code->sib = sib;
	code->rm = rm;
	code->displ = displ;
	code->immediate = imm;
	return code;
}

int isValidCode(Code* code){
	printCode(code);
	unsigned char oc = code->opcode;
	unsigned char type = oc & 0xF; //extracts the 4 less significant bits
	unsigned char class = oc >>= 4; //extracts the 4 more significant bits
	
	
	unsigned char mem = code->mode & 0x3; //extrects the 2 less significant bits in the class byte

	if(mem == 3){
		error_handler("L'istruzione genera un eccezione a runtime poichè entrambi gli operandi sono in memoria");
		return 0;
	}

	if(class == 0 && (type = 0 || type > 3)){//HW class
		error_handler("L'istruzione non esiste nell'instruction set dello Z64");	
		return 0;
	}
	else if(class == 1 && type > 9){
		error_handler("L'istruzione non esiste nell'instruction set dello Z64");	
		return 0;
	}
	else if(class == 2 && type > 11){
		error_handler("L'istruzione non esiste nell'instruction set dello Z64");	
		return 0;
	}
	else if(class == 3 && type > 7){
		error_handler("L'istruzione non esiste nell'instruction set dello Z64");	
		return 0;
	}
	else if(class == 4 && type > 13){
		error_handler("L'istruzione non esiste nell'instruction set dello Z64");	
		return 0;
	}
	else if(class == 5 && type > 4){
		error_handler("L'istruzione non esiste nell'instruction set dello Z64");	
		return 0;
	}
	else if(class == 6 && type > 9){
		error_handler("L'istruzione non esiste nell'instruction set dello Z64");	
		return 0;
	}

	else if(class>6){
		error_handler("L'istruzione non esiste nell'instruction set dello Z64");	
		return 0;
	}
	return 1;

	
}


void printCode(Code* code){
	printf("opcode: %u\n",code->opcode);
	printf("mode: %u\n",code->mode);
	printf("sib: %u\n",code->sib);
	printf("rm: %u\n",code->rm);
	printf("displ: %u\n",code->displ);
	printf("imm: %lu\n",code->immediate);
}