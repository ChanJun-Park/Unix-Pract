#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

char buf[100];

void fatal(char *str) ;
void catchsigchld(int signo);

int main() {
    pid_t pid;
    static struct sigaction act;

    act.sa_handler = catchsigchld;
    act.sa_flags = SA_RESTART;
    sigfillset(&(act.sa_mask));         // 처리중에 들어오는 다른 시그널 blocking
    sigaction(SIGCHLD, &act, NULL);

    while(1) {
        fputs(">", stdout);
        fgets(buf, 100, stdin);
        buf[strlen(buf) - 1] = '\0';
        
        printf("test:%s\n", buf);

        switch(pid = fork()) {
        case 0:  // 자식
            execlp("sleep", "sleep", "10", NULL);
            break;
        case -1:
            fatal("main()");
        default: // 부모
            break;
        }
    }

    return 0;
}

void fatal(char *str) {
    perror(str);
    exit(1);
}

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