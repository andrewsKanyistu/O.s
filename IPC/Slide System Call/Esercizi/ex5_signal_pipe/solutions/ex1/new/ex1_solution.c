/**
 * @file
 * The solution of exercise number 1.
 *
 * @author K.
 */

/* Feature test macro for kill() */
#define _POSIX_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

#include "ex1_solution.h"

int main()
{
    /* Registering befor fork. Children will inherit this mask. */
	signal(SIGUSR1, signalHandler);
	signal(SIGUSR2, signalHandler);

    pid_t child1Pid = forkCall(child1);
    pid_t child2Pid = forkCall(child2);

    /* Waiting 1st signal */
    pause();
    sleep(5);

    printf("[Parent pid = %d] Sending signal to 2nd child.\n", getpid());
    kill(child2Pid, SIGUSR1);

    /* Waiting 2nd signal */
    pause();
    sleep(5);

    printf("[Parent pid = %d] Sending signal to 1st child.\n", getpid());
    kill(child1Pid, SIGUSR2);

    wait(NULL);
    wait(NULL);

    printf("[Parent pid = %d] Terminating.\n", getpid());

    return EXIT_SUCCESS;
}


pid_t forkCall(const FunctionPointer func)
{
    pid_t status = fork();
    if (status == -1)
    {
        fprintf(stderr, "fork() failed :(\n");
        exit(EXIT_FAILURE);
    }
    else if (status == 0)
    {
        /* child */
        int ret = (*func)();
        exit(ret);
    }

    return status;
}


void signalHandler(int s)
{
    // Nothing special to do...
    // Just print something and setting again this handler to assure portability.
    if (s == SIGUSR1)
    {
        printf("[?????? pid = %d] Received signal SIGUSR1.\n", getpid());
        signal(SIGUSR1, signalHandler);
    }
    else if (s == SIGUSR2)
    {
        printf("[?????? pid = %d] Received signal SIGUSR2.\n", getpid());
        signal(SIGUSR2, signalHandler);
    }
    else
    {
        fprintf(stderr, "[?????? pid = %d] Received unexpected signal. "
                "This should never happen... Signal = %d\n", getpid(), s);
    }
}


int child1(void)
{
    sleep( 5 );

    printf("[Child1 pid = %d] Sending signal to parent.\n", getpid());
    kill(getppid(), SIGUSR1);

    printf("[Child1 pid = %d] Waiting for Child2 signal.\n", getpid());
    pause();

    printf("[Child1 pid = %d] Terminating.\n", getpid());

    return EXIT_SUCCESS;
}

int child2(void)
{
    printf("[Child2 pid = %d] Waiting for Child1 signal.\n", getpid());
    pause();

    sleep( 5 );

    printf("[Child2 pid = %d] Sending signal to parent.\n", getpid());
    kill(getppid(), SIGUSR2);

    printf("[Child2 pid = %d] Terminating.\n", getpid());

    return EXIT_SUCCESS;
}

