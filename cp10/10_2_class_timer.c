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

#define MSGSIZE 6
char *msg1 = "hello";
char *msg2 = "bye!!";
void parent(int[][2]);
int child(int[]);

int main(void){
	int pip[3][2];
	int i;

	for(i=0;i<3;i++){
		pipe(pip[i]);
		if(fork() == 0)
			child(pip[i]);
	}
	parent(pip);
	for(i=0;i<3;i++){
		wait(0);
	}
	exit(0);
}

void parent(int p[3][2]){
	char buf[MSGSIZE], ch;
	fd_set set, master;
	int i, j, k, n;
    struct timeval t;

	for(i=0;i<3;i++)
		close(p[i][1]);

    FD_ZERO(&master);
    for(i=0;i<3;i++)
        FD_SET(p[i][0], &master);
    t.tv_sec = 5;
    while(set=master, select(p[2][0] + 1, &set, NULL, NULL, &t) > 0) { //ㅔ[====
        for(i=0;i<3;i++){
            if(FD_ISSET(p[i][0], &set)){
                if(n = read(p[i][0], buf, MSGSIZE) > 0) // n == 0, 쓰고 있는 프로세스가 종료됐을 때
                    printf("MSG from %d=%s\n", i, buf);
                else if(n == 0) // 쓰고 있는 프로세스 종료 -> 해당 배열 0으로 초기화
                    FD_CLR(p[i][0], &master);
            }
        }
        t.tv_sec = 5; // 반복문 1번 돌 때마다 5초씩 대기
    }
}

int child(int p[2]){
	int count;
		
	close(p[0]); // 읽기용 파이프 닫기
	for(count=0;count<2;count++){ //3번 반복해서 씀
		write(p[1], msg1, MSGSIZE); // hello 쓰기
		sleep(getpid()%4); // 0~3초 대기
	}
	write(p[1], msg2, MSGSIZE); // bye 쓰기
	exit(0);
}