#include <unistd.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <fcntl.h>
#include <errno.h>

int join(char *com1[], char *com2[]) {
    int p[2], status;

    switch(fork()) {
    case -1: fatal("1st fork call in join");
    case 0: break;
    default: wait(&status); return (status);
    }
    if (pipe(p) == -1) fatal("pipe call in join");

    switch(fork()) {
    case -1: fatal("2nd fork call in join");
    case 0:
        dup2(p[1], 1);
        close(p[0]);
        close(p[1]);
        execvp(com1[0], com1);
        fatal("1st execvp call in join");
    default:
        dup2(p[0], 0);
        close(p[0]);
        close(p[1]);
        execvp(com2[0], com2);
        fatal("2st execvp call in join");        
    }
}

int main() {

    exit(0);
}