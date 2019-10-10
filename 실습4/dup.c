#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#define TEST "test.txt"

int main()
{
    int fd = open(TEST, O_RDWR | O_CREAT, 0644);
    printf("Before dup2()\n");
    dup2(fd, 1);
    printf("After dup2()\n");
    close(fd);
    return 0;
}
