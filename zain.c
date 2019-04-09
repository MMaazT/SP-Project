#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>

int main(int argc , char *argv[]){

	char buff1[500];
	int w1=write(STDOUT_FILENO, "Enter the command:\n", 20);
	if (w1==-1)	perror("Write1: ");
	while(1){
		int r1=read(STDIN_FILENO,buff1, sizeof(buff1));
		if (r1==-1)	perror("Read1: ");
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
			int sub=atoi(token);
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
			int tok2=1;
			mul=atoi(token); //assign it to mult
			//printf("%d %d", mul, tok2);
			while(token!=NULL){
				token= strtok(NULL, " "); //store the current number
				mul= mul * (tok2); //the current number gets multiplied by the next number in the string
				tok2=atoi(token); // stores the next number of the string.
			}
				printf("%i\n", mul);
		}
		else if(strcmp(token, "div")==0){
			float div;
			token =strtok(NULL, " ");
			div=atoi(token);
			int tok2=1;
			div=atoi(token); //assign it to div
			while(token!=NULL){
				token= strtok(NULL, " "); //store the subsequent number in token
				div= div / (tok2); //the previous number gets multiplied by the next number in the string
				tok2=atoi(token); // stores the next number of the string.
			}
			printf("%f\n", div);
			char divbuff[50];
			int s =sprintf(divbuff, "%f\n", div);
			if(s==-1) perror("sprintf:");
			write(STDOUT_FILENO, divbuff, sizeof(divbuff));
		}
		else if(strcmp(token, "run")==0){
			int pid=fork();
			int i=0;
			char *argu[200];
			while(token!=NULL){
				token=strtok(NULL, " ");
				argu[i]=token;
				i++;
			}
			argu[i]=NULL;
			if(pid==0){
				int e= execvp(argu[0],argu);
				if(e==-1) perror("exec");
			}
		}
		else if(strcmp(token, "exit")==0){
			break;
			exit(0);
		}
	}

}