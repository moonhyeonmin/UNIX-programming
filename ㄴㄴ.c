#define MESSAGE_SIZE 100 // 메시지의 최대 길이 설정
#define TALKER_N 10

typedef struct Message {
        int sender_id;         // 메시지를 보낸 사용자의 ID
        char content[MESSAGE_SIZE]; // 메시지 내용
} Message;

        int talkers           // 현재 통신 중인 사용자 수
        int msg_count;        // 메시지 카운트 (0~9 순환)
        int write_idx;        // 쓰기 인덱스 (다음 쓸 위치) and 새로운 talker가 읽을 위치
} Metadata;



int main(int argc, char *argv[]){
        pid_t pid1, pid2;
        key_t key1 = ftok("keytalk", 1); // 메시지 버퍼
        key_t key2 = ftok("keytalk", 2); // 메타데이터
        int myid = atoi(argv[1]);        //id 설정(sender id)
        int next_read;          //다음에 읽을 msg#
        int next_write;         //다음에 쓸 msg#

        int shmid1 = shmget(key1, sizeof(Message) * TALKER_N, IPC_CREAT | 0600);
        int shmid2 = shmget(key2, sizeof(struct Metadata), IPC_CREAT | 0600);

                return 1;
        }

        Message *msg_buf = (Message *)shmat(shmid1, NULL, 0);
        struct Metadata *meta = (struct Metadata *)shmat(shmid2, NULL, 0);

        if (msg_buf == (void *)-1 || meta == (void *)-1) {
                perror("shmat failed");
                return 1;
        }

        //+C.S 진입< tCS 라고 칭한다.>
        //+첫 프로세스일 경우 데이터 셋팅 
        if(meta.id[myid] == 1){         //이미 사용중인 id 일때 
                printf("이미 사용중인 id");
                //메모리 해제 등 종료 프로세스 
                return 1;
        }
        meta.id[myid] = 1;
        meta.talkers = meta.talkers+1;
        next_read = meta.write_idx;
        printf("id=%d, talkers=%d, msg#=%d\n",myid,meta.talkers,next_read);
        //+C.S 나가기

        pid1 = fork();
        if(pid1 == 0){          //reader
                while(1){
                        if(msg_buf[next_read] != NULL){
                                Message msg = msg_buf[next_read];
                                if(msg.sender_id == myid){
                                        if(strcmp(msg.content,"talk_quit")){
                                                //+종료작업
                                                exit(0);
                                        }
                                        next_read=(next_read+1)%TALKER_N;
                                }
                                printf("[sender=%d & msg#=%d] %s\n",msg.sender_id, msg.msg_n,msg.content);
                                //+C.S 진입
                                msg_buf[next_read].reader_n--;
                                if(msg_buf[next_read].reader_n == 0){ //마지막으로 읽은 reader가 NULL로 변경
                                        msg_buf[next_read] == NULL;
                                        meta.read_idx = (meta.read_idx+1)%TALKER_N;
                                }
                                //+C.S 나가기
                                next_read = (next_read+1)%TALKER_N;
                        }

                }
        Message *msg_buf = (Message *)shmat(shmid1, NULL, 0);
        struct Metadata *meta = (struct Metadata *)shmat(shmid2, NULL, 0);

        if (msg_buf == (void *)-1 || meta == (void *)-1) {
                perror("shmat failed");
                return 1;
        }

        //+C.S 진입
        //+첫 프로세스일 경우 데이터 셋팅 
        if(meta.id[myid] == 1){         //이미 사용중인 id 일때 
                printf("이미 사용중인 id");
                //메모리 해제 등 종료 프로세스 
                return 1;
        }
        meta.id[myid] = 1;
        //tC.S 진입 tC.S 라 부른다 . talker수 동기화를 위한 C.S 
        meta.talkers = meta.talkers+1;
        //tC.S 나가기
        next_read = meta.write_idx;
        printf("id=%d, talkers=%d, msg#=%d\n",myid,meta.talkers,next_read);
        //+C.S 나가기

        pid1 = fork();
        if(pid1 == 0){          //reader
                while(1){
                        if(msg_buf[next_read] != NULL){
                                Message msg = msg_buf[next_read];
                                if(msg.sender_id == myid){
                                        if(strcmp(msg.content,"talk_quit")){
                                                //+종료작업
                                                exit(0);
                                        }
                                        next_read=(next_read+1)%TALKER_N;
                                }
                                printf("[sender=%d & msg#=%d] %s\n",msg.sender_id, msg.msg_n,msg.content);
                                //+C.S 진입
                                msg_buf[next_read].reader_n--;
                                if(msg_buf[next_read].reader_n == 0){ //마지막으로 읽은 reader가 NULL로 변경
                                        msg_buf[next_read] == NULL;
                                        meta.read_idx = (meta.read_idx+1)%TALKER_N;
                                }
                                //+C.S 나가기
                                next_read = (next_read+1)%TALKER_N;
                        }

                }
        if(pid2 == 0){          //writer
                char content[MESSAGE_SIZE];
                while(1){
                        scanf("%s",content);
                        //e 세마포 e > 0
                        //s 세마포 C.S  진입 write를 위한 세마포
                        Message msg;
                        msg.sender_id = myid;
                        msg.msg_n = meta.write.idx;
                        msg.content = content;
                }
                exit(0);
        }


        wait(NULL);
        wait(NULL);
        // 공유 메모리 분리
        shmdt(message_buffer);
        shmdt(meta);

        // 마지막 프로세스라면 공유 메모리 삭제
        shmctl(shmid1, IPC_RMID, NULL);
        shmctl(shmid2, IPC_RMID, NULL);

        printf("Parent: Cleanup complete.\n");
        return 0;

}
