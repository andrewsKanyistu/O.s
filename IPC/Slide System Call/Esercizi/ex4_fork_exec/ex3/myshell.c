/******************************************
MODULO: myshell.c
SCOPO: Mini-shell
La shell si chiude solo quando viene
invocato il comando exit.
******************************************/
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>

#define MAXARG 20 /* Massimo numero di argomenti per un comando */

/* Dichiarazione della funzione che esegue il comando */
void execute(int argc, char *argv[]);

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

		/* stampare un prompt per la shell */

		... ...

		/* invocare la funzione getargs per la parserizzazione della
		linea di comando digitata al prompt della shell.
		Alla getargs devono essere forniti 3 parametri:
		1) il puntatore alla variabile argc per memorizzare il numero di argomenti
		della linea di comando.
		2) il vettore argv[] per memorizzare gli argomenti della linea di comando.
		3) la costante MAXARG che indica il numero massimo di argomenti gestibili.
		*/

		... ...


		/*
			se il comando digitato è "exit" allora bisogna uscire dalla shell.
			Usare la funzione strcmp per comparare il primo elemento del
			vettore argv[] con la stringa "exit". Vedere man strcmp.
			Se il primo elemento di argv[] è "exit" allora bisogna uscire dal
			programma.

		*/

		... ...

		/*
			Se non è stato digitato il comando exit allora bisogna invocare la
			funzione execute per eseguire il comando.
			Execute necessita di un parametro: il vettore argv[] contenente gli
			elementi della linea di comando da eseguire.
			Ricordare che argv[0] deve essere il nome del comando da eseguire.
		*/

		... ...
	}
}


/* definizione della funzione che analizza la linea di comando */
char getargs(int *argcp, char *argv[], int max){

	static char cmd[100]; /* variabile per memorizzare la linea di comando.
												deve essere static altrimenti quando esco dalla
												funzione il suo valore viene perso */

	char *cmdp; /* puntatore di servizio */

	int i; /* variabile per il ciclo for */


	/*
		leggere la linea digitata dall'utente della shell e memorizzarla
		nel vettore cmd.
	*/

	... ...

	/*
		memorizzo il puntatore alla stringa di comando appena letta
	*/

	cmdp = cmd;

	/*
		parserizzare la linea di comando memorizzata nel vettore cmd tramite un
		ciclo for e l'utilizzo della funzione strtok che permette di
		separare i pezzi della stringa di comando.
		Utilizzare il puntatore cmdp alla prima invocazione della funzione strtok,
		per le volte sucessive assegnare NULL a cmdp per continuare la
		parserizzazione della stringa. VEDERE man strtok !!!!
	*/

	for(i=0; i<=max; i++){

		... ...

	}

	/*
		se il numero di argomenti della linea di comando è maggiore di MAXARG
		stampare un messaggio di errore e ritornare -1.
		Il comando non può essere eseguito perchè possono essere gestiti solo
		MAXARG argomenti.
	*/

	... ...

	/*
		memorizza nella locazione di memoria puntata da argcp il numero di
		argomenti parserizzati, ovvero il numero di argomenti della linea di
		comando.
	*/

	... ...

	return(1);
}

/* definizione della funzione che esegue il comando */
void execute(int argc, char *argv[]) {

	/* creare il processo figlio che eseguirà il comando */

	... ...

	/*
		se la fork non riesce a creare il processo stampare un messaggio
		di errore e ritornare il controllo al main
	*/

	... ...

	/*
		utilizzare la execvp nel codice del figlio per eseguire il comando.
		Tutti i parametri necessari alla execvp sono contenuti nel vettore
		argv[]. VEDERE man 3 exec!!!!
	*/

	... ...

	/*
		inserire il codice che viene eseguito se la execvp non va a buon fine.
		Usare la funzione syserr o perror per stampare il motivo del fallimento.
	*/

	... ...

	/*
		inserire il codice per il padre. Il padre deve attendere che il figlio
		abbia terminato l'esecuzione del comando prima di ritornare il controllo
		al main.
	*/

}
