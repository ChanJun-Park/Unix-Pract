#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#define MSGSIZE 16

char *msg1 = "hello, world #1";
char *msg2 = "hello, world #2";

int main()
{
    char inbuf[MSGSIZE];
    int p[2], j;
    pid_t pid;

    /* 파이프를 개방한다. */
    if (pipe(p) == -1) {
        perror("pipe call error");
        exit(1);
    }

    pid = fork();
    if (pid == 0) { // 자식 프로세스
        close(p[0]);
        write(p[1], msg1, MSGSIZE);
        return 0;
    }

    pid = fork();
    if (pid == 0) { // 자식 프로세스
        close(p[0]);
        write(p[1], msg2, MSGSIZE);
        return 0;
    }

    // 부모 프로세스
    close(p[1]);
    for (j = 0; j < 2; j++) {
        read(p[0], inbuf, MSGSIZE);
        printf("%s\n", inbuf);
    }
    wait(NULL);
    wait(NULL);
    return 0;
}
