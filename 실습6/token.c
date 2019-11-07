#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#define BUFSIZE 512

int fd[3][2];
char buffer[BUFSIZE];
int parent();
int child1();
int child2();

int main() {
    pid_t pid;
    if (pipe(fd[0]) || pipe(fd[1]) || pipe(fd[2])) {
        perror("pipe()");
        return -1;
    }
    
    pid = fork();
    if (pid == 0) {
        pid = fork();
        if (pid > 0) 
            return child1();
        else if (pid == 0)
            return child2();
        else {
            perror("fork() in child");
            return -3;
        }
    }
    else if (pid < 0) {
        perror("fork()");
        return -1;
    }
    return parent();
}

int parent() {
    close(fd[1][0]); close(fd[1][1]);
    close(fd[2][1]); close(fd[0][0]);
    while(1) {
        gets(buffer);
        if (feof(stdin))
            break;
        write(fd[0][1], buffer, BUFSIZE);
        read(fd[2][0], buffer, BUFSIZE);
        printf("Parent : %s\n", buffer);
        buffer[0] = '\0';
    }
    close(fd[0][1]); close(fd[2][0]);
    return 0;
}

int child1() {
    int res;
    close(fd[2][0]); close(fd[2][1]);
    close(fd[0][1]); close(fd[1][0]);
    while(1) {
        res = read(fd[0][0], buffer, BUFSIZE);
        if (res <= 0)
            break; 
        printf("child1 : %s\n", buffer);
        write(fd[1][1], buffer, BUFSIZE);
    }
    close(fd[1][1]); close(fd[0][0]);
    return 0;
}

int child2() {
    int res;
    close(fd[0][0]); close(fd[0][1]);
    close(fd[1][1]); close(fd[2][0]);
    while(1) {
        res = read(fd[1][0], buffer, BUFSIZE);
        if (res <= 0)
            break; 
        printf("child2 : %s\n", buffer);
        write(fd[2][1], buffer, BUFSIZE);
    }
    close(fd[1][0]); close(fd[2][1]);
    return 0;
}