#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>

void g_exit(int s) {
    sigset_t set;
    sigaddset(&set, SIGINT);
    sigaddset(&set, SIGQUIT);
    sigprocmask(SIG_SETMASK, &set, NULL);

    unlink("tempfile");
    fprintf(stderr, "Interrupted -- exiting\n");

    sigprocmask(SIG_UNBLOCK, &set, NULL);
    exit(1);
}

int main() {
    static struct sigaction act;
    act.sa_handler = g_exit;
    sigaction(SIGINT, &act, NULL);

    sleep(30);
}