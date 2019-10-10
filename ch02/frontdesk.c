#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define NAMELENGTH  41
#define NROOMS  10

char namebuf[NAMELENGTH];
int infile = -1;

int isEmpty(const char* resident) 
{
    for (int i = 0; i < NAMELENGTH - 1; i++) {
        if(resident[i] != ' ') return 0;
    }

    return 1;
}

char* getoccupier(int roomno)
{
    off_t offset;
    ssize_t nread;

    offset = (roomno - 1) * NAMELENGTH;

    if (lseek(infile, offset, SEEK_SET) == -1)
        return NULL;

    if ((nread = read(infile, namebuf, NAMELENGTH)) <= 0)
        return NULL;

    namebuf[nread-1] = '\0';
    return (namebuf);
}

int findfree() 
{
    int i;
    char* resident;
    for (int i = 1; i < NROOMS; i++) {
        resident = getoccupier(i);
        if(resident == NULL) {
            printf("error in reading resident %d\n", i);
            return -1;
        }
        if (isEmpty(resident)) {
            return i;
        }
    }

    printf("There is no empty room\n");
    return -1;
}

int freeroom(int roomno)
{
    off_t offset;
    int i;
    char buffer[41];
    int writecnt;

    offset = NAMELENGTH * (roomno -1);
    if(lseek(infile, offset, SEEK_SET) == -1) {
        printf("error in freeroom\n");
        return -1;
    }

    memset(buffer, ' ', sizeof(buffer)-1);
    buffer[NAMELENGTH - 1] = '\n';

    if ((writecnt = write(infile, buffer, NAMELENGTH)) < NAMELENGTH) {
        printf("error in freeroom\n");
        return -1;
    }

    return 1;
}

// 방이 비어있으면 새로운 투숙객을 집어넣는다.
// 투숙객은 41자로 되어야 하며 마지막 글자는 개행문자여야한다.
int addguest(int roomno, const char* newuser)
{
    char select;
    char* resident = getoccupier(roomno);
    int writecnt;
    off_t offset = (roomno - 1) * NAMELENGTH;

    if(resident == NULL) {
        printf("error in reading resident,(in addguest)\n");
        return -1;
    }
    
    if(!isEmpty(resident)) {
        printf("%d번 방은 비어있는 방이 아닙니다.\n", roomno);
        printf("기존 투숙객을 밀어내고 새로운 투숙객을 추가하시겠습니까?[y/n] :");
        select = getchar();

        if(select == 'Y' || select == 'y') {
        }
        else if(select == 'N' || select == 'n') {
            printf("명령이 취소되었습니다.\n");
            return -1;
        }
        else {
            printf("잘못된 명령입니다\n");
            return -1;
        }
    }

       
    if( lseek(infile, offset, SEEK_SET) == -1) {
        printf("error in addguest\n");
        return -1;
    }

    writecnt = write(infile, newuser, NAMELENGTH);
    if (writecnt < NAMELENGTH) {
        printf("error in writing newuser, (in addguest)\n");
        return -1;
    }

    return 1;
}

void printCommand() 
{
    printf("-------------------------------------------\n");
    printf("프로그램 종료 : exit\n");
    printf("빈방 출력 : emproom\n");
    printf("방 배치 : addguest (방번호) (투숙객이름) \n"); 
    printf("-------------------------------------------\n\n");
}

int main() {

    int roomindex, len;
    char command[1000];
    char roomnumber[1000];
    char username[41];
    memset(command, 0, sizeof(command));
    memset(username, ' ', sizeof(username));

    if (infile == -1 && (infile = open("residents", O_RDWR)) == -1)
        return -1;

    printf("Welcome! This is Hotel frontdesk program\n");
    printf("\n");
    printCommand();

    while(1) {
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
        else if(strcmp(command, "addguest") == 0) {
            printf("방번호와 투숙객 이름을 입력하시오\n");
            scanf("%s ", roomnumber);
            fgets(username, sizeof(username), stdin);
            len = strlen(username);
            username[len - 1] = ' ';
            username[len] = ' ';
            username[len + 1] = ' ';
            username[40] = '\n';

            addguest(atoi(roomnumber), username);
        }
    }
    
    close(infile);
    return 0;
}
