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

int main(void){
    int i, in, pid, status, p[2][2];

    for(i=0; i<2; i++)
        pipe(p[i]);

    pid=fork();
    if (pid==0){
        close(p[1][0]); // 자식 읽기전용 파이프 닫기
        close(p[0][1]); // 부모 쓰기전용 파이프 닫기

        fcntl(p[0][0], F_SETFL, O_NONBLOCK);

        read(p[0][0], &in, sizeof(int)); // 자식이 부모의 읽기전용 파이프 읽기
        printf("child reads %d ... \n", in);
        in++;
        printf("child writes %d ... \n", in);
        write(p[1][1], &in, sizeof(int));

        sleep(10);
        exit(15);
    }

    close(p[0][0]); // 부모 읽기전용 파이프 닫기
    close(p[1][1]); // 자식 쓰기전용 파이프 닫기

    scanf("%d", &in);
    printf("parent writes %d ... \n", in);
    write(p[0][1], &in, sizeof(int));
    read(p[1][0], &in, sizeof(int)); // 부모 프로세스에서 자식의 
    printf("parents reads %d ... \n", in);

    wait(&status);
    if (WIFEXITED(status))
        printf("child end normally ... %d\n", WEXITSTATUS(status));
    else if(WIFSIGNALED(status))
        printf("child end signal : %d ... \n", WTERMSIG(status));

    exit(0);
}