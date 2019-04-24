#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <stdbool.h>
#include <signal.h>

void printList();
void addProcess(pid_t pid, char* name, char* st);
typedef struct Process{
    pid_t pid;
    pid_t ppid;
    char * name; 
    char* stime;
    char* etime;
    bool isActive;
 } Processes;


Processes proc[1000];
int tracker=0;
int hours, minutes, seconds;
time_t now;

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
            argu[i]=token + '\0';;
            i++;
    }
        argu[i]=NULL;
        int pipe_descriptors[2];
        int pid=fork();
        if(pid==0){
            int e= execvp(argu[0],argu);
            if (e == -1)
        {
            perror("Exec");
                }
        }
        if(pid>0){
            char timebuff[30];	
            time(&now);
            struct tm *local = localtime(&now);
            hours = local->tm_hour;      	// get hours since midnight (0-23)
            minutes = local->tm_min;     	// get minutes passed after the hour (0-59)
            seconds = local->tm_sec;
            if (hours < 12)	// before midday
                sprintf(timebuff, "%02d:%02d:%02d am", hours, minutes, seconds);
            else	// after midday
		        sprintf(timebuff,"%02d:%02d:%02d pm", hours - 12, minutes, seconds);
            //write(STDOUT_FILENO, timebuff, sizeof(timebuff));
            addProcess(pid,argu[0], timebuff);         
        }
    }
    else if(strcmp(token, "kill")==0){
        bool terminated=false;
        token= strtok(NULL, " ");
        int i;
        for(i=0; i<=tracker; i++){
            char strpid[6];
            sprintf(strpid, "%d", proc[i].pid);
            //itoa(proc[i].pid, strpid,6);
            if (strcmp(strpid, token)==0){
                if(proc[i].isActive==false){
                    write(STDOUT_FILENO, "Process has already terminated!\n", 32);
                    break;
                }
                kill(proc[i].pid, SIGTERM);
                proc[i].isActive=false;
                break;
            }
            else if (strcmp(proc[i].name, token)==0){ //not working: try running after above loop
                if(proc[i].isActive==false){
                    write(STDOUT_FILENO, "Process has already terminated!", 32);
                    break;
                }
                kill(proc[i].pid, SIGTERM);
                proc[i].isActive=false;
                break;
        }
    
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
void addProcess(pid_t pid, char* name, char* st){
    proc[tracker].pid = pid;
    proc[tracker].name = strdup(name);
    proc[tracker].stime= strdup(st);
    proc[tracker].isActive=true;
    tracker++;
}

void printList(){
    int seconds1, seconds2, h2, m2, s2,difft, dh, dm, ds;
    char * buff[100000];
    int w= write(STDOUT_FILENO, "No.\tProcessID\tProcessName\tStartTime\tTimeElapsed\tActive\n\n", 56);
    int i;
    for(i=0; i<tracker; i++){
        int no=0;
        char onebuff[100];
        char elap[16];
        no = sprintf(onebuff, "%d\t", i+1);
        no += sprintf(onebuff + no, "%d\t\t", proc[i].pid);
        no += sprintf(onebuff + no, "%s\t\t", proc[i].name);
        no += sprintf(onebuff + no, "%s\t", proc[i].stime);
        
        //elapsedtime
        time(&now);
        struct tm *local = localtime(&now);
        hours = local->tm_hour;      	// get hours since midnight (0-23)
        if (hours>12) hours-=12;
        minutes = local->tm_min;     	// get minutes passed after the hour (0-59)
        seconds = local->tm_sec;
        seconds1= hours*60*60+ minutes*60 + seconds;
        sscanf(proc[i].stime,"%02d:%02d:%02d", &h2, &m2,&s2);
        seconds2= h2*60*60+ m2*60 + s2;
        difft=seconds1-seconds2;
        dm = difft/60;
        dh = dm/60;
        dm = dm%60;
        ds = difft%60;
        int s=sprintf(elap, "%02d:%02d:%02d", dh, dm ,ds);
        
        proc[i].etime=strdup(elap);
        no += sprintf(onebuff + no, "%s\t", elap);
        no += sprintf(onebuff + no, "%s\n", proc[i].isActive  ? "Yes" : "No");
        buff[i]= strdup(onebuff);
    }

    for(i=0; i<tracker; i++){
        write(STDOUT_FILENO, buff[i], strlen(buff[i]));
}
}