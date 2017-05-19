/******************************************
MODULO: mycat.c
SCOPO: Emulazione del comando cat
******************************************/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include "mylib.h" /*Contiene la definizione della funzione syserr()*/

#ifndef SIZE
#define SIZE 512 /*dimensione per il buffer di lettura/scrittura*/
#endif

int main (int argc, char *argv[])
{
	int fd; /*file descriptor*/
	int i, n;
	char buf[SIZE]; /*buffer di lettura/scrittura*/

	if (argc < 2) {
		fprintf(stderr,"Specify at least one argument.\n");
		exit(1);
	}

	for(i=1; i < argc; ++i)
    {
		printf("********** file: %s ***********\n", argv[i]);

		/*apertura del file da leggere*/
		if ((fd = open(argv[i],O_RDONLY,0644)) == -1) {
			syserr("mycat", "Cannot open file.");
		}

		/*lettura del contenuto del file nel buffer*/
		while ((n = read(fd,buf,SIZE)) > 0)
        {
			/*scrittura del contenuto del buffer nello stdout*/
			write(1,buf,n);
		}
		printf("*************************************\n");
	}

    return 0;
}

