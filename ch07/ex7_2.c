#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>

char *msg1 = "hello, parent";
char *msg2 = "hello, child";

int main() {
    int ptoc[2];
    int ctop[2];
    pid_t pid;
    char buf[100];

    pipe(ptoc), pipe(ctop);

    switch(pid = fork()) {
    case -1:
        perror("fork()");
        exit(1);
        break;
    case 0:
        close(ctop[0]);
        close(ptoc[1]);
        write(ctop[1], msg1, strlen(msg1));
        read(ptoc[0], buf, sizeof(buf));
        printf("message from parent : %s\n", buf);
        return 0;
    default:
        close(ctop[1]);
        close(ptoc[0]);
        write(ptoc[1], msg2, strlen(msg2));
        read(ctop[0], buf, sizeof(buf));
        printf("message from child : %s\n", buf);
        wait(NULL);
        break;
    }


    return 0;
}