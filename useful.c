#include <stdio.h>

//int to char * (int from 0 to 99)
char * itoa(int n){
	char * c = "0123456789";
	char r[2];
	if(i<10){
		strncpy(r,n,1);
		strncat(r,n+i,1);
	}
	else if(i<20){
		strncpy(r,n+1,1);
		strncat(r,n+(i-10),1);
	}

	else if(i<30){
		strncpy(r,n+2,1);
		strncat(r,n+(i-20),1);
	}
	else if(i<40){
		strncpy(r,n+3,1);
		strncat(r,n+(i-30),1);
	}
	else if(i<50){
		strncpy(r,n+4,1);
		strncat(r,n+(i-40),1);
	}
	else if(i<60){
		strncpy(r,n+5,1);
		strncat(r,n+(i-50),1);
	}
	else if(i<70){
		strncpy(r,n+6,1);
		strncat(r,n+(i-60),1);
	}
	else if(i<80){
		strncpy(r,n+7,1);
		strncat(r,n+(i-70),1);
	}
	else if(i<90){
		strncpy(r,n+9,1);
		strncat(r,n+(i-90),1);
	}
	else if(i<100){
		strncpy(r,n+9,1);
		strncat(r,n+(i-90),1);
	}
	return r;
}