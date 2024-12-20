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
    int i, in, pid, p[2][2];

    for(i=0;i<2;i++){
        pipe(p[i]);
    }

    pid = fork();
    if(pid==0){
        close(p[1][0]); // 자식 읽기용 파이프 닫기
        close(p[0][1]); // 부모 쓰기용 파이프 닫기

        for(i=0; i<3; i++) {
            if(read(p[0][0], &in, sizeof(int))) { // 자식이 부모의 읽기 파이프 읽기를 성공하면
                printf("child reads %d ... \n", in);
                in++;
                printf("child writes %d ... \n", in);
                write(p[1][1], &in, sizeof(int)); // 자신의 쓰기 파이프에 쓰기
            }
            else {
                printf("child read failed ... \n");
            }
        }
        exit(0); // 반복문 끝나면 자식 종료
    }

    close(p[0][0]); // 부모 읽기용 파이프 닫기
    close(p[1][1]); // 자식 쓰기용 파이프 닫기

    for(i=0; i<2; i++){
        scanf("%d", &in);
        printf("parent wirted %d ... \n", in);
        write(p[0][1], &in, sizeof(int)); // 부모의 쓰기 파이프에 쓰기
        read(p[1][0], &in, sizeof(int)); // 자식의 읽기 파이프 읽기
        printf("parent reads %d ... \n", in);
    }

    close(p[0][1]); // 부모 쓰기용 파이프 닫기

    wait(0); // 자식 프로세스가 종료될 때까지 대기
    exit(0);
}