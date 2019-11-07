#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#define BUFSIZE 32

int main() {
    int fd[2];
    pid_t pid;
    char buf[BUFSIZE];
    pipe(fd);
    pid = fork();
    write(fd[1], "Hello world", BUFSIZE);

    if (pid > 0) {
        while(read(fd[0], buf, BUFSIZE) > 0)
            printf("Message : %s\n", buf);
        wait(NULL);
    }
    close(fd[0]); close(fd[1]);
    return 0;
}