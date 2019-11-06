/*
    filename : simple_myshell.c
    작성자 : 12130397 박찬준
    변경사항
    - 2019-11-05 : (1) cd 명령어 오류 수정. cd 명령어는 bash의 built-in 명령이다.
                   따라서 execv를 이용한 프로세스 변경이 아니라 myshell 자체적으로
                   처리한다.
                   (2) exit 명령어 추가. exit 명령어 역시 bash의 built-in 명령이다.
                   (3) background 실행 구현. 두가지 방법을 생각했다. 하나는 자식프로세스(a)가
                   다시 자식 프로세스(b)를 fork한 다음, wait하지 않고 바로 종료하여 (b)프로세스가
                   orphant 프로세스로서 init을 부모로 가지게 하는 방법이다. 이렇게 하면 background
                   로 실행되는 프로세스가 종료될때 init이 알아서 wait하게 할 수 있다. 그러나 background
                   프로세스는 myshell의 직계 자손이 되지 않는다. 두번째는 background 프로세스는
                   wait하지 않고 좀비 프로세스가 되도록 하는 것이다. 이는 background 프로세스가 myshell
                   의 직계 자손이 되지만 종료된 background 프로세스를 좀비가 되지 않도록 myshell에서 
                   관리 코드를 넣어주어야 한다. 일단은 두번째 방법으로 구현했다.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#define MAX_CMD_ARG 10

// const char *prompt = "myshell> ";
const char *prompt_front = "myshell:";
char prompt[BUFSIZ];
char* cmdvector[MAX_CMD_ARG];
char  cmdline[BUFSIZ];

void fatal(char *str);
void makeprompt();
int makelist(char *s, const char *delimiters, char** list, int MAX_LIST);
void catchsigchld(int signo);
void clearcmdline();

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
    sigaction(SIGCHLD, &act, NULL);

    while (1) {

        makeprompt();
  	    fputs(prompt, stdout);
	    fgets(cmdline, BUFSIZ, stdin);
	    cmdline[strlen(cmdline) - 1] ='\0';

        // cd 명령을 감지하고 자식 프로세스가 아니라 myshell 내부적으로 처리하기 위해
        // makelist 함수를 fork() 함수 호출 이전으로 앞당긴다.
        numtokens = makelist(cmdline, " \t", cmdvector, MAX_CMD_ARG);

        // shell built-in 명령어들 처리
        if (!strcmp(cmdvector[0], "cd")) {
            chdir(cmdvector[1]);
            continue;
        }
        else if (!strcmp(cmdvector[0], "exit")) {
            exit(0);
        }

	    switch(pid=fork()){
	    case 0:     // 자식 프로세스(shell 명령어)
            if (!strcmp(cmdvector[numtokens - 1], "&")) {   // 백그라운드 실행이면
                cmdvector[numtokens - 1] = NULL;            // 백그라운드 옵션은 myshell에서 처리
            }
            execvp(cmdvector[0], cmdvector);      
		    fatal("main()");
	    case -1:
  		    fatal("main()");
	    default:    // 부모 프로세스(myshell)
            if (strcmp(cmdvector[numtokens - 1], "&")) {    // 백그라운드 실행이 아닐때 pause를 통해서 동기화시킨다.
                // waitpid(pid, &status, 0);
                pause();
            }
	    }
    }
    return 0;
}

void fatal(char *str) {
    perror(str);
    exit(1);
}

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

    printf("\n\n test code : %s \n\n", s);

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

    if (WIFEXITED(status)) {
        exit_status = WEXITSTATUS(status);
        if (exit_status == 0) {
            
        }
    }
}

void clearcmdline() {
    cmdline[0] = '\0';
}