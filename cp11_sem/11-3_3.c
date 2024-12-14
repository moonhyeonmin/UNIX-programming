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
    int semid, i;
    key_t key;
    union semun arg;
    ushort buf1[3]={1, 2, 3}, buf2[3];

    key=ftok("key", 7);
    semid=semget(key, 3, 0600|IPC_CREAT);

    arg.array=buf1;
    printf("SETALL ... %d\n", semctl(semid, 0, SETALL, arg));

    arg.array=buf2;
    printf("gETALL ... %d\n", semctl(semid, 0, GETALL, arg));
    for(i=0; i<3; i++){
        printf("%d\n", arg.array[i]);
    }
}