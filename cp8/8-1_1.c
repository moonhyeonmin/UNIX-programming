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
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/sem.h>

int main(void){
    int i, status;
    pid_t pid;

    pid = fork();
    if(pid == 0){
        for(i=0; i<5; i++){
            printf("%d child is running...\n", i);
            sleep(1);
        }
        exit(0);
    }
    else{
        sleep(2);
        kill(pid, SIGINT);
    }

    wait(&status);
    if(WIFEXITED(status)){
        printf("exit status=%d\n", WEXITSTATUS(status));
    }
    else if(WIFSIGNALED(status)){
        printf("signal number=%d\n", WTERMSIG(status));
    }

    exit(0);
}