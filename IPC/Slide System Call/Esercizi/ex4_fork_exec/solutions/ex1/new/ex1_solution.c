/**
 * @file
 * The solution of exercise number 1.
 *
 * @author K.
 */


#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

#include "ex1_solution.h"


int main()
{
    printProcessInfos("Parent");

    pid_t child1Pid = forkCall(child1);
    pid_t child2Pid = forkCall(child2);

    printf("[Parent] child1 pid = %d, child2 pid = %d\n",
           child1Pid, child2Pid);

    wait(NULL);
    wait(NULL);

    return EXIT_SUCCESS;
}



void printProcessInfos(const char * const name)
{
    printf("[%s] pid = %d, parent pid = %d\n", name, getpid(), getppid());
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


int child1(void)
{
    pid_t grandChild1Pid = forkCall(grandChild1);
    sleep( 2 );
    printf("[Child1] grandChild1 pid = %d\n", grandChild1Pid);
    printProcessInfos("Child1");
    wait(NULL);
    return EXIT_SUCCESS;
}

int child2(void)
{
    pid_t grandChild2Pid = forkCall(grandChild2);
    pid_t grandChild3Pid = forkCall(grandChild3);
    sleep( 4 );
    printf("[Child2] grandChild2 pid = %d grandChild2 pid = %d\n", grandChild2Pid, grandChild3Pid);
    printProcessInfos("Child2");
    wait(NULL);
    wait(NULL);
    return EXIT_SUCCESS;
}

int grandChild1(void)
{
    sleep( 6 );
    printProcessInfos("GrandChild1");
    return EXIT_SUCCESS;
}

int grandChild2(void)
{
    sleep( 8 );
    printProcessInfos("GrandChild2");
    return EXIT_SUCCESS;
}

int grandChild3(void)
{
    sleep( 10 );
    printProcessInfos("GrandChild3");
    return EXIT_SUCCESS;
}


