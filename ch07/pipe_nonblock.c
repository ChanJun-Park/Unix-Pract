#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#define MSGSIZE 6

int parent(int *);
int child(int *);
int fatal(char *);

char *msg1 = "hello";
char *msg2 = "bye!!";

int main()
{
    int pfd[2];

    if (pipe(pfd) == -1)
        fatal("pipe call");
    
    if (fcntl(pfd[0], F_SETFL, O_NONBLOCK) == -1)
        fatal("fcntl call");

    switch(fork()) {
    case -1:
        fatal("fork call");
    case 0:
        child(pfd);
    default:
        parent(pfd);
    }
    return 0;
}

int parent (int p[2])
{
    int nread;
    char buf[MSGSIZE];
    
    close(p[1]);
    
    for(;;) {
        switch(nread = read(p[0], buf, MSGSIZE)) {
        case -1:
            if (errno == EAGAIN) {
                printf("pipe empty\n");
                sleep(1);
                break;
            }
        case 0:
            printf("End of conversation\n");
            exit(0);
        default:
            printf("MSG=%s\n", buf);
        }       
    }
}

int child (int p[2])
{
    int count;

    close(p[0]);

    for (count = 0; count < 3; count++) {
        write(p[1], msg1, MSGSIZE);
        sleep(3);
    }

    write(p[1], msg2, MSGSIZE);
    exit(0);
}

int fatal(char *s)
{
    perror(s);
    exit(1);
}