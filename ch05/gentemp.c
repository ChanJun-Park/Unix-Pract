#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>

static int num = 0;

static char namebuf[20];
static char prefix[] = "./tmp";

char *filename;

char *gentemp(void);
int itoa(int i, char *string);

int main()
{
    int i = 0;
    int fd;

    for (i = 0; i<10; i++) {
        filename = gentemp();
        fd = open(filename, O_CREAT | O_WRONLY);
        close(fd);
    }
    return 0;
}

char *gentemp(void)
{
    int length;
    pid_t pid;

    pid = getpid();

    strcpy(namebuf, prefix);
    length = strlen(namebuf);

    itoa(pid, &namebuf[length]);

    strcat(namebuf, ".");
    length = strlen(namebuf);

    do {
        itoa(num++, &namebuf[length]);
    } while(access(namebuf, F_OK) != -1);

    return namebuf;
}

int itoa(int i, char *string)
{
    int power, j;

    j = i;

    for (power = 1; j >= 10; j/=10)
        power *= 10;
    
    for (; power > 0; power /= 10)
    {
        *string++ = '0' + i/power;
        i %= power;
    }

    *string = '\0';
}