#include <stdio.h>
#include <string.h>
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

    if (infile == -1 && (infile = open("residents", O_RDONLY)) == -1)
        return NULL;

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
    char* resident = getoccupier(roomno);
    int writecnt;
    off_t offset = (roomno - 1) * NAMELENGTH;

    if(resident == NULL) {
        printf("error in readingn resident,(in addguest)\n");
        return -1;
    }
    
    if(isEmpty(resident)) {
        if( lseek(infile, offset, SEEK_SET) == -1) {
            printf("error in addguest\n");
            return -1;
        }

        writecnt = write(infile, newuser, NAMELENGTH);
        if (writecnt < NAMELENGTH) {
            printf("error in writing newuser, (in addguest)\n");
            return -1;
        }
    }

    return 1;
}

int main() {

    int index;
    index = findfree();

    if(index != -1) {
        printf("%d room is empty\n", index);
    }

    close(infile);
    return 0;
}
