#include <stdio.h>
#include <signal.h>

int main()
{
    pid_t pid;
    pid = fork();
    if (pid == 0) {
	kill(getppid(), SIGKILL);
	printf("I Killed parent\n");
    }
    else if (pid >0) {
	sleep(10);
	printf("You Immoral!!\n");
    }

    return 0;
}
