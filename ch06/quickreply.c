#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

#define TIMEOUT     5
#define MAXTRIES    5
#define LINESIZE    100
#define CTRL_G      '\007'
#define TRUE        1
#define FALSE       0

static int timed_out;
static char answer[LINESIZE];

char *quickreply(char *prompt)
{
    void catch(int);
    int ntries;
    static struct sigaction act, oact;

    /* SIGALRM을 포착하고, 과거 행동을 저장한다. */
    act.sa_handler = catch;
    sigaction(SIGALRM, &act, &oact);

    for (ntries = 0; ntries < MAXTRIES; ntries++) {
        timed_out = FALSE;
        printf("\n%s > ", prompt);

        alarm(TIMEOUT);
        
        gets(answer);

        alarm(0);

        if (!timed_out)
            break;
    }

    sigaction(SIGALRM, &oact, NULL);

    return (ntries == MAXTRIES ? ((char*)0) : answer);
}

void catch(int signo) {
    timed_out = TRUE;
    printf("\n\nInterrupted\n");
    printf("signo : #%d\n", signo);
}

int main()
{
    printf("%s\n", quickreply("say something"));
    return 0;
}