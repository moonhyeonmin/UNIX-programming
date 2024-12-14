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
    int semid, n;
    key_t key;
    union semun arg;

    key=ftok("key", 1);
    semid=semget(key, 1, 0600|IPC_CREAT);
    arg.val=3;
    semctl(semid, 0, SETVAL, arg);

    n=semctl(semid, 0, GETVAL, arg);
    printf("semaphore value = %d\n", n);

    n=semctl(semid, 0, GETPID, arg);
    printf("semaphore creator pid = %d, my pid = %d\n", n, getpid());

    n=semctl(semid, 0, GETNCNT, arg);
    printf("semaphore number of waiting processes = %d\n", n);

    n=semctl(semid, 0, GETZCNT, arg);
    printf("semaphore number of waiting zero = %d\n", n);
    return 0;
}