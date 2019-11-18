#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#define FIFO "/tmp/fifo"
#define BUFSIZE 512

int main() {
    int fd;
    char buffer[BUFSIZE];
    fd = open(FIFO, O_RDWR);
    if (fd < 0)
        return -1;
    while(read(fd, buffer, BUFSIZE) > 0)
        printf(">> %s", buffer);
    close(fd);
    return 0;
}