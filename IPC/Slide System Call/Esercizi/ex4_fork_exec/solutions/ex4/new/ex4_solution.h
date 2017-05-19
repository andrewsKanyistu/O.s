#ifndef EX4_SOLUTION_H
#define EX4_SOLUTION_H

/**
 * @file
 * Convenience header to provide some documentation.
 *
 * @author K.
 */


/**
 * @brief Performs an exec(), making the child process to execute the given parameters.
 * In case of error during fork() or exec(), calls exit() passing an error code.
 * The child will never return to execute the caller code.
 * @param argv The array of parameters.
 * @return The pid of the child process.
 */
pid_t forkExec(char * const argv[]);


/**
 * Reads an input line from stdin, and then tokenizes it.
 * @param argc Where to write the number of read parameters.
 * @param argv Where to write the array of read parameters.
 * @return 1 if process must execute in background, 0 otherwise.
 */
int getCommand(int * argc, char *** argv);


/**
 * Deletes the strings stored into the given array.
 * @param argv The array of strings.
 */
void freeArgv(char **argv);

#endif
