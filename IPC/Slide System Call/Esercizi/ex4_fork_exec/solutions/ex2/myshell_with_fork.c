/******************************************
MODULO: myshell.c
SCOPO: Mini-shell
******************************************/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>

int main (int argc, char *argv[])
{
	pid_t child;
	int status;

	/* Creazione del processo figlio*/
	child=fork();
	if (child == -1) {
		/* errore */
		perror("fork() error");
		exit(3);
	} else if (child == 0) {
		/* figlio: esegue il comando */
		execvp(argv[1],&argv[1]);
		perror("execvp() fails");
		exit(4);
	} else {
		/* padre: deve aspettare il figlio che termini */
  	    while ((child=wait(&status)) != -1);
				printf("Mio figlio ha eseguito il comando ed e' uscito. Termino anch'io.\n");
	}
	exit(0);
}
