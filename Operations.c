#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>

int main(int argc , char *argv[]){

char buff1[500];
int w1=write(STDOUT_FILENO, "Enter the command:\n", 20);
if (w1==-1){
    perror("Write1: ");
}
int r1=read(STDIN_FILENO,buff1, sizeof(buff1));
if (r1==-1){
    perror("Read1: ");
}
buff1[r1-1]='\0';
char *token= strtok(buff1, " ");
if(strcmp(token, "add")==0){
    int sum=0;
    token= strtok(NULL, " ");
    while(token!=NULL){
        sum+=atoi(token);
        token=strtok(NULL, " ");
    }
    char sumbuff[10];
    int s =sprintf(sumbuff, "%d\n", sum);
    if(s==-1) perror("sprintf:");
        write(STDOUT_FILENO, sumbuff, s);
}
else if(strcmp(token, "sub")==0){
    token= strtok(NULL, " ");
    int sub;
    sub=atoi(token);
    while(token!=NULL){
        token= strtok(NULL, " ");
        sub= sub - atoi(token);
    }
    //printf("%i\n", sub);
    char diffbuff[10];
    int s =sprintf(diffbuff, "%d\n", sub);
    if(s==-1) perror("sprintf:");
    write(STDOUT_FILENO, diffbuff, s);
}
else if(strcmp(token, "mul")==0){
    int mul=0;
    token= strtok(NULL, " "); //store the first number in token
    int tok2=1;
    mul=atoi(token); //assign it to mult
    printf("%d %d", mul, tok2);
    while(token!=NULL){
        token= strtok(NULL, " "); //store the current number
        mul= mul * (tok2); //the current number gets multiplied by the next number in the string
        tok2=atoi(token); // stores the next number of the string.
    }
        printf("%i\n", mul);
}
}

