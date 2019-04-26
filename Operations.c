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
    char * name; 
    char* stime;
    char* eltime;
    char * endtime;
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
        char diffbuff[10];
        int s =sprintf(diffbuff, "%i\n", sub);
        if(s==-1) perror("sprintf:");
        write(STDOUT_FILENO, diffbuff, s);
    }
    else if(strcmp(token, "mul")==0){
        int mul=1;
        //token= strtok(NULL, " "); //store the first number in token
        int tok2;
        mul=atoi(token); //assign it to mult
        tok2= atoi(strtok(NULL, " "));
        while(token!=NULL){
            mul= mul * (tok2); //the current number gets multiplied by the next number in the string
             token= strtok(NULL, " "); //store the current number
            tok2=atoi(token); // stores the next number of the string.
           
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
            argu[i]=token; //add a null if creates problem
            i++;
        }   
        argu[i]=NULL;
        int pipe_status, exec_status, pid, pipe_descriptors[2];
        pipe_status = pipe2(pipe_descriptors, __O_CLOEXEC);
        pid=fork();
        if(pid==0){
            int exec_status= execvp(argu[0],argu);
            if (exec_status == -1){     
                char error_buffer[4] = {0};
                int error = errno;
                sprintf(error_buffer, "%d", error);
                close(pipe_descriptors[0]);
                write(pipe_descriptors[1], error_buffer, sizeof(int));
                close(pipe_descriptors[1]);  
            }
        }
        if(pid>0){
            char buffer[4] = {0};
            close(pipe_descriptors[1]);
            int bytes_read = read(pipe_descriptors[0], buffer, sizeof(int));
            if (bytes_read == 0){
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
                addProcess(pid,argu[0], timebuff);
            }
            if (bytes_read > 0){
                char tempbuff[32];
                int error = atoi(buffer);
                sprintf(tempbuff,"EXECVP Failed because: %s\n\0",strerror(error));
                write(STDOUT_FILENO, tempbuff, strlen(tempbuff));
                break;
            }         
        }
    }
    else if(strcmp(token, "kill")==0){
        token= strtok(NULL, " ");
        int i;
        bool allTerminated=true;
        bool isPresent=false;
        for(i=0; i<tracker; i++){
            bool allTerminated=true;
            char strpid[6];
            sprintf(strpid, "%d", proc[i].pid);
            if (strcmp(strpid, token)==0){
                isPresent=true;
                if(!proc[i].isActive){
                    write(STDOUT_FILENO, "Process has already terminated!\n", 33);
                    break;
                }
                int k= kill(proc[i].pid, SIGTERM);
                if(k==-1) perror("Kill Error: ");
                else{
                    proc[i].isActive=false;
                    //timestamp
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
                    proc[i].endtime= strdup(timebuff);
                    break;
                    }
            }
            else if (strcmp(proc[i].name+'\0', token)==0){
                isPresent=true; 
                if(proc[i].isActive){
                    int k =kill(proc[i].pid, SIGTERM);
                    if(k==-1) perror("Kill Error: ");
                    else {
                        proc[i].isActive=false;
                        allTerminated=false; 
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
                        proc[i].endtime= strdup(timebuff);
                        break;
                    } 
                }/*
                else {
                    if(i!=tracker-1) continue;
                    else{
                        if(allTerminated){
                        write(STDOUT_FILENO, "All instances of this process have already terminated!\n", 55);
                        break;}   
                    }      
                    }*/   
            }
        }   if(!isPresent) write(STDOUT_FILENO,"No process present with the given name and pid!\n", 48);
    }
    else if(strcmp(token, "exit")==0){
        break;
        exit(0);
    }
    else if(strcmp(token, "printlist")==0){
        printList();
    }
    else if (strcmp(token, "help")==0){
        write(STDOUT_FILENO,"1. add: The 'add' command sums the string of numbers that follows it.\n", 71);
        write(STDOUT_FILENO,"2. sub: The 'sub' command subtracts each subsequent number from the previous one.\n", 83);
        write(STDOUT_FILENO,"3. mul: The 'mul' command multiplies all the numbers.\n", 55);
        write(STDOUT_FILENO,"4. div: The 'div' command divides the first number by the second number.\n", 74);
        write(STDOUT_FILENO,"5. run: The 'run' command starts a new process if exec is successful.\n", 71);
        write(STDOUT_FILENO,"6. exit: The 'exit' command terminates the current program.\n", 61);
        write(STDOUT_FILENO,"7. kill: The 'kill' command terminates a child process by either pid or name which follows it.\n", 96);
        write(STDOUT_FILENO,"8. printlist: The 'printlist' command prints the process list in its current state.\n", 85);
    }
}
}
void addProcess(pid_t pid, char* name, char* st){
    proc[tracker].pid = pid;
    proc[tracker].name = strdup(name);
    proc[tracker].stime= strdup(st);
    proc[tracker].isActive=true;
    proc[tracker].endtime= "-------------";
    tracker++;
}

void printList(){
    int seconds1, seconds2, h2, m2, s2,difft, dh, dm, ds;
    char  buff[100000];
    int w= write(STDOUT_FILENO, "No.\tProcessID\tProcessName\tStartTime\tTimeElapsed\tActive\tEndTime\n\n", 65);
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
        
        if(proc[i].isActive){
            proc[i].eltime=strdup(elap);
            no += sprintf(onebuff + no, "%s\t", elap);
        }
        else{
            proc[i].eltime= "------------";
            no += sprintf(onebuff + no, "%s\t", proc[i].eltime);
        }
        no += sprintf(onebuff + no, "%s\t", proc[i].isActive  ? "Yes" : "No");
        no += sprintf(onebuff + no, "%s\n", proc[i].endtime);
        buff[i]= strdup(onebuff);
    }
    for(i=0; i<tracker; i++){
        write(STDOUT_FILENO, buff[i], strlen(buff[i]));
    }
}