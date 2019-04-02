#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>

int main(int argc , char *argv){

char buff1[500];
int w1=write(STDOUT_FILENO, "Enter the command:\n", 20);
int r1=read(STDIN_FILENO,buff1, sizeof(buff1));
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
    int diff;
    token= strtok(NULL, " ");
    diff=atoi(token);
    while(token!=NULL){
        diff-=atoi(token);
        token=strtok(NULL, " ");
    }
    char diffbuff[10];
    int s =sprintf(diffbuff, "%d\n", diff);
    if(s==-1) perror("sprintf:");
    write(STDOUT_FILENO, diffbuff, s);
}
}

