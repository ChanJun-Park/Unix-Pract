#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

static void fpe(int unused) {
    printf("SIGFPE caught. \n");
    exit(1);
}

int main()
{
    int error;
    struct sigaction act;
    sigemptyset(&act.sa_mask);
    act.sa_flags = SA_ONESHOT;
    act.sa_handler = fpe;
    sigaction(SIGFPE, &act, NULL);
    error = 4 / 0;
    return 0;
}
