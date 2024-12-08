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

void do_child(int id, int p[3][2]){
        int i, in, pid=getpid();

        for(i=0;i<3;i++){
                close(p[i][1]);
                if(id!=i)
                        close(p[i][0]);
        }

        while(1){
                read(p[id][0], &in, sizeof(int));
                if(in==-1)
                        exit(0);
                else
                        printf("%d %d\n", pid, in);
        }

}

int main(void){
        int i, in, pid, p[3][2];

        for(i=0;i<3;i++)
                pipe(p[i]);

        for(i=0;i<3;i++){
                if(fork()==0){
                        do_child(i, p);
                }
        }

        close(p[0][0]);
        close(p[1][0]);
        close(p[2][0]);

        for(i=0;i<12;i++){
                scanf("%d", &in);
                write(p[i%3][1], &in, sizeof(int));
        }

        in=-1;
        for(i=0;i<3;i++){
                write(p[i][1], &in, sizeof(int));
        }

        for(i=0;i<3;i++){
                wait(0);
        }
        exit(0);
}
