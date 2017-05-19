/******************************************
MODULO: myshell.c
SCOPO: Mini-shell che esegue un comando
******************************************/
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int main (int argc, char *argv[])
{

	/* Alla shell devo passare almeno un argomento */
	if (argc > 1) {
	
		/* 
			La famiglia di funzion exec sostituisce nel processo corrente
			l'immagine del programma in esecuzione con quella del
			programma che viene passato come primo parametro.
			In questo caso la funzione execvp eseguirà il programma indicato
			in argv[1] passandogli i parametri memorizzati nei successivi
			elementi del vettore argv[].
			&argv[1] è il puntatore all'elemento 1 del vettore argv[]. 
			Gli elementi di argv[] sono stringhe di caratteri (ovvero char*).
			Ad execvp non viene fornito argv[0] perchè quest'ultimo è il nome
			del programma stesso (ovvero myshell.x) e non di quello che deve essere
			eseguito che si trova in argv[1].
		*/
		execvp(argv[1],&argv[1]);
		
		/*
			La exec ritorna solo in caso di errore. Quindi la chimata alla perror
			viene effettuata solo se si verifica un errore nella exec. In caso 
			contrario il codice sottostante una exec non verrà mai eseguito perchè
			viene sovrascritto dal nuovo programma caricato dalla exec!!!
		*/
		perror("execvp() fails!!!");
	
	/*
		Se non ho fornito parametri nella linea di comando allora non ho
		niente da eseguire.
	*/	
	} else {
		printf("Nothing to do!\n");
	}	
}
