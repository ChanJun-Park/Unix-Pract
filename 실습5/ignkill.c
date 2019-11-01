#include <sys/wait.h>
#include <signal.h>

int main() {
    signal(SIGKILL, SIG_IGN);
    sleep(600);
    return 0;
}
