#include <stdio.h>
#include <stdlib.h>
#include <time.h>


struct process{
    int pid;
    int ppid;
    time_t stime;
    time_t etime;
    struct process *next;
    struct process *prev;
};


int main(){

    printf("Maaz");

}
