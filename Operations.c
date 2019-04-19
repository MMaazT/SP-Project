#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void printList();
void addProcess(pid_t pid, pid_t ppid, char* name);
typedef struct process{
    pid_t pid;
    pid_t ppid;
    char * name;
    time_t stime;
    time_t etime;
    //bool isActive;
};
struct process Table[1000];
int tracker=0;

int main(int argc , char *argv[]){




char buff1[500];
int w1=write(STDOUT_FILENO, "Enter the command:\n", 20);
if (w1==-1){
    perror("Write1: ");
}
while(1){
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
    //write(STDOUT_FILENO, token, 3);
    token= strtok(NULL, " ");
    int sub;
    sub=atoi(token);
    token= strtok(NULL, " ");
    while(token!=NULL){
        sub= sub - atoi(token);
        token= strtok(NULL, " ");
    }
    //printf("%i\n", sub);
    char diffbuff[10];
    int s =sprintf(diffbuff, "%i\n", sub);
    if(s==-1) perror("sprintf:");
    write(STDOUT_FILENO, diffbuff, s);
}
else if(strcmp(token, "mul")==0){
    int mul=0;
    token= strtok(NULL, " "); //store the first number in token
    int tok2;
    mul=atoi(token); //assign it to mult
    tok2= atoi(strtok(NULL, " "));
    while(token!=NULL){
        mul= mul * (tok2); //the current number gets multiplied by the next number in the string
        tok2=atoi(token); // stores the next number of the string.
        token= strtok(NULL, " "); //store the current number
    }
    char mulbuff[50];
    int s =sprintf(mulbuff, "%i\n", mul);
    if(s==-1) perror("sprintf:");
    write(STDOUT_FILENO, mulbuff, s);
}
else if(strcmp(token, "div")==0){
    float div;
    token =strtok(NULL, " ");
    div=atoi(token);
    token =strtok(NULL, " ");
    float tok2;
    tok2=atoi(token); //assign it to div
    while(token!=NULL){
        div= div / (tok2); //the previous number gets multiplied by the next number in the string
        tok2=atoi(token); // stores the next number of the string.
        token= strtok(NULL, " "); //store the subsequent number in token
    }
    char divbuff[50];
    int s =sprintf(divbuff, "%0.2f\n", div);
    if(s==-1) perror("sprintf:");
    write(STDOUT_FILENO, divbuff, s);
}
else if(strcmp(token, "run")==0){
    
    int i=0;
    char *argu[200];
    while(token!=NULL){
        token=strtok(NULL, " ");
        argu[i]=token;
        i++;
    }
    argu[i]=NULL;
    printf("%s %d", argu[0], strlen(argu[0]));
    int pid=fork();
    if(pid==0){
        int e= execvp(argu[0],argu);
        if(e==-1) perror("exec");
        else {
         //   addProcess(getpid(), getppid(), argu[0]);
        }
    }
    if(pid>0){
        addProcess(pid, getpid(), argu[0]);
    }

}
else if(strcmp(token, "exit")==0){
    break;
    exit(0);
}
else if(strcmp(token, "printlist")==0){
    printList();
}
}

}
void addProcess(pid_t pid, pid_t ppid, char* name){
    struct process *p;
    p->pid=pid;
    p->ppid=ppid;
    strcpy(p->name,name);

    Table[tracker]= *p;
    tracker++;
}

void printList(){
    char buff[1000];
   
    int i;
    for(i=0; i<2; i++){
        char sb[10];
        sprintf(sb, "%d\t", Table[i].pid);
        strcpy(buff,sb) ;
        sprintf(sb, "%d\t", Table[i].ppid);
        strcat(buff,sb);
        sprintf(sb, "%s\t", Table[i].name);
        strcat(buff, sb);
        strcat(buff, "\0\n");
    }
    write(STDOUT_FILENO, buff, strlen(buff)+1);
}