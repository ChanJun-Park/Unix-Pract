#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#define NAMELENGTH  41

char namebuf[NAMELENGTH];
int infile = -1;

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

#define NROOMS  10

int main() {

    int j;
    char *getoccupier (int), *p;

    for (j = 1; j<NROOMS; j++) {
        if (p = getoccupier(j))
            printf("Room %2d, %s\n", j, p);
        else
            printf("Error on room %d\n", j);
    }
    
    return 0;
}
