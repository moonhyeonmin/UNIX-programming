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

// union semun { // 세마포어 초기화를 위한 구조체, semctl() 함수에서 사용
//     int val; // 세마포어 값
//     struct semid_ds *buf; // 세마포어 정보, 여러 세마포어를 동시에 초기화할 때 사용
//     unsigned short *array; // 세마포어 배열, IPC 스탯으로 정보를 가져올 때 사용
// };

int main(void){
    int semid, n, k;
    key_t key;
    union semun arg;
    struct sembuf p_buf={0, -1, 0}; // 세마포어 제어 구조체, semop() 함수에서 사용

    key=ftok("key", 7);
    semid=semget(key, 1, 0600|IPC_CREAT|IPC_EXCL); // 세마포어 생성
    if(semid == -1) // 세마포어 생성에 실패 (이미 있음)
        semid=semctl(key, 1, 0600); // 이미 존재하는 세마포어에 접속
    else{ // 세마포어 생성에 성공 (처음 생성)
        arg.val = 1; // 세마포어 값 1로 초기화
        semctl(semid, 0, SETVAL, arg); // 세마포어 초기화
    }

    printf("process %d : %d ---> ", getpid(), semctl(semid, 0, GETVAL, 0));
    semop(semid, &p_buf, 1);
    printf(" %d\n", semctl(semid, 0, GETVAL, 0));
    return 0;
}