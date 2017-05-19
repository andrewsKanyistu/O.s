#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* At least 4 errors. */
/* Given an input string (e.g. "Duffy")
 * this program will print a nice message
 * (e.g. "Hello Duffy, have a nice day!")
 */

static char * message = ", have a nice day";


static char exclamation = '!';


char * create_string( char * h, char * name )
{
    unsigned int new_size;
    /* Creating a string h + message = "Hello ,have a nice day" */
    char * ret = (char *) malloc( strlen( message ) + strlen( h ) + 1 );
    strcpy( ret + strlen( h ), message );
    strncpy( &ret[ 0 ], h, 6 );
    /* Creating in ret the space for the user string */
    new_size = strlen( ret ) + strlen( name ) +1;
    ret = (char*) realloc( ret, new_size );
    memmove(& ret[ strlen( h ) ] + strlen(name), & ret[ strlen( h ) ], strlen(message) );
    strncpy(& ret[ strlen( h ) ], name, strlen( name ) );
    /* Adding the final '!' */
    ret[ new_size -1] = exclamation;
    return ret;
}


int main( int argc, char * argv[] )
{
    char h[ 5 ] = "Hello ";
    char * out;

    if ( argc != 2 )
    {
        printf( "ERROR: required a name as input.\n" );
        exit( 1 );
    }

    out = create_string( h, argv[ 1 ] );

    printf( "%s\n", out );

    free( out );

    return 0;
}
