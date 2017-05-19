
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void error( char * msg )
{
    if ( msg )
    {
        fprintf( stderr, "ERROR: %s\n", msg  );
    }
    else
    {
        perror( "ERRROR: terminating the program.\n" );
    }
    exit( 1 );
}


int main()
{
    pid_t status0;
    pid_t status1;
    pid_t status2;
    pid_t status3;

    printf( "Starting process: pid = %d, ppid =  %d\n", getpid(), getppid() );

    status0 = fork();
    if ( status0 == -1 ) error( NULL );

    status1 = fork();
    if ( status1 == -1 ) error( NULL );

    if ( status0 == 0 && status1 == 0 )
    {
        /* Grandchild 1 */
        sleep( 6 );
        printf( "Grandchild1: pid = %d, ppid = %d\n", getpid(), getppid() );
    }
    else if ( status0 == 0 && status1 != 0 )
    {
        /* Child 1 */
        sleep( 2 );
        printf( "Child1: pid = %d, ppid = %d, grandson1 = %d\n", getpid(), getppid(), status1 );
        wait( NULL );
    }
    else if ( status0 != 0 && status1 == 0 )
    {
        /* Child 2 */

        status2 = fork();
        if ( status2 == -1 ) error( NULL );
        if ( status2 == 0 )
        {
            /* Grandchild2 */
            sleep( 8 );
            printf( "Grandchild2: pid = %d, ppid = %d\n", getpid(), getppid() );
            return 0;
        }

        status3 = fork();
        if ( status3 == -1 ) error( NULL );
        if ( status3 == 0 )
        {
            /* Grandchild2 */
            sleep( 10 );
            printf( "Grandchild3: pid = %d, ppid = %d\n", getpid(), getppid() );
            return 0;
        }

        sleep( 4 );
        printf( "Child2: pid = %d, ppid = %d, grandson2 = %d, grandson3 = %d\n",
                getpid(), getppid(), status2, status3 );
        wait( NULL );
        wait( NULL );
    }
    else /* if ( status0 != 0 && status1 != 0 ) */
    {
        /* Parent */
        printf( "Parent: pid = %d, ppid = %d, child1 = %d, child2 = %d\n",
                getpid(), getppid(), status0, status1 );
        wait( NULL );
        wait( NULL );
    }

    return 0;
}
