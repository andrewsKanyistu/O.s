
#include <stdlib.h>
#include <stdio.h>


/* At least 4 errors. */
/* What does this program calculate? */

/* This program calculates for each
 * position i the sum of a random number with
 * all the values in the previous positions.
 * Random numbers are generated between 1 and the array length.
 */


int main( int argc, char * argv[] )
{
    int slen;
    unsigned int len;
    int * data;
    int r;
    register unsigned int i;
    register unsigned int j;

    if ( argc != 2 )
    {
        printf( "Required the number of elements as argument.\n");
        exit( 1 );
    }

    slen = atoi( argv[ 1 ]);
    if ( slen <= 0 )
    {
        printf( "Required a number greater than zero.\n" );
        exit( 1 );
    }

    len = (unsigned int) slen;
    data = (int *) calloc( len, sizeof(int) );

    for ( i = 0; i < len; ++i ) /* and not <= */
    {
        /* Here rand() is used at the best. See the rand() man page for details. */
        r = 1 + (int) (len * (rand() / (RAND_MAX + 1.0)));
        data[ i ] = r;
        for ( j = i; j > 0; --j ) /* and not >= */
        {
            /* and not just data[ i ] */
            data[ i ] += *(int*)( & data[ i ] - ((i -j +1) % len) );
            /* This is just:
             * data[ i ] += data[ j -1 ];
             * The "% len" is just noise...
             */
        }
    }

    printf("Result:\n");
    for ( i = 0; i < len; ++i ) /* and not <= */
    {
        printf("%d ", *(data + i));
    }
    printf("\n");

    return 0;
}
