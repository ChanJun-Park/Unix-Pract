#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

int main() {

    int fd;
    ssize_t w1, w2;
    char header1[512], header2[1024];

    memset(header1, 0, sizeof(header1));
    memset(header2, 0, sizeof(header2));

    strcpy(header1, "test message1\n\0");
    strcpy(header2, "test message2\n\0");

    if ( (fd = open("ch02_1_7_test.txt", O_WRONLY | O_CREAT | O_EXCL)) == -1)
    {
        printf("error in opening ch02_1_7_test.txt\n");
        return -1;
    }

    w1 = write(fd, header1, 512);
    w2 = write(fd, header2, 1024);

    close(fd);

    return 0;
}
