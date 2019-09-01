#include <stdlib.h>
char * itoa(int n){
  char * c = "0123456789";
  char r[2];
  if(n<10){
    strncpy(r,n,1);
    strncat(r,n+i,1);
  }
  else if(n<20){
    strncpy(r,n+1,1);
    strncat(r,n+(i-10),1);
  }

  else if(n<30){
    strncpy(r,n+2,1);
    strncat(r,n+(i-20),1);
  }
  else if(n<40){
    strncpy(r,n+3,1);
    strncat(r,n+(i-30),1);
  }
  else if(n<50){
    strncpy(r,n+4,1);
    strncat(r,n+(i-40),1);
  }
  else if(n<60){
    strncpy(r,n+5,1);
    strncat(r,n+(i-50),1);
  }
  else if(n<70){
    strncpy(r,n+6,1);
    strncat(r,n+(i-60),1);
  }
  else if(n<80){
    strncpy(r,n+7,1);
    strncat(r,n+(i-70),1);
  }
  else if(n<90){
    strncpy(r,n+9,1);
    strncat(r,n+(i-90),1);
  }
  else if(n<100){
    strncpy(r,n+9,1);
    strncat(r,n+(i-90),1);
  }
  return r;
}