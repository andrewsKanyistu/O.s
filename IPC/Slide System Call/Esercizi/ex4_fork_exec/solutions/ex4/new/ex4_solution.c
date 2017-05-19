/**
 * @file
 * The solution of exercise number 4.
 *
 * @author K.
 */

/* Feauture test macro for strdup() */
#define _BSD_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

#include "ex4_solution.h"


int main()
{
    char ** argv = NULL;
    int argc = 0;

    while (1)
    {
        int isBackground = getCommand(&argc, &argv);

        if (argv != NULL && argv[0] != NULL
            && strcmp(argv[0], "exit") == 0)
        {
            freeArgv(argv);
            break;
        }

        forkExec(argv);
        freeArgv(argv);

        if (!isBackground) wait(NULL);
    }

    free(argv);
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


int getCommand(int * argc, char *** argv)
{
    const char delimiters[] = " \t\n";
    char commandString[51];
    commandString[sizeof(commandString)-1] = '\0';
    int current = 0;

    /* The happy prompt: */
    printf(":-) > ");
    fgets(commandString, sizeof(commandString)-1, stdin);
    fflush(stdin);

    char * tk = strtok(commandString, delimiters);
    while (tk != NULL)
    {
        if (current >= *argc)
        {
            /* Resizing argv */
            *argv = realloc(*argv, current +1);
            ++ *argc;
        }

        (*argv)[current] = strdup(tk);
        ++current;

        tk = strtok(NULL, delimiters);
    }

    int ret = 0;
    if (current != 0 && strcmp((*argv)[current-1], "&") == 0)
    {
        --current;
        free((*argv)[current]);
        ret = 1;
    }

    if (*argv != NULL)
    {
        /* NULL terminating the array. */
        (*argv)[current] = NULL;
    }

    return ret;
}


void freeArgv(char **argv)
{
    if (argv == NULL) return;
    for (register char ** i = argv; *i != NULL; ++i)
    {
        free(*i);
        *i = NULL;
    }
}
