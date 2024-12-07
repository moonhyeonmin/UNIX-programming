#include<stdio.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<fcntl.h>
#include<unistd.h>
#include<ftw.h>
#include<dirent.h>
#include<string.h>
#include<sys/wait.h>
#include<stdlib.h>
#include<sys/mman.h>
#include<signal.h>
#include<sys/msg.h>
#include<sys/time.h>

void catchint(int);

int main(void){
    int i, n;
    static struct sigaction act;

    act.sa_handler = catchint;
    sigaction(SIGINT, &act, NULL);

    n=pause();

    for(i = 0;i < 5; i++) {
        printf("%d...%d\n", i, n);
    }

    exit(0);
}

void catchint(int signo){
    printf("signal catch ... \n");
}