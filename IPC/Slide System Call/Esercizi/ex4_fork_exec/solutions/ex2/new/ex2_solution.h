#ifndef EX2_SOLUTION_H
#define EX2_SOLUTION_H

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



#endif
