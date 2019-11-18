#include <unistd.h>
#include <sys/stat.h>
#include <stdio.h>
#include <fcntl.h>
#define FIFO "/tmp/fifo"
#define BUFSIZE 512
#define MODE 0600

int main() {
    int fd;
    char buffer[BUFSIZE];
    if (-1 == mkfifo(FIFO, MODE))
	return -1;
    fd = open(FIFO, O_WRONLY);
    if (fd == -1)
	return -2;
    while(1) {
	printf("> ");
	if(fgets(buffer, BUFSIZE-1, stdin) <= 0)
	    break;
	write(fd, buffer, BUFSIZE);
    }
    close(fd);
    unlink(FIFO);
    return 0;
}
