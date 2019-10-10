#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include "hotel.h"
#include "getoccupier.h"

char buffer[LINELENGTH];

char *getoccupier(int roomno) {
    off_t offset;
    ssize_t nread;
    
    // 파일 읽기 실패
    if (listfiledes == -1 && (listfiledes = open(listfile, O_RDONLY)) == -1) {
	fprintf(stderr, "Error on opening list file \n");
	return NULL;
    }

    // 방 번호로부터 offset 계산
    offset = (roomno - 1) * LINELENGTH;

    // offset으로 파일 포인터 이동
    if(lseek(listfiledes, offset, SEEK_SET) == -1) {
	fprintf(stderr, "Error on setting offset \n");
	return NULL;
    }

    //offset부터 파일 읽기
    if((nread = read(listfiledes, buffer, LINELENGTH)) < 0) {
	fprintf(stderr, "Error on reading offset \n");
	return NULL;
    }

    // 각 행 마지막에 있는 개행문자를 NULL문자로 대체하여 하나의 스트링 생성
    buffer[LINELENGTH -1] = '\0'; 

    return buffer;
}
