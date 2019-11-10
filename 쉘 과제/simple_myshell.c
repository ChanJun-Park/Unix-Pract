/*
    filename : simple_myshell.c
    작성자 : 12130397 박찬준
    변경사항
    - 2019-11-05 : (1) cd 명령어 오류 수정. cd 명령어는 bash의 built-in 명령이다.
                   따라서 execv를 이용한 프로세스 변경이 아니라 myshell 자체적으로
                   처리한다.

                   (2) exit 명령어 추가. exit 명령어 역시 bash의 built-in 명령이다.
                   myshell 자체적으로 처리한다.
                   
                   (3) foreground & background 실행 구현. 우선 종료된 자식프로세스를 
                   관리하기 위해 사용하는 wait 함수를 main이 아닌 SIGCHLD 시그널 핸들러
                   에서 호출하게 했다. foreground 명령의 경우 부모 프로세스에서 자식 프로세스의 
                   종료로 발생하는 SIGCHLD가 적절히 처리될때까지 pause함수를 호출하여 
                   대기시킴으로써 구현하였다. background 명령의 경우 pause 함수를 호출하지
                   않게하여 myshell의 while 구문이 계속해서 실행되도록 구현하였다.
    
    - 2019-11-10 : (1) 엔터만 입력하면 세그멘테이션 오류가 발생하면서 종료되는 버그 수정.

                   (2) myshell이 제어키에 의해서 종료되지 않고, 포그라운드 프로세스는 제어키에
                   의해 종료될 수 있도록 시그널 처리.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_CMD_ARG 10
#define MAX_BGND_PROC 100
#define MAX_PATH_SIZE 100
#define MAX_USR_NAME 100
#define TRUE 1
#define FALSE 0
#define FOREGROUND 0
#define BACKGROUND 1

// const char *prompt = "myshell> ";
const char *prompt_front = "myshell:";
char prompt[BUFSIZ];
char* cmdvector[MAX_CMD_ARG];
char  cmdline[BUFSIZ];
char homedirpath[MAX_PATH_SIZE];
pid_t fgnd_process;

void fatal(char *str);
void makeprompt();
int makelist(char *s, const char *delimiters, char** list, int MAX_LIST);
void catchsigchld(int signo);
int findhomepath();
void run_cmd_grp(char** cmd_args, int type);

int main() {
    int i=0;
    int numtokens = 0;
    int status = 0;
    pid_t pid;
    pid_t tmp_pid;
    static struct sigaction act;

    // myshell의 자식 프로세스가 종료하거나 중단되는 경우(SIGCHLD)
    // 필요시 myshell에서 수행해야하는 일들 처리
    act.sa_handler = catchsigchld;
    sigfillset(&(act.sa_mask));         // 처리중에 들어오는 다른 시그널 blocking
    act.sa_flags = SA_RESTART;          // 시스템호출이 시그널 핸들러에 의해 블럭되는 경우 재시작
    sigaction(SIGCHLD, &act, NULL);

    // myshell이 제어키(^C, ^\, ^Z)에 의해서 종료되지 않도록
    // 다음 시그널들에 대해서 무시하도록 설정
    act.sa_handler = SIG_IGN;
    sigemptyset(&(act.sa_mask));
    
    sigaction(SIGINT, &act, NULL);
    sigaction(SIGQUIT, &act, NULL);
    sigaction(SIGTSTP, &act, NULL);

    // background 프로세스가 터미널에 write를 시도할때 발생
    signal(SIGTTOU, SIG_IGN);

    while (1) {

        makeprompt();
  	    fputs(prompt, stdout);
	    fgets(cmdline, BUFSIZ, stdin);
	    cmdline[strlen(cmdline) - 1] ='\0';

        // cd 명령을 감지하고 자식 프로세스가 아니라 myshell 내부적으로 처리하기 위해
        // makelist 함수를 fork() 함수 호출 이전으로 앞당긴다.
        numtokens = makelist(cmdline, " \t", cmdvector, MAX_CMD_ARG);

        if (numtokens == -1) {
            fprintf(stderr, "명령어가 너무 깁니다.\n");
            continue;
        }
        else if (numtokens == 0) { // 엔터키만 입력하는 경우 처리
            continue;
        }

        // shell built-in 명령어들 처리
        if (!strcmp(cmdvector[0], "cd")) {
            if (numtokens == 1) {  // home 디렉토리로 이동
                findhomepath();
                cmdvector[1] = homedirpath;
            }
            else if (numtokens > 2) {
                fprintf(stderr, "cd : 인수가 너무 많음\n");
                continue;
            }
            // 현재 디렉토리 변경
            if (chdir(cmdvector[1]) == -1) {
                fprintf(stderr, " %s ", cmdvector[1]);
                perror(" cd ");                
            }
            continue;
        }
        else if (!strcmp(cmdvector[0], "exit")) {
            exit(0);
        }

        if (!strcmp(cmdvector[numtokens - 1], "&")) {   // 백그라운드 실행이면
            cmdvector[numtokens - 1] = NULL;            // 백그라운드 옵션은 myshell에서 처리
            run_cmd_grp(cmdvector, BACKGROUND);
        }
        else { // 포그라운드 실행
            run_cmd_grp(cmdvector, FOREGROUND);
        }
    }
    return 0;
}

void fatal(char *str) {
    perror(str);
    exit(1);
}

// myshell이 실행되고 있는 현재 작업디렉토리를 표시할 수 있도록
// shell prompt를 만드는 함수
void makeprompt() {
    int len = strlen(prompt_front);

    strcpy(prompt, prompt_front);
    if (getcwd(prompt + len, BUFSIZ - len) == NULL) {
        fatal("error in makeprompt");
    }

    len = strlen(prompt);
    prompt[len] = '>';
    prompt[len + 1] = '\0';
}

int makelist(char *s, const char *delimiters, char** list, int MAX_LIST) {	
    int i = 0;
    int numtokens = 0;
    char *snew = NULL;

    if( (s==NULL) || (delimiters==NULL) ) return -1;

    snew = s + strspn(s, delimiters);	/* delimiters�� skip */
    if( (list[numtokens]=strtok(snew, delimiters)) == NULL )
        return numtokens;
        
    numtokens = 1;

    while(1) {
        if( (list[numtokens]=strtok(NULL, delimiters)) == NULL)
            break;
        if(numtokens == (MAX_LIST-1))
            return -1;
        numtokens++;
    }
    return numtokens;
}

