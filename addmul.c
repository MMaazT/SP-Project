#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void Operations(char * buff, char* saveptr2);

int main(){
    char * buff[100];
    char *saveptr1, *saveptr2, *str1, *str2;
    int r= read(STDIN_FILENO, buff, 100);
    buff[r]='\0'; //change to r-1 if doesnt work
    int i=0;
    char * token1;
    char *delimArr[20];
    token1= strtok_r(buff, ";", &saveptr1);
    while(token1!=NULL){
        delimArr[i]=token1;
        //printf("%s", delimArr[i]);
        i++;
        token1=strtok_r(NULL,";", &saveptr1);
    }
    int j;
    for(j=0; j<i; j++){
        Operations(delimArr[j], saveptr2); //change to &saveptr if doesnt work
    }
    }

    void Operations(char * subtoken, char* saveptr2){
        //char* subtoken;
        subtoken=strtok_r(subtoken, " ", &saveptr2);
        if(strcmp(subtoken, "add")==0){
            int sum=0;
            subtoken= strtok_r(NULL, " ", &saveptr2);
            while(subtoken!=NULL){
                sum+=atoi(subtoken);
                subtoken=strtok_r(NULL, " ",&saveptr2);
        }
        char sumbuff[10];
        int s =sprintf(sumbuff, "%d\n", sum);
        if(s==-1) perror("sprintf:");
        write(STDOUT_FILENO, sumbuff, s);
}
        
    }
    


