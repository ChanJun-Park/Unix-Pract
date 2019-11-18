#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#define FIFO "/tmp/fifo"
#define BUFSIZE 512

int main() {
    int fd;
    int res;
    char buffer[BUFSIZE];
    while((fd = open(FIFO, O_RDWR | O_NONBLOCK)) < 0) {
        if (errno != ENOENT)
            return -1;
        printf("Pipe is not ready, waiting 1 sec.\n");
    }
    while(1) {
        res = read(fd, buffer, BUFSIZE);
        if (res < 0) {
            if (errno != EAGAIN)
                break;
            sleep(1);
            continue;
        }
        printf(">> %s", buffer);
    }
    close(fd);
    return 0;
}