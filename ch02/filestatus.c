#include <fcntl.h>
#include <stdio.h>

int filestatus(int filedes)
{
    int arg1;

    if ((arg1 = fcntl(filedes, F_GETFL)) == -1)
    {
	printf ("filestatus failed\n");
	return -1;
    }

    printf("File descriptor %d: ", filedes);
    
    /* 개방시의 플래그를 테스트한다 */
    switch (arg1 & O_ACCMODE) {
    case O_WRONLY:
	printf("write-only");
	break;
    case O_RDONLY:
	printf("read-only");
	break;
    case O_RDWR:
	printf("read write only");
	break;
    default:
	printf("no such mode");
	break;
    }
    if (arg1 & O_APPEND)
	printf(" - append flag set");

    printf("\n");
    return 0;
}

int main() {

    int filedes;
    filedes = open("memo", O_WRONLY);
    filestatus(filedes);

    return 0;
}
