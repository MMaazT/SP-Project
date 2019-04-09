#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main(){
    char * buff[100];
    int r= read(STDIN_FILENO, buff, 100);
    buff[r-1]='\0';
    int i;
    int s=0;

    char aux[r];
    for(i=0; i<strlen(buff); i++){
        if(buff[i]!=';'){
            aux[i]=buff[i];
        }
        else{
            aux[i]='\0';
            int sum1= addTok(char* a);
        }
    }


}