#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main() {
    int filedes = creat("pract_2_3_text.txt", 0644);
    if(filedes == -1) {
        printf("파일 오픈에 실패함");
        exit(1);
    }
    
    char buffer[30];
    strcpy(buffer, "test message1");
    write(filedes, buffer, 30);

    int filedes2 = open("pract_2_3_text.txt", O_RDWR);
    if(filedes2 == -1) {
        printf("파일2 오픈에 실패함");
        exit(2);
    }

    strcpy(buffer, "test message2");
    write(filedes2, buffer, 30);

    return 0;
}
