/******************************************
MODULO: myshell.c
SCOPO: Mini-shell
******************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>

#define MAXARG 20

void execute(char *argv[], int mode);
char getargs(int *argcp, char *argv[], int max);

int main (){
	char *argv[MAXARG+1];
	int argc;

	while(1) {

		printf("> ");

		if(!getargs(&argc, argv, MAXARG)|| argc == 0)
			continue;

		if(strncmp(argv[0], "exit", 4) == 0)
			exit(0);

		/*
			controllo se l'ultimo elemento della linea di comando è
			il simbolo & per avviare l'esecuzione del comando in
			background.
		*/

		if(strncmp(argv[argc-1], "&", 1) == 0) {

			/* esecuzione background */
			argv[argc-1] = NULL; /* sostituisco & con NULL per la exec !!!*/
			execute(argv, 0);

		/* esecuzione foreground */
		} else {
			execute(argv, 1);
		}
	}
}


char getargs(int *argcp, char *argv[], int max){
	static char cmd[100];
	char *cmdp;
	int i;

    /* gets() has security problems, due to missing buffer overflow checks.
     * Prefer using fgets(). See man gets().
     */
	if(gets(cmd) == NULL)
		exit(0);

	cmdp = cmd;

	for(i=0; i<=max; i++){

		if((argv[i] = strtok(cmdp, " \t")) == NULL)
			break;

		cmdp = NULL;
	}

	if(i > max){
		printf("Too many arguments\n");
		return -1;
	}

	*argcp = i;
	return(1);
}


void execute(char *argv[], int mode) {

	switch (fork()) {
		case -1:
			printf("Can't create new process\n");
			return;

		case 0: /* codice del figlio */
			fflush(stdout); /* svuoto il buffer dello stdout */
			execvp(argv[0], argv);
			printf("Can't execute\n");
			perror("execvp");
			exit(1);

		default: /* codice del padre */
			/*
				attendo il termine dell'esecuzione del figlio solo se mode = 1
				ovvero se ho lanciato un comando in foreground !! */

			if(mode) {
				if(wait(NULL) == -1)
					perror("wait");
			}
	}
}
