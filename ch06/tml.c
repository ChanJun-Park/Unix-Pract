#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#define TRUE 1
#define FALSE 0
#define BELLS "\007\007\007"

int alarm_flags = FALSE;

void setflag(int sig){
    alarm_flags = TRUE;
}

int main(int argc, char ** argv) {
    int nsecs, j;
    pid_t pid;
    static struct sigaction act;

    if (argc <= 2) {
        fprintf(stderr, "Usage: tml #minutes message\n");
        exit(1);
    }

    if ((nsecs = atoi(argv[1]) * 60) <= 0) {
        fprintf(stderr, " tml : invalid time \n");
        exit(2);
    }

    switch(pid = fork()) {
    case -1:
        perror("tml");
        exit(1);
    case 0:
        break;
    default:
        printf("tml process -id %d\n",pid);
        exit(0);    
    }

    act.sa_handler = setflag;
    sigaction(SIGALRM, &act, NULL);

    alarm(nsecs);

    pause();

    if (alarm_flags == TRUE) {
        printf(BELLS);
        for (j = 2; j < argc; j++) {
            printf("%s", argv[j]);
        }
        printf("\n");
    }

    return 0;
}