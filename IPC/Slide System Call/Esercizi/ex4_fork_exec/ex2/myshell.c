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

	/* Controllar che alla shell venga passato almeno un argomento */
	
		/* 
			La famiglia di funzion exec sostituisce nel processo corrente
			l'immagine del programma in esecuzione con quella del
			programma che viene passato come primo parametro.
			Usare la funzione execvp in modo che esegua il programma indicato
			in argv[1] passandogli i parametri memorizzati nei successivi
			elementi del vettore argv[].
			Si ricorda che &argv[1] è il puntatore all'elemento 1 del vettore argv[]. 
			Gli elementi di argv[] sono stringhe di caratteri (ovvero char*).
			Se si usa execvp non serve fornire argv[0] perchè quest'ultimo 
			è il nome del programma stesso (ovvero myshell.x) e non di quello 
			che deve essere eseguito, che si trova invece in argv[1].
		*/

		
		/*
			La exec ritorna solo in caso di errore. Quindi la chimata alla syserr
			deve essere fatta solo se si verifica un errore nella exec. In caso 
			contrario il codice sottostante una exec non verrà mai eseguito perchè
			viene sovrascritto dal nuovo programma caricato dalla exec!!!
		*/
	
	/*
		Se non ho fornito parametri nella linea di comando allora non ho
		niente da eseguire e devo segnalarlo all'utente
	*/	

}
