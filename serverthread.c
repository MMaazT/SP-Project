#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <stdbool.h>
#include <signal.h>
#include <errno.h>
#include <pthread.h>
#define TRUE 1

/*
 * This program creates a socket and then begins an infinite loop. Each time
 * through the loop it accepts a connection and prints out messages from it.
 * When the connection breaks, or a termination message comes through, the
 * program accepts a new connection.
 */
void printlist();
void * client_funcs();
void * conn_handle();
void addProcess(pid_t pid, char* name, char* st);
void addConn(char* ip, int port);

typedef struct Process{
    pid_t pid;
    char * name; 
    char* stime;
    char* eltime;
    char * endtime;
    bool isActive;
 } Processes;
 typedef struct Connection{
    char * ip_address;
    int port;
 } Connections;
 
Connections conn[1000];
Processes proc[1000];
int tracker=0;
int connTrack=0; //index for connections
int hours, minutes, seconds;
time_t now;
int msgsock;
pthread_t conn_handler, client_handler;
char buf[1024];
int rval;
int i;

int main(void)
{
	int sock, length;
	struct sockaddr_in server;
	/* Create socket */
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0) {
		perror("opening stream socket");
		exit(1);
	}
	/* Name socket using wildcards */
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = 0;
	if (bind(sock, (struct sockaddr *) &server, sizeof(server))) {
		perror("binding stream socket");
		exit(1);
	}
	/* Find out assigned port number and print it out */
	length = sizeof(server);
	if (getsockname(sock, (struct sockaddr *) &server, (socklen_t*) &length)) {
		perror("getting socket name");
		exit(1);
	}
    char portBuff[30];
	int pc =sprintf(portBuff, "Socket has port #%d\n", ntohs(server.sin_port));
	
    write(STDOUT_FILENO, portBuff, pc);

	/* Start accepting connections */
	listen(sock, 5);
    //int ret=pthread_create(&conn_handler, NULL, conn_handle, NULL);
    //if(ret==-1){
      //      perror("ThreadConnHandler: ");
       // }
	
    do {
        
        struct sockaddr_in clientaddr;
        struct socklen_t * clientaddr_size = sizeof(clientaddr);
        int ret=pthread_create(&conn_handler, NULL, conn_handle, NULL);
		msgsock = accept(sock, (struct sockaddr *) &clientaddr, &clientaddr_size);

           
        char * ip_client;
        ip_client= strdup(inet_ntoa(clientaddr.sin_addr));		
        int port_client= ntohs(clientaddr.sin_port);
        addConn(ip_client, port_client);
        write(STDOUT_FILENO, ip_client, 16 ); 
        
        if (msgsock == -1)
			perror("accept");
		else {
			//my code
			//fork will create a client handler which will then do all the computations.
			int f1=fork();
			if(f1<0) perror("Client didnt start: ");
			else if(f1==0){
                //client handler  
                int ret=pthread_create(&client_handler, NULL, client_funcs, NULL);
                if(!ret){
                    perror("thread");
                }
				
		    }
			else {
                
                           
			}
		}
} while (TRUE);
    
	//might have to close sock
}

void addProcess(pid_t pid, char* name, char* st){
    proc[tracker].pid = pid;
    proc[tracker].name = strdup(name);
    proc[tracker].stime= strdup(st);
    proc[tracker].isActive=true;
    proc[tracker].endtime= "-------------";
    tracker++;
}
void addConn(char* ip, int port){
    conn[connTrack].ip_address=strdup(ip);
    conn[connTrack].port=port;
    connTrack++;
}
void printList(){
    int seconds1, seconds2, h2, m2, s2,difft, dh, dm, ds;
    //char * buff[100000];
    int i;
	char onebuff[1024];
	int no=0;
	no= sprintf(onebuff, "%s", "No.\tProcessID\tProcessName\tStartTime\tTimeElapsed\tActive\tEndTime\n\n");
    for(i=0; i<tracker; i++){
        char elap[16];
        no += sprintf(onebuff+no, "%d\t", i+1);
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
        //buff[i]= strdup(onebuff);
    }
	write(msgsock, onebuff, no);
	
}

void listconnections(){
    char onebuff[1024];
	int no=0;
    int j;
	no= sprintf(onebuff, "%s", "No.\tIP\tPort\n");
    for(j=0; j<connTrack; j++){
        no += sprintf(onebuff+no, "%d\t", j+1);
        no += sprintf(onebuff + no, "%d\t", conn[connTrack].ip_address);
        no += sprintf(onebuff + no, "%d\t\n", conn[connTrack].port);
    }
    write(STDOUT_FILENO, onebuff, no);
}

