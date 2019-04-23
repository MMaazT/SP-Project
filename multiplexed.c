#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#define TIMEOUT 5
#define BUF_LEN 1024

int main(void) {
    struct timeval tv;
    fd_set readfds;
    int ret;


    /*Waiting for input */
    FD_ZERO(&readfds);
    FD_SET(STDIN_FILENO, &readfds);

    
    /*Wait up to 5 seconds */
    tv.tv_sec=TIMEOUT;
    tv.tv_usec=0;

    /*All right now, block! */

    ret=SELECT(STDIN_FILENO +1, &readfds,NULL,NULL,&tv);

    if(ret==-1){
        perror("select: ");
        return 1;

    }
    else if (!ret){
        printf("%d seconds elapsed\n", TIMEOUT);
        return 0;
    }

    if(FD_ISSET(STDIN_FILENO, &readfds)){
        char buf [BUF_LEN+1];
        int len;

        /*guaranteed not to block */
        len=read(STDIN_FILENO, buf, BUF_LEN);
        if(len==-1){
            perror("read");
            return 1;
        }
        if(len){
            buf[len]='\0';
            printf("read: %s\n", buf);
        }
        return 0;
    }

}