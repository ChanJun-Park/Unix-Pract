#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

const char * filename = "memo";

int main() {
    printf("리눅스 연습\n");
    
    int filedes;
    printf("이전 메모 복원\n");
    if((filedes = open(filename, O_RDWR)) == -1) {
        printf("파일 열기 실패\n");
        exit(1);
    }


    char buf[1024];
    int num;
    printf("이전 메모 읽기\n");

    while(1) {
        num = read(filedes, buf, 1024);
        if(num == 0) break;
        printf("%s", buf);
    }

    return 0;
}

void printCommandList() {

}
