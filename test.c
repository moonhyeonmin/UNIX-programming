#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <signal.h>
#include <string.h>
#include <sys/mman.h>

# define MAX 10
typedef struct {
    char type;
    union {
        int i_val;
        float f_val;
        char c_val;
    } value;
} Data;

// 전역 변수 선언
int fd1, fd2;
int nprint = 0;
int nscanf = 0;
int mode = 0;
int NUM;
int cnt = 0;
Data *log1_data;
int *log2_data;

void r_init(void) {
    if (access("./TEMP", F_OK) == -1) {
        mkdir("TEMP", 0700);

        fd1 = open("./TEMP/LOG1", O_RDWR | O_CREAT, 0600);
        fd2 = open("./TEMP/LOG2", O_RDWR | O_CREAT, 0600);

        ftruncate(fd1, sizeof(Data) * 10);
        ftruncate(fd2, sizeof(int) * 2);

        log1_data = mmap(NULL, sizeof(Data) * 10, PROT_READ | PROT_WRITE, MAP_SHARED, fd1, 0);
        log2_data = mmap(NULL, sizeof(int) * 2, PROT_READ | PROT_WRITE, MAP_SHARED, fd2, 0);

        if (log1_data == MAP_FAILED || log2_data == MAP_FAILED) {
            perror("mmap 실패");
            exit(1);
        }

        nprint = 0;
        nscanf = 0;
        log2_data[0] = 0;
        log2_data[1] = 0;

        mode = 0;
    } else {
        fd1 = open("./TEMP/LOG1", O_RDWR);
        fd2 = open("./TEMP/LOG2", O_RDWR);

        log1_data = mmap(NULL, sizeof(int) * 10, PROT_READ | PROT_WRITE, MAP_SHARED, fd1, 0);
        log2_data = mmap(NULL, sizeof(int) * 2, PROT_READ | PROT_WRITE, MAP_SHARED, fd2, 0);

        if (log1_data == MAP_FAILED || log2_data == MAP_FAILED) {
            perror("mmap 실패");
            exit(1);
        }

        nscanf = log2_data[0];
        nprint = log2_data[1];

        printf("r_init: 복구된 nscanf = %d, nprint = %d\n", nscanf, nprint);
        mode = 1; // 복구 모드 활성화
    }
}

void r_scanf(char* format, void* input) {
    if (mode == 1) { // 복구 모드 처리
        printf("\n복구 모드 활성화. 저장된 입력 횟수: %d\n", nscanf);
        printf("돌아갈 인덱스 (0 ~ %d): ", nscanf - 1);
        scanf("%d", &NUM);
        

        if (NUM >= 0 && NUM < nscanf) {
            printf("복구 완료: 유효한 인덱스 %d로 복구합니다.\n", NUM);
            nscanf = NUM;
            log2_data[0] = nscanf;
        } else {
            printf("잘못된 인덱스입니다. 복구를 중단합니다.\n");
        }

        mode = 0;
    } else {
        printf("데이터 타입을 입력하세요 ('i': 정수, 'f': 실수, 'c': 문자): ");
        char type;
        scanf(" %c", &type); // 데이터 타입 입력

        if (type == 'i') {
            printf("정수를 입력하세요: ");
            scanf("%d", (int*)input);
            log1_data[nscanf].type = 'i';
            log1_data[nscanf].value.i_val = *(int*)input;
        } else if (type == 'f') {
            printf("실수를 입력하세요: ");
            scanf("%f", (float*)input);
            log1_data[nscanf].type = 'f';
            log1_data[nscanf].value.f_val = *(float*)input;
        } else if (type == 'c') {
            printf("문자를 입력하세요: ");
            scanf(" %c", (char*)input);
            log1_data[nscanf].type = 'c';
            log1_data[nscanf].value.c_val = *(char*)input;
        } else {
            printf("잘못된 타입입니다.\n");
            return;
        }

        nscanf++;
        log2_data[0] = nscanf;
    }
}

void r_printf(float value, char* format) {
    if (nprint < nscanf || mode == 0) {
        log2_data[1] = ++nprint;
        if (strstr(format, "%d") != NULL) {
            printf(format, (int)value);
        } else {
            printf(format, (double)value);
        }
    }
}

void r_print_all(void) {
    printf("저장된 모든 데이터:\n");
    for (int i = 0; i < nscanf; i++) {
        printf("log1_data[%d]: ", i);
        if (log1_data[i].type == 'i') {
            printf("정수 = %d\n", log1_data[i].value.i_val);
        } else if (log1_data[i].type == 'f') {
            printf("실수 = %f\n", log1_data[i].value.f_val);
        } else if (log1_data[i].type == 'c') {
            printf("문자 = %c\n", log1_data[i].value.c_val);
        } else {
            printf("알 수 없는 타입\n");
        }
    }
    printf("\n");
}

void r_cleanup(void) {
    munmap(log1_data, sizeof(Data) * 10);
    munmap(log2_data, sizeof(int) * 2);

    unlink("./TEMP/LOG1");
    unlink("./TEMP/LOG2");
    rmdir("TEMP");
}

int main(void) {
    pid_t pid;
    int input_value, status;
    int i;
    int continue_flag = 1;

    struct sigaction act_parent = { 0 };
    act_parent.sa_handler = SIG_IGN; // 부모 프로세스에서 SIGINT 무시
    sigaction(SIGINT, &act_parent, NULL);

    while (continue_flag) {
        r_init();

        pid = fork();
        if (pid == 0) { // 자식 프로세스
            struct sigaction act_child = { 0 };
            act_child.sa_handler = SIG_DFL; // 기본 SIGINT 처리
            sigaction(SIGINT, &act_child, NULL);
            while(cnt < MAX) {
                r_scanf("%d", &input_value);
                r_print_all();
            }
        }

        wait(&status);

        if (WIFSIGNALED(status)) {
            continue_flag = 1;
        } else {
            continue_flag = 0;
        }
    }

    r_cleanup();
    return 0;
}