// myshell 에서 실행한 프로세스가 종료된 경우 실행되는 SIGCHLD 시그널 핸들러
// 종료된 자식 프로세스를 wait를 통해 거두어들인다.
void catchsigchld(int signo) {
    pid_t pid;
    int status;
    int exit_status;

    if ((pid = wait(&status)) == -1) {
        fatal("error in catchsigchld");
    }

    // foreground 프로세스가 종료된 경우 main함수 내부의 pause에서 
    // 탈출할 수 있도록 fgnd_process를 0으로 설정한다.
    if (pid == fgnd_process) {
        fgnd_process = 0;
    }
}

// 제어 단말기를 사용하는 사용자의 홈 디렉토리를 계산한다.
int findhomepath() {
    char *username;
    int len;

    if ( (username = getlogin()) == NULL) {
        return -1;
    }
    
    strcpy(homedirpath, "/home/");
    
    len = strlen(homedirpath);
    strcpy(&homedirpath[len], username);

    return 0;
}

// myshell에 입력한 명령어를 처리하는 함수
void run_cmd_grp(char** cmd_args, int type) {
    pid_t pid = 0;
    static struct sigaction nact;

    switch(pid=fork()){
    case 0:     // 자식 프로세스
        // 새로운 프로세스 그룹 생성
        setpgid(0, 0);
        
        // 자식프로세스는 제어키(^C, ^\, ^Z)에 대해서 디폴트 액션을 취하도록 설정
        nact.sa_handler = SIG_DFL;
        sigaction(SIGINT, &nact, NULL);
        sigaction(SIGQUIT, &nact, NULL);
        sigaction(SIGTSTP, &nact, NULL);  // 프로세스 중단 수정 필요
        
        if (type == FOREGROUND) {   // 자식 프로세스가 터미널에 대한 제어권 획득
            tcsetpgrp(STDIN_FILENO, getpgid(0));
        }

        execvp(cmdvector[0], cmdvector);      
        fatal("main()");
    case -1:
        fatal("main()");
    default:    // 부모 프로세스(myshell)
        if (type == FOREGROUND) {    // 백그라운드 실행이 아닐때 pause를 통해서 동기화시킨다.
            fgnd_process = pid;
            while(fgnd_process != 0) {  // foreground 프로세스가 종료되지 않은 경우에 background 프로세스의 
                pause();                // 종료에 의해서 pause가 풀리지 않도록 계속해서 pause한다.
            }
            tcsetpgrp(STDIN_FILENO, getpgid(0)); // 쉘은 기다렸다 제어권 회수
        }
    }
}
