#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#define MSGSIZ  63

char *fifo = "fifo";

void catch(int signo) {
    printf("SIGPIPE catched");
}

int main() {
    int fd;
    struct sigaction act;
    sigemptyset(&act.sa_mask);
    act.sa_handler = catch;

    sigaction(SIGPIPE, &act, NULL);

    if (mkfifo("fifo", 0666) == -1) {
        printf("test\n");
    }

    fd = open("fifo", O_WRONLY);

    write(fd, fifo, 4);
    return 0;
}