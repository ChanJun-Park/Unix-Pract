#include <stdio.h>
#include <signal.h>
#include <setjmp.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

sigjmp_buf position1;
sigjmp_buf position2;
void goback(int signo);
void goback2(int signo);

int main() {
    static struct sigaction act;

    if (sigsetjmp(position1, 1) == 0) {
        act.sa_handler = goback;
        sigaction(SIGINT, &act, NULL);
    }

    printf("test1\n");

    if (sigsetjmp(position2, 1) == 0) {
        act.sa_handler = goback2;
        sigaction(SIGQUIT, &act, NULL);
    }
        
    printf("test2\n");

    pause();
}

void goback(int signo) {
    fprintf(stderr, "\nInterrupted\n");

    siglongjmp(position1, 1);
}

void goback2(int signo) {
    fprintf(stderr, "\nInterrupted\n");

    siglongjmp(position2, 1);
}