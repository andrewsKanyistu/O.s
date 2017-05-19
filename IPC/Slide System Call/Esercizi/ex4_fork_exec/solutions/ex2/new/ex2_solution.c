/**
 * @file
 * The solution of exercise number 2.
 *
 * @author K.
 */


#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

#include "ex2_solution.h"


int main(int argc, char * argv[])
{
    if (argc == 1)
    {
        /* Error! */
        fprintf(stderr, "No command passed as command line argument.\n");
        return EXIT_FAILURE;
    }

    pid_t childPid = forkExec(argv +1);

    int status;
    wait(&status);

    if (! WIFEXITED(status))
    {
        fprintf(stderr, "Child terminanted in an unexpected way (e.g. due to a signal)\n");
        return EXIT_FAILURE;
    }

    int childExitCode = WEXITSTATUS(status);
    if (childExitCode == EXIT_FAILURE)
    {
        fprintf(stderr, "Child terminated with an error code.\n");
        return EXIT_FAILURE;
    }

    printf("\n---------------------------------------------------\n");
    printf("Command executed successfully. Its PID was: %d\n", childPid);
    printf("---------------------------------------------------\n");

    return EXIT_SUCCESS;
}



pid_t forkExec(char * const argv[])
{
    pid_t status = fork();
    if (status == -1)
    {
        fprintf(stderr, "fork() failed :(\n");
        exit(EXIT_FAILURE);
    }
    else if (status == 0)
    {
        /* Child */
        execvp(argv[0], argv);
        exit(EXIT_FAILURE);
    }
    return status;
}
