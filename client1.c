#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <stdbool.h>
#include <signal.h>
#include <pthread.h>


/*
 * This program creates a socket and initiates a connection with the socket
 * given in the command line.  One message is sent over the connection and
 * then the socket is closed, ending the connection. The form of the command
 * line is streamwrite hostname portnumber
 */

pthread_t server_msg;


int main(int argc, char *argv[])
	{
	int sock;
	struct sockaddr_in server;
	struct hostent *hp;
	char buf[1024];

	/* Create socket */
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0) {
		perror("opening stream socket");
		exit(1);
	}
	/* Connect socket using name specified by command line. */
	server.sin_family = AF_INET;
	hp = gethostbyname(argv[1]);
	if (hp == 0) {
		fprintf(stderr, "%s: unknown host\n", argv[1]);
		exit(2);
	}
	bcopy(hp->h_addr_list[0], &server.sin_addr, hp->h_length);
	server.sin_port = htons(atoi(argv[2]));

	if (connect(sock,(struct sockaddr *) &server,sizeof(server)) < 0) {
		perror("connecting stream socket");
		exit(1);
	}
	int count=0;
	while(1) {
		count++;
		char buff[1024];
		int r=read(STDIN_FILENO, buff, sizeof(buff));
	    int w= write(sock, buff, r-1);
		if(w<0)
			 perror("writing on stream socket");
		int readback= read(sock, buff, sizeof(buff));
		write(STDOUT_FILENO, buff, readback);
		//buff[readback]='\0';
		if(strncmp(buff, "exit", 4)==0){
			exit(0);
		}
		
	}
	close(sock);
}



