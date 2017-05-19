
#include <stdlib.h>
#include <stdio.h>


/* At least 5 errors. */
/* What is calculated? */

/* This program generates a sequence of random numbers,
 * and then prints each generated number multiplied by 3.
 * The multiplication is performed adding each number
 * with itself 3 times.
 * Each random number is chosen in [0,SIZE).
 */

enum CONSTS
{
    SIZE = 5,
    LENGTH = 25
};

int main()
{
    /* On the stack, thus it is linear. */
    int m1[ SIZE ][ SIZE ];
    /* Heap-allocated array, used as linearized matrix. */
    int * m2;
    /* Heap allocated matrix. */
    int ** m3;
    int r;
    int * p;
    int res[ LENGTH ];
    register unsigned int i;
    register unsigned int j;
    register unsigned int k;

    m2 = (int*) malloc( LENGTH * sizeof( int ));
    m3 = (int**) malloc( SIZE * sizeof( int* ));
    for ( i = 0; i < SIZE; ++i )
    {
        m3[ i ] = (int *) malloc( SIZE * sizeof( int ));
    }

    /* here we calculate matrix[i][j] = r */
    for ( i = 0, p = (int*) m1; i < SIZE; ++i )
    {
        for ( j = 0; j < SIZE; ++j, ++p )
        {
            /* This is not the best way: check the man page about rand(). */
            r = rand() % SIZE;
            /* This works because m1 is linear. */
            *p = r;
            /* This formula calculates the respective of m2[i][j]. */
            /* It is not possible to use m2[i][j] since m2 is not a matrix. */
            *(m2 + i * SIZE + j) = r;
            /* Equivalent to m3[i][j].*/
            *(*(m3 + i) +j) = r;
        }
    }
    /* (p-SIZE) points to m1.
     * Not need to free m1, since it is on the stack:
     * free ( p - SIZE );
     */

    for ( i = 0, k = 0; i < SIZE; ++i )
    {
        for ( j = 0; j < SIZE; ++j, ++k )
        {
            /* not m2 + k: either m2[ k ] or *(m2+k) */
            res[ k ] = m1[ i ][ j ] + m2[ k ] + m3[ i ][ j ];
        }
        /* Deref, or m3[ i ], and not just (m3+i) */
        free( *(m3 + i) );
    }
    /* Again, only heap allocated memory should be freed:
     * no need to free m1, which is on the stack.
     * free( m1 );
     */
    free( m2 );
    free( m3 );

    printf( "Result:\n" );
    for ( i = 0; i < LENGTH; ++i )
    {
        printf( "res[ %d ] = %d\n", i, res[ i ] );
    }

    return 0;
}
