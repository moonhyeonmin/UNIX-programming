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

void catchalarm(int);

int main(void){
    int i, n;
    static struct sigaction act;

    act.sa_handler = catchalarm;
    sigaction(SIGALRM, &act, NULL);

    n = alarm(10);
    printf("알람설정 = %d\n", n);

    for(i=0; i<10; i++){
        printf("%d child running...\n", i);
        if(i==1){
            n=alarm(3);
            printf("알람 재설정 = %d\n", n);
        }
        sleep(1);
    }
    exit(0);
}

void catchalarm(int signo){
    printf("catchalarm: %d\n", signo);
    alarm(3);
}