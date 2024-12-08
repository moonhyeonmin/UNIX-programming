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

struct databuf{
    int d_nread;
    char d_buf[512];
};

int main(void){
    char *buf1;
    int i, n, shmid1, shmid2, shmid3, *buf2;
    key_t key1, key2, key3;
    struct databuf *buf3;

    key1 = ftok("key1", 1);
    key2 = ftok("key2", 2);
    key3 = ftok("key3", 3);
    if (key3 == -1){
        perror("ftok key3 Failed");
        exit(1);
    }

    shmid1 = shmget(key1, 512, IPC_CREAT | 0600); // key1을 이용하여 512바이트 크기의 공유 메모리 생성
    shmid2 = shmget(key2, 10 * sizeof(int), IPC_CREAT | 0600); // key2를 이용하여 10개의 int형 데이터를 저장할 수 있는 공유 메모리 생성
    shmid3 = shmget(key3, 5 * sizeof(struct databuf), IPC_CREAT | 0600); // key3를 이용하여 5개의 struct databuf형 데이터를 저장할 수 있는 공유 메모리 생성

    buf1 = (char *)shmat(shmid1, 0, 0); // buf에 shmid1을 연결
    buf2 = (int *)shmat(shmid2, 0, 0); // buf2에 shmid2를 연결
    buf3 = (struct databuf *)shmat(shmid3, 0, 0); // buf3에 shmid3를 연결

    n=read(0, buf1, 512); // 표준 입력에서 최대 512바이트를 읽어 buf1에 저장
    write(1, buf1, 512); // buf1에 저장된 내용을 표준 출력으로 출력

    for(i=0;i<10;i++){
        scanf("%d", buf2+i); // 10개의 int형 데이터를 입력받아 buf2에 저장
    }

    for(i=0;i<10;i++)
        printf("%d\n", *(buf2 + i)); // buf2에 저장된 내용을 출력

    for(i=0; i<5; i++){
        n = read(0, (buf3+i)->d_buf, 512); // 표준 입력에서 최대 512바이트를 읽어 buf3[i].d_buf에 저장
        (buf3+i)->d_nread = n; // buf3[i].d_nread에 읽은 바이트 수 저장
    }

    for(i=0;i<5;i++)
        printf("%d ... %s\n", (buf3+i)->d_nread, (buf3+i)->d_buf); // buf3에 저장된 내용을 출력

    shmctl(shmid1, IPC_RMID, 0); // shmid1에 연결된 공유 메모리 제거
    shmctl(shmid2, IPC_RMID, 0); // shmid2에 연결된 공유 메모리 제거
    shmctl(shmid3, IPC_RMID, 0); // shmid3에 연결된 공유 메모리 제거
    exit(0);
}