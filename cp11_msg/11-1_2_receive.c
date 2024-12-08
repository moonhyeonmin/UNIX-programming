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

struct q_entry{
    long mtype;
    int mnum;
};

int main(void){
    int qid, i, in;
    key_t key;
    struct q_entry msg;

    key=ftok("key", 3);
    qid=msgget(key, IPC_CREAT | 0600);
    for(i=0;i<2;i++){
        msgrct(qid, &msg, sizeof(int), 3, 0);
        printf("%d\n", msg.mnum);
    }

    exit(0);
}