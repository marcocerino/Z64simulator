#include "useful.h"


void stoi(char* string, int * n){
	int l = strlen(string);
	printf("l: %d\n",l);
	*n = 0;
	for(int i=0;i<l; i++){
		if(string[i]<'0' || string[i]>'9'){
			printf("errore, passare una stringa composta da cifre da 0 a 9\n");
			*n = -1;
			return;
		} 
		int c = string[i] -'0';
		printf("c: %d\n",c);
		*n += c * pow(10,l-i-1);
	}
}

void itos(int n, char*string){
	if(n<10){
		*string = n+'0';
	}
	else{
		char d,u;
		d = n/10;
		u = n -(d*10);
		d+='0';
		u+='0';
		*string = d;
		strncat(string,&u,1);
	}
}