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

void catchsig(int);

void do_child(){
    int i;
    static struct sigaction act;

    act.sa_handler = catchsig; // signo 수신하면 취할 행동 지정
    sigaction(SIGINT, &act, NULL); // SIGINT 수신하면 catchsig 함수 호출
    sigaction(SIGUSR1, &act, NULL); // SIGUSR1 수신하면 catchsig 함수 호출
    sigaction(SIGUSR2, &act, NULL); // SIGUSR2 수신하면 catchsig 함수 호출

    for(i=0; i<5; i++){
        printf("%d child is running...\n", i);
        sleep(1);
    }

    exit(0);
}

int main(void){
    int i, k, status;
    pid_t pid;

    pid = fork();
    if(pid == 0){
        do_child();
    }

    sleep(2);
    kill(pid, SIGINT);
    sleep(1);
    kill(pid, SIGUSR1);
    sleep(1);
    kill(pid, SIGUSR2);
    sleep(1);

    wait(&status);
    printf("exit status=%d\n", WEXITSTATUS(status));

    exit(0);
}

void catchsig(int signo){
    int i, pid;

    for(i=0;i<5;i++){
        printf("handling sig=%d ... \n", signo);
        sleep(1);
    }
}