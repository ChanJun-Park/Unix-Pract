#include "q.h"
#include <stdio.h>
#include <stdlib.h>

int enter(char * objname, int priority);
int warn(char *s);
int init_queue(void);
int serve(void);
int proc_obj(struct q_entry *msg);

int main(int argc, char **argv)
{
    int priority;

    if (argc != 3)
    {
        fprintf(stderr, "usage: %s objname priority\n", argv[0]);
        exit(1);
    }

    if ((priority = atoi(argv[2])) <= 0 || priority > MAXPRIOR)
    {
        warn("invalid priority");
        exit(2);
    }

    if (enter(argv[1], priority) < 0)
    {
        warn("enter failure");
        exit(3);
    }

    exit(0);
}

int enter(char * objname, int priority)
{
    int len, s_qid;
    struct q_entry s_entry;

    if ( (len = strlen(objname)) > MAXOBN)
    {
        fprintf(stderr, "name too long");
        return -1;
    }

    if (priority > MAXPRIOR || priority < 0)
    {
        fprintf(stderr, "invalid priority level");
        return -1;
    }

    if ( (s_qid = init_queue()) == -1)
        return -1;

    s_entry.mtype = (long)priority;
    strncpy(s_entry.mtext, objname, MAXOBN);

    if (msgsnd(s_qid, &s_entry, len, 0) == -1)
    {
        perror("msgsnd failed");
        return -1;
    }
    else
        return 0;
}

int warn(char *s)
{
    fprintf(stderr, "warning: %s\n", s);
}

int init_queue(void)
{
    int queue_id;

    if ( (queue_id = msgget(QKEY, IPC_CREAT| QPERM)) == -1)
        perror("msgget failed");

    return (queue_id);
}

int serve(void) 
{
    int mlen, r_qid;
    struct q_entry r_entry;

    if ((r_qid = init_queue()) == -1)
        return -1;

    for (;;) 
    {
        if ((mlen = msgrcv(r_qid, &r_entry, MAXOBN, (-1 * MAXPRIOR), MSG_NOERROR)) == -1)
        {
            perror("msgrcv failed");
            return -1;
        }
        else
        {
            r_entry.mtext[mlen] = '\0';

            proc_obj(&r_entry);
        }
    }
}

int proc_obj(struct q_entry *msg)
{
    printf("\npriority: %ld name: %s\n", msg->mtype, msg->mtext);
}