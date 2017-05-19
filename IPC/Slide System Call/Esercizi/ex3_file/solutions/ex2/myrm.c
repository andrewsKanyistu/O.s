/******************************************
MODULO: myrm.c
SCOPO: emulazione del comando rm
******************************************/
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include "mylib.h"

int main (int argc, char *argv[])
{
	int i;
	int fd; /*file descriptor*/

	if (argc < 2) {
		fprintf(stderr,"Specificare almeno un argomento.\n");
		exit(1);
	}

	/* scandisco la lista degli argomenti e li rimuovo uno alla volta */
	for (i=1; i < argc; ++i)
    {
		/* Controllo di avere i permessi di scrittura sul file */
		if ((fd = open(argv[i], O_WRONLY, 0600)) == -1) {
			syserr("myrm", "File error.\n");
		} else {
			/* Rimozione del file */
			unlink(argv[i]);
		}
	}

    return 0;
}

