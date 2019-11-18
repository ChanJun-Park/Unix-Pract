#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BUF_SIZE 100
const char * msg1 = "hello world!";

int main() {

    int filedes[2];
    char buffer[BUF_SIZE];

    if (pipe(filedes) < 0) {
        perror("pipe()");
        exit(1);
    }

    write(filedes[1], msg1, strlen(msg1) + 1);

    read(filedes[0], buffer, BUF_SIZE);

    printf("msg from pipe : %s\n", buffer);
    return 0;
}