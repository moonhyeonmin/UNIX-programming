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
    int i, j, num[10], sum = 0;
    pid_t pid;
    sigset_t mask;
    static struct sigaction act;

    act.sa_handler = catchint;
    sigaction(SIGINT, &act, NULL);

    sigemptyset(&mask);
    sigaddset(&mask, SIGINT);
    sigaddset(&mask, SIGUSR1);


    pid=fork();
    if(pid==0){
        for(i=0;i<5;i++){
        sigprocmask(SIG_SETMASK, &mask, NULL);
        scanf("%d", &num[i]);
        sigprocmask(SIG_UNBLOCK, &mask, NULL);
        sum+=num[i];
        for(j=0;j<=i;j++){
            printf("... %d\n", num[j]);
            sleep(1);
            }
        }

        exit(0);
    }
    else{
        sleep(2);
        kill(pid, SIGUSR1);
    }

    wait(0);
    exit(0);
}

void catchint(int signo){
    printf("Don't INTERRUPT ... \n");
}