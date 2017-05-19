/******************************************
MODULO: myshell.c
SCOPO: Mini-shell
La shell si chiude solo quando viene
invocato il comando exit.
******************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>

#define MAXARG 20 /* Massimo numero di argomenti per un comando */

/* Dichiarazione della funzione che esegue il comando */
void execute(char *argv[]);

/* Dichiarazione della funzione che analizza la line di comando */
char getargs(int *argcp, char *argv[], int max);

/* main */
int main (){

	char *argv[MAXARG+1]; /* vettore per memorizzare le varie stringhe
												della linea di comando */

	int argc; /* numero di elementi della linea di comando */

	/* i valori per argv[] e argc vengono calcolati durante la parserizzazione
	dalla funzione getargs */

	while(1) { /* ciclo infinitamente finchè non viene invocato il comando exit */

		printf(">"); /* prompt per la shell */

		/*
			se la getargs ritorna un errore oppure non ci sono comandi da eseguire
			si riparte dall'inizio del while
		*/
		if(!getargs(&argc, argv, MAXARG) || argc == 0)
			continue;

		/*
			se il comando digitato è "exit" allora esco dalla shell.
			la funzione strncmp compara i primi 4 caratteri della stringa contenuta
			in argv[0] con la stringa "exit". strncmp ritorna 0 se sono uguali,
			un valore < 1 se argv[0] è lessicograficamente minore di "exit", un
			valore > 1 se argv[0[ è lessicograficamente maggiore di "exit".
			Vedere man strncmp.
		*/
		if(strncmp(argv[0], "exit", 4) == 0)
			exit(0);

		/* esegue il comando digitato al prompt della shell */
		execute(argv);
	}
}


/* definizione della funzione che analizza la linea di comando */
char getargs(int *argcp, char *argv[], int max){

	static char cmd[100]; /* variabile per memorizzare la linea di comando.
												deve essere static altrimenti quando esco dalla
												funzione il suo valore viene perso */
	char *cmdp;
	int i;

	/*
		la funzione gets legge una linea dallo standard input fino al
		carattere di ritorno a capo e la memorizza nel vettore di caratteri
		cmd. gets ritorna NULL in caso di errore.
	*/
	if(gets(cmd) == NULL)
		exit(0);

	/*
		memorizzo il puntatore alla stringa di comando appena letta
	*/
	cmdp = cmd;

	/* parserizzazione della linea di comando */
	for(i=0; i<=max; i++){

		/*
			la funzione strtok scorre il vettore di caratteri puntato da cmdp
			alla ricerca di un determinato delimitatore
			e ritorna il pezzo di stringa (token) precedente al delimitatore.
			In questo caso il delimitatore è rappresentato da
			un carattere di spazio o dalla una tabulazione.
			La ricerca del token parte dall'inizio della stringa puntata da cmdp.
			Per accedere ai successivi token è necessario richiamare la funzione
			strtok passandogli come primo parametro NULL.
			La funzione ritorna NULL se non vi sono più token nella stringa.
			I token vengono memorizzati una alla volta, grazie al ciclo for,
			negli elementi del vettore di caratteri	argv[].
		*/
		if((argv[i] = strtok(cmdp, " \t")) == NULL)
			break; /* se ho terminato la parserizzazione posso uscire dal for */

		/*
			Setto a NULL il puntatore cmdp per utilizzarlo al prossimo ciclo del
			for come parametro della strtok. In tal modo la strtok riesegue la
			"tokenizzazione" della stringa fornitagli precedentemente proseguendo
			dalla posizione raggiunta nella precedente chiamata.
		*/
		cmdp = NULL;
	}

	/*
		se il numero di argomenti della linea di comando è maggiore di MAX
		stampo un messaggio di errore e ritorno -1. Il comando non può essere
		eseguito.
	*/
	if(i > max){
		printf("Too many arguments\n");
		return -1;
	}

	/*
		memorizzo nella locazione di memoria puntata da argcp il numero di
		argomenti parserizzati, ovvero il numero di argomenti della linea di
		comando.
	*/
	*argcp = i;
	return(1);
}

/* definizione della funzione che esegue il comando */
void execute(char *argv[]) {
	int i;

	/* creazione del processo figlio che eseguirà il comando */
	switch (fork()) {

		case -1: /* la fork non è riuscita a creare il processo */
			printf("Can't create new process\n");
			return;

		case 0: /* questo è il codice del figlio */
			execvp(argv[0], argv); /* eseguo il comando */

			/*
				se la exec va a buon fine non ritorna più e quindi il codice
				sottostante non viene mai eseguito.
			*/
			printf("Can't execute\n");
			perror("execvp");
			exit(1);

		default: /* questo è il codice per il padre */
			if(wait(NULL) == -1) /* attende la terminazione del processo figlio */
				perror("wait");
	}
}