void * client_funcs(){
    do {			
        bzero(buf, sizeof(buf));
        if ((rval = read(msgsock, buf, 1024)) < 0)
            perror("reading stream message");
        else if (rval == 0){
            //close(msgsock);
            int i;
            for(i=0; i<tracker; i++){
                write(buf, "Maaz", 4);
                if(proc[i].isActive)
                    kill(proc[i].pid, SIGTERM);
            }
            write(STDOUT_FILENO, "Ending connection\n", 19);
        }
        else {
            //Operations start here
            char *token= strtok(buf, " ");
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
                write(msgsock, sumbuff, s);
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
                write(msgsock, diffbuff, s);
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
                write(msgsock, mulbuff, s);
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
                write(msgsock, divbuff, s);
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
                    write(msgsock, "Exec Successful\n", 16);
                }
                if (bytes_read > 0){
                    char tempbuff[32];
                    int error = atoi(buffer);
                    sprintf(tempbuff,"EXECVP Failed because: %s\n\0",strerror(error));
                    write(msgsock, tempbuff, strlen(tempbuff));
                    break;
                }         
            }
        }
        else if(strcmp(token, "kill")==0){
            token= strtok(NULL, " ");
            int i;
            bool isPresent=false;
            for(i=0; i<tracker; i++){
                char strpid[6];
                sprintf(strpid, "%d", proc[i].pid);
                if (strcmp(strpid, token)==0){
                    isPresent=true;
                    if(!proc[i].isActive){
                        write(msgsock, "Process has already terminated!\n", 33);
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
                        write(msgsock, "The process has terminated successfully!\n", 41);
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
                            //allTerminated=false; 
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
                            write(msgsock, "The process has terminated successfully!\n", 42);
                            break;
                        } 
                    }
                    else {
                        write(msgsock, "Atleast one process by this name has already terminated.\n",58);
                        break;
                        }   
                }
            }   if(!isPresent) write(msgsock,"No process present with the given name and pid!\n", 49);
        }
       
        else if(strcmp(token, "printlist")==0){
            printList();
        }
        else if (strcmp(token, "help")==0){
            char buff[1000];
            int no = sprintf(buff, "%s", "1. add: The 'add' command sums the string of numbers that follows it.\n");
            no+=sprintf(buff+no, "%s", "2. sub: The 'sub' command subtracts each subsequent number from the previous one.\n");
            no+=sprintf(buff+no,"%s", "3. mul: The 'mul' command multiplies all the numbers.\n");
            no+=sprintf(buff+no, "%s","4. div: The 'div' command divides the first number by the second number.\n");
            no+=sprintf(buff+no,"%s","5. run: The 'run' command starts a new process if exec is successful.\n");
            no+=sprintf(buff+no,"%s","6. exit: The 'exit' command terminates the current program.\n");
            no+=sprintf(buff+no,"%s","7. kill: The 'kill' command terminates a child process by either pid or name which follows it.\n");
            no+=sprintf(buff+no,"%s","8. printlist: The 'printlist' command prints the process list in its current state.\n");
            write(msgsock, buff, no);
        }
        else{
            write(msgsock, "NO SUCH COMMAND FOUND!\n",24);
        }
    }
        
    }while (rval != 0);
    close(msgsock);
}

void * conn_handle(){
    while(1){
    //write(STDOUT_FILENO, "Hello\n", 6);
    bzero(buf, sizeof(buf));
    int r=read(STDIN_FILENO, buf, sizeof(buf));
    char *token= strtok(buf, " ");
        if(strncmp(token, "listconnections", 5)==0){
            //write(STDOUT_FILENO, "Hello\n", 7);
            listconnections();
        }
        else if(strcmp(token, "printbyip")==0){
            char ips[30];
            token= strtok(NULL, " ");
            int j;
            int s;
            for(j=0; j< connTrack; j++){
                if(strcmp(conn[connTrack].ip_address,token)==0){
                    s= sprintf(ips, "%s\t%d\n", token, conn[connTrack].port);
                }
            }
            write(STDOUT_FILENO, ips, s);
        }
        else if(strcmp(token, "add")==0){
            int sum=0;
            token= strtok(NULL, " ");
            while(token!=NULL){
                sum+=atoi(token);
                token=strtok(NULL, " ");
        }
            char sumbuff[10];
            int s =sprintf(sumbuff, "%d\n", sum);
            if(s==-1) perror("sprintf:");
            //printf( "23123\n");
            //write(STDOUT_FILENO, sumbuff, 3);
            write(STDOUT_FILENO, sumbuff, s);
        }
}
}
