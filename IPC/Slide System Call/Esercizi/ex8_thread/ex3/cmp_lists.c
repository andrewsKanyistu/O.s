/*
*	SCOPO: Utilizzo dei threads
*
*	Il programma permette di creare un file per comparare liste di numeri
*	Input: 	due o più file che contengono le liste di numeri.
*					Creare una thread per ogni file fornito in input
*	Output:	file contenente una tabella indicante la lista di numeri da
*					0 a 10000 e una X in corrispondenza di ogni numero presente nel
*					relativo file di input.
*/


#include <pthread.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


/* dichiarare una struttura dati che contenga gli argomenti per le threads*/
typedef struct {
	int t;
	char *filename;
	char *list;	/*puntatore alla lista di numeri*/
} thread_arg;

thread_arg *arg;

/*start routine per le threads*/
void *sort(void *arg) {

	
	/* l'argomento arg e' di tipo thread_arg */

	/* aprire il file ((thread_arg *)arg)->filename */
	
	/* Inizializzare gli elementi dell'array (thread_arg *)arg)->list 
	per la memorizzazione dei numeri presenti nel file col carattere ' ' */
	
	/* Identificare i numeri presenti nel file
	   e inserire una X per ogni numero presente nell'elemento 
		 corrispondente del vettore (thread_arg *)arg)->list
		 */
}


/*main thread*/
int main(int argc, char *argv[])
{
	pthread_t *thread;				/*puntatore alle threads*/
	pthread_attr_t attr;			/*attibuto per le thread*/
	char **lists;							/*array di liste per memorizzare i numeri*/
	FILE *fout;								/*file di output*/

	if(argc < 3) { 
		printf("Inserire almento 2 file come parametri.\n");
		exit(-1);
	}

	fout = fopen("./out.txt", "w");

	/*allocare spazio per tante thread quanti sono i file in ingresso 
		usare la funzione malloc. Lo spazio ritornato deve essere puntato
		dal puntatore thread
	*/
	
	/*allocare spazio per le liste di numeri 
		usare la funzione malloc. Lo spazio ritornato deve essere puntato
		dal puntatore lists 
		Attenzione: lists e' un puntatore a puntatore di caratteri. Bisogna
		fare una doppia allocazione (riga, colonna).
		Se non si capisce il significato di quanto scritto sopra chiedere
		al docente o agli studenti tutori.	
	*/

	/*allocare spazio per l'argomento della startup routine delle threads*/	

	/* definire attributo Joinable per le threads */


	/*creare le threads e fornire ad ognuna un argomento di tipo
	  thread_arg 
	*/
	 
	/* Distruggere l'attributo */
	
	/* attendere la terminazione delle threads tramite pthread_join() */
	
	/*scrivere il risultato su file*/
}
