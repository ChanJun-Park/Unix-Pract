/*
    hotel.c
    호텔의 투숙객 정보를 관리하는 프로그램
    투숙객 정보는 프로그램 실행시 명령줄 인자로 전달된다.
    호텔은 총 10개의 방을 가지고 있다고 가정한다.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

// 실행을 간단히 하기 위해서 투숙객 정보는 7문자로 제한했다. 마지막 문자는 개행문지이다.
#define NAMELENGTH  7
#define NROOMS  10

char namebuf[NAMELENGTH];
int infile = -1;
char* listfile;

void printCommand();
void nameFormatting(char * name);
int isEmpty(const char* resident);
char* getoccupier(int roomno);
int findfree();
int freeroom(int roomno);
int addguest(int roomno, const char* newuser);

int main(int argc, char* argv[]) {

    int roomindex, namelen;
    char command[1000];
    char roomnumber[1000];
    char username[100];

    if (argc < 2) {
        printf("program usage : hotel residents_list_file_name\n");
        return 1;
    }

    listfile = argv[1];
    if ((infile = open(listfile, O_RDWR)) == -1) {
        perror("error in opening residents list file\n");
        return 1;
    }
    memset(command, 0, sizeof(command));
    memset(username, ' ', sizeof(username));

    printf("Welcome! This is Hotel frontdesk program!\n");
    printf("\n");
    printCommand();

    while(1) {
        printf("> ");
        scanf("%s", command);
        if(strcmp(command, "exit") == 0) break;
        else if(strcmp(command, "emproom") == 0) {

            roomindex = findfree();
            if (roomindex == -1) {
                printf("빈방을 찾을 수 없음\n");
                continue;
            }
            else {
                printf("%d room is empty\n", roomindex);
            }
        }
        else if(strcmp(command, "checkin") == 0) {

            printf("체크인 할 방번호와 투숙객 이름(6자 이하)을 입력하시오\n");
            
            scanf("%s ", roomnumber);
            fgets(username, sizeof(username), stdin);

            namelen = strlen(username);
            username[namelen - 1] = '\0';   // fgets으로 들어온 개행문자 제거
            nameFormatting(username);

            addguest(atoi(roomnumber), username);
        }
        else if (strcmp(command, "checkout") == 0) {

            printf("체크아웃 할 방 번호를 입력하시오\n");
            scanf("%s", roomnumber);

            freeroom(atoi(roomnumber));
        }
        else if (strcmp(command, "help") == 0) {
            printCommand();
        }
        else {
            printf("잘못된 명령어입니다\n");
        }
    }

    close(infile);
    return 0;
}

void printCommand() 
{
    printf("-------------------------------------------\n");
    printf("프로그램 종료 : exit\n");
    printf("빈방 출력 : emproom\n");
    printf("체크인 : checkin\n");
    printf("체크아웃 : checkout\n");
    printf("프로그램 명령어 보기 : help\n");
    printf("-------------------------------------------\n\n");
}

// 투숙객 정보를 한 라인당 정확히 41자로 맞춰주는 함수
void nameFormatting(char * name)
{
    int i;
    int len;
    len = strlen(name);

    for (i = len; i < NAMELENGTH - 2; i++) {
        name[i] = ' ';          // 40자가 안되는 이름 뒤에는 모두 공백으로 채운다.
    }
    name[NAMELENGTH - 1] = '\n';
}

// 방이 비어있는지 확인한다. 비어있는 방의 투숙객 정보는 모두 공백으로 채워진다.
int isEmpty(const char* resident) 
{
    for (int i = 0; i < NAMELENGTH - 1; i++) {
        if(resident[i] != ' ') return 0;
    }

    return 1;
}

// roomno에 해당하는 호실의 투숙객 정보를 반환한다.
char* getoccupier(int roomno)
{
    off_t offset;
    ssize_t nread;

    // 파일 열기 실패
    if (infile == -1 && (infile = open(listfile, O_RDONLY)) == -1) {
        fprintf(stderr, "error in opening %s\n", listfile);
        return NULL;
    }
        

    // 방번호로부터 offset 계산
    offset = (roomno - 1) * NAMELENGTH;

    // offset 으로 파일 포인터 이동
    if (lseek(infile, offset, SEEK_SET) == -1) {
        perror("Error on setting offset \n");
        return NULL;
    }

    //offset부터 파일 읽기
    if ((nread = read(infile, namebuf, NAMELENGTH)) <= 0) {
        perror("Error on reading offset \n");
        return NULL;
    }

    namebuf[nread-1] = '\0';
    return (namebuf);
}

// 비어있는 방의 번호를 반환한다.
int findfree() 
{
    int i;
    char* resident;
    for (int i = 1; i < NROOMS; i++) {
        resident = getoccupier(i);
        if(resident == NULL) {
            fprintf(stderr,"error in reading resident %d\n", i);
            return -1;
        }
        if (isEmpty(resident)) {
            return i;
        }
    }

    printf("There is no empty room\n");
    return -1;
}

// 인자로 전달한 번호에 해당하는 방을 비운다. 
// 비워진 방의 투숙객 정보는 공백문자로 채운다.
int freeroom(int roomno)
{
    off_t offset;
    int i;
    char buffer[41];
    int writecnt;

    offset = NAMELENGTH * (roomno -1);
    if(lseek(infile, offset, SEEK_SET) == -1) {
        perror("error in freeroom\n");
        return -1;
    }

    // 투숙객 정보를 공백으로 채운다.
    memset(buffer, ' ', sizeof(buffer)-1);
    buffer[NAMELENGTH - 1] = '\n';

    if ((writecnt = write(infile, buffer, NAMELENGTH)) < NAMELENGTH) {
        perror("error in freeroom\n");
        return -1;
    }

    printf("%d번방 체크아웃 성공\n", roomno);
    return 1;
}

// 방이 비어있으면 새로운 투숙객을 집어넣는다.
int addguest(int roomno, const char* newuser)
{
    char select;
    char* resident = getoccupier(roomno);
    int writecnt;
    off_t offset = (roomno - 1) * NAMELENGTH;

    if(resident == NULL) {
        perror("error in reading resident,(in addguest)\n");
        return -1;
    }
    
    if(!isEmpty(resident)) {
        
        while(1) {
            printf("%d번 방은 비어있는 방이 아닙니다.\n", roomno);
            printf("기존 투숙객을 밀어내고 새로운 투숙객을 추가하시겠습니까?[y/n] :");
        
            select = getchar();
            getchar();

            if(select == 'Y' || select == 'y') {
                break;
            }
            else if(select == 'N' || select == 'n') {
                printf("명령이 취소되었습니다.\n");
                return -1;
            }
            else {
                printf("잘못된 명령입니다\n");
                continue;
            }
        }
    }

       
    if( lseek(infile, offset, SEEK_SET) == -1) {
        perror("error in addguest\n");
        return -1;
    }

    writecnt = write(infile, newuser, NAMELENGTH);
    if (writecnt < NAMELENGTH) {
        perror("error in writing newuser, (in addguest)\n");
        return -1;
    }

    printf("checkin 성공\n");
    return 1;
}
