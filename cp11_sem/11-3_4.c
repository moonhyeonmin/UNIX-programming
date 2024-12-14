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
    struct sembuf p_buf;

    key=ftok("key", 1);
    semid=semget(key, 1, 0600|IPC_CREAT|IPC_EXCL);
    if(semid == -1){
        semget(key, 1, 0);
    }
    else{
        arg.val=1;
        semctl(semid, 0, SETVAL, arg); // 세마포어 값 1로 설정
    }

    p_buf.sem_num=0;
    p_buf.sem_op=-1;
    p_buf.sem_flg=0;
    semop(semid, &p_buf, 1); // 세마포어 값 1 감소

    printf("process %d in critical section\n", getpid());
    sleep(5);
    printf("process %d leaving critical section\n", getpid());

    p_buf.sem_op=1;
    semop(semid, &p_buf, 1); // 세마포어 값 1 증가

    return 0;
}