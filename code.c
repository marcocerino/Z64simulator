#include "code.h"

void init_code(code_t* code){
	code->opcode = 0;
	code->mode = 0;
	code->sib = 0;
	code->rm = 0;
	code->displ = 0;
	code->immediate = 0;
}

int is_instruction(const char* c){
	int l = strlen(c);
	printf("strlen: %d\n",l);
	if(l != 128 && l!= 23 && l!= 47){
		return 0;
	}

	for(int i=0; i<l; i++){

		if(l==128 && (c[i] != '0' && c[i]!='1')){
			printf("errore: %u\n",c[i]);
			return 0;
		}
		
		else if((l==23 || l== 47) && (c[i]!= ' ' && !(c[i]>='0' && c[i]<='9') && !(c[i]>='A' && c[i]<='F'))){
			printf("errore: %u\n",c[i]);
			return 0;
		}
	}
	return 1;
}

//transform a sting (must be in the correct form) into an instruction
size_t get_code(const char* c,code_t* code){
		init_code(code);
	if(is_instruction(c) == 0){
		printf("not an inst\n");
		return -1;
	}
	init_code(code);
	int l = strlen(c);
	if(l == 128)
		return get_code_bits(c,code);
	else
		return get_code_mask(c,code);
}

//transform a sting (must be in the correct form) into an instruction
size_t get_code_bits(const char* c,code_t* code){
	
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
		if(c[i]=='0')
			now = 0;
		else now = 1;
		opc += now * pow(2,7-i);
	}
	//mode camp is a bit long
	for(i=0;i<8; i++){
		if(c[i+8]=='0')
			now = 0;
		else now = 1;
		mode += now *pow(2,7-i);
	}

	//SIB is a bit long
	for(i=0;i<8; i++){
		if(c[i+16]=='0')
			now = 0;
		else now = 1;
		sib += now *pow(2,7-i);
	}

	// R/M is a bit long
	for(i=0;i<8; i++){
		if(c[i+24]=='0')
			now = 0;
		else now = 1;
		rm += now *pow(2,7-i);
	}

	int	byte = 0;

	//displacemant is 4 bit long
	for(i=0; i<32;i++){
		if(c[i+32]=='0')
			now = 0;
		else now = 1;
		byte += now * pow(2,7-(i%8));
		if(i%8==0){
			displ += byte * pow(2,(i-32)%8);
			byte = 0;
		}
	}

	for(i=0; i<64;i++){
		if(c[i+64]=='0')
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
	if(imm = 0)
		return 8;
	return 16;
}


size_t get_code_mask(const char* c,code_t* code){
	int ret;
	int i;
	int l = strlen(c);
	if(l==23)
		ret = 8;
	else
		ret = 16;
	unsigned char bits[ret];
	unsigned char bit; 
	unsigned char aux;
	int ctr1,ctr2;
	ctr1 = ctr2 = 0;
	for(i=0; i<ret; i++)
		bits[i] = 0;
	for(i =0; i<l; i++){
		aux = c[i];
		if(aux == ' ')
			;
		else{
			if (aux>='0' && aux<='9')
				aux -= '0';
			else if(aux>='A' && aux<='F')
				aux -= 55;
			if(ctr1 == 0){
				ctr1=1;
				bit = aux<<4;

			}
			else if(ctr1 == 1){
				bit +=aux;
				bits[ctr2] = bit;
				bit = 0;
				ctr2++;
				ctr1 = 0;

			}
		}
	}
	code->opcode = bits[0];
	code->mode = bits[1];
	code->sib = bits[2];
	code->rm = bits[3];

	for(i=4;i<8;i++)
		code->displ += bits[i] * pow(2,(i-4)*8);
	
	if(ret == 16){
		for(i=8;i<16;i++)
		code->immediate += bits[i] * pow(2,(i-8)*8);
	}
	else code->immediate = 0;
	return ret;


}

int is_valid_code(code_t* code){
	print_code(code);
	unsigned char oc = code->opcode;
	unsigned char type = oc & 0xF; //extracts the 4 less significant bits
	unsigned char class = oc >> 4; //extracts the 4 more significant bits
	
	printf("isv: class: %u\n",class);

	printf("isv: type: %u\n",type);
	
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
	else if(class == 5 && type > 5){
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


void print_code(code_t* code){
	
	printf("*****************************\n");
	printf("opcode: %u\n",code->opcode);
	printf("mode: %u\n",code->mode);
	printf("sib: %u\n",code->sib);
	printf("rm: %u\n",code->rm);
	printf("displ: %u\n",code->displ);
	printf("imm: %lu\n",code->immediate);
}