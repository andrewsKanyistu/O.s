/******************************************
MODULO: myshell.c
SCOPO: Mini-shell
******************************************/
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>

#define MAXARG 20

void execute(int argc, char *argv[], int mode);
char getargs(int *argcp, char *argv[], int max);

int main (){
	char *argv[MAXARG+1];
	char *prompt;
	int argc;

	while(1) {
		printf(">");
		if(!getargs(&argc, argv, MAXARG)|| argc == 0)
			continue;

		if(strncmp(argv[0], "exit", 4) == 0)
			exit(0);

		if(strncmp(argv[argc-1], "&", 1) == 0) {	/*background execution*/
			argv[argc-1] = NULL;
			argc--;
			execute(argc, argv, 0);
		} else {																	/*foreground execution*/
			execute(argc, argv, 1);
		}
	}
}


char getargs(int *argcp, char *argv[], int max){
	static char cmd[100];
	char *cmdp;
	int i;

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


void execute(int argc, char *argv[], int mode) {

	int i, j;
	int p[2];
	char *t_argv[MAXARG+1];

	switch (fork()) {

		case -1:
			printf("Can't create new process\n");
			return;

		case 0:	/* codice del figlio */
			fflush(stdout);

			/*
				cercare una eventuale simbolo di pipe (|)
				tra gli argomenti contenuti nel vettore argv[]
				Per capire se esiste il simbolo di pipe basta
				utilizzare la funzione strcnmp per confrontare
				ogni elemento di argv[] con il carattere
				usato per fare una pipe nella shell.
			*/

					/*
						Se si trova il simbolo della pipe:
						1) creare la pipe
						2) creare un figlio (si tratta di un nipote rispetto al main)
						3) nel figlio:
							 a) chiudere il file descriptor 1
							 b) usare la dup per duplicare lo stdout nel
							    canale di scrittura della pipe
							 c) eseguire tramite la exec il comando che sta prima
							    del simbolo della pipe.
						4) nel padre:
							 a) chiudere il file descriptor 0
							 b) usare la dup per duplicare lo stdin nel canale
							    di lettura della pipe.
							 c) eseguire tramite la exec il comando che sta dopo
							    il simbolo della pipe. L'input per questo comando
									verra' preso dal canale di lettura della pipe.
					*/


			/*
				se non ho trovato il il simbolo di pipe eseguo il codice seguente
				per l'esecuzione del comando semplice.
			*/
			execvp(argv[0], argv);
			printf("Can't execute\n");
			perror("execvp");
			exit(1);

		default: /* padre */
			if(mode) { /*
									attendo la terminazione del figlio solo se il comando
									viene eseguito in foreground
								*/
				if(wait(NULL) == -1)
					perror("wait");
			}
	}
}
