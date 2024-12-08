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

    shmid1 = shmget(key1, 512, IPC_CREAT | 0600);
    shmid2 = shmget(key2, 10 * sizeof(int), IPC_CREAT | 0600);
    shmid3 = shmget(key3, 5 * sizeof(struct databuf), IPC_CREAT | 0600);

    buf1 = (char *)shmat(shmid1, 0, 0);
    buf2 = (int *)shmat(shmid2, 0, 0);
    buf3 = (struct databuf *)shmat(shmid3, 0, 0);

    sleep(10);
    wirte(1, buf1, 512);

    sleep(10);
    for(i=0;i<10;i++)
        printf("%d\n", *(buf2 + i));

    sleep(10);
    for(i=0;i<5;i++)
        printf("%d ... %s\n", (buf3 + i)->d_nread, (buf3 + i)->d_buf);

    shmctl(shmid1, IPC_RMID, 0);
    shmctl(shmid2, IPC_RMID, 0);
    shmctl(shmid3, IPC_RMID, 0);
    exit(0);
}