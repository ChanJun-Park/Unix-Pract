#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

char buf[100];

void fatal(char *str) ;
void catchsigchld(int signo);

int main() {


    while(1) {
        fputs(">", stdout);
        fgets(buf, 100, stdin);
        buf[strlen(buf) - 1] = '\0';

        printf("test:%s\n", buf);
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