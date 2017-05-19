
#include <stdlib.h>
#include <stdio.h>


/* At least 5 errors. */
/* What is calculated? */

enum CONSTS
{
    SIZE = 5,
    LENGTH = 25
};

int main( )
{
    int m1[ SIZE ][ SIZE ];
    int * m2;
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
            r = rand() % SIZE;
            *p = r;
            *(m2 + i + j) = r;
            *(m3 + i + j) = r;
        }
    }
    free ( p - SIZE );

    for ( i = 0, k = 0; i < SIZE; ++i )
    {
        for ( j = 0; j < SIZE; ++j, ++k )
        {
            res[ k ] = m1[ i ][ j ] + (m2 + k) + m3[ i ][ j ];
        }
        free( m3 + i );
    }
    free( m1 );
    free( m2 );
    free( m3 );

    printf( "Result:\n" );
    for ( i = 0; i < LENGTH; ++i )
    {
        printf( "res[ %d ] = %d\n", i, res[ i ] );
    }

    return 0;
}
