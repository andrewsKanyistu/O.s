#ifndef EX1_SOLUTION_H
#define EX1_SOLUTION_H

/**
 * @file
 * Convenience header to provide some documentation.
 *
 * @author K.
 */


/**
 * @brief Convenience typedef for child processes main() methods signature
 */
typedef int (* FunctionPointer) (void);

/**
 * @name Support methods.
 * @{
 */

/**
 * @brief Prints PID infos related to the current process.
 * @param name The name of the current process.
 */
void printProcessInfos(const char * const name);

/**
 * @brief Performs a fork(), making the child process to execute the given method.
 * In case of error during fork(), calls exit() passing an error code.
 * The child will never return to execute the caller code.
 * @param func The method that must be executed by the child process.
 * @return The pid of the child process.
 */
pid_t forkCall(const FunctionPointer func);


/** @} */

/**
 * @name Child processes main() methods.
 * @{
 */

/** @brief Method representing the first child main() code. */
int child1(void);

/** @brief Method representing the first child main() code. */
int child2(void);

/** @brief Method representing the first child main() code. */
int grandChild1(void);

/** @brief Method representing the first child main() code. */
int grandChild2(void);

/** @brief Method representing the first child main() code. */
int grandChild3(void);

/** @} */

#endif
