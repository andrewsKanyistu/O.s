/*
*	SCOPO: Utilizzo dei threads
*
*	Il programma permette di creare un file per comparare liste di numeri
*	Input: 	due o più file che contengono le liste di numeri.
*					La versione attuale gestisce file con 10000 elementi.
*	Output:	file contenente una tabella indicante la lista di numeri da
*					0 a 10000 e una X in corrispondenza di ogni numero presente nel
*					relativo file di input.
*/


#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


/*struttura che contiene gli argomenti per le threads*/
typedef struct {
	int t;
	char *filename;
	char* list;
} thread_arg;

thread_arg *arg;


/*start routine per le threads*/
void *sort(void *arg)
{
	FILE *fd;
	char s[6];
	int i;
	
	printf("Gestisco il file: %s\n", ((thread_arg *)arg)->filename);
	fd = fopen(((thread_arg *)arg)->filename, "r");
	
	/*Inizializzazione dell'array per la memorizzazione dei numeri
	presenti nel file*/
	for(i=0; i < 10000; i++)
			((thread_arg *)arg)->list[i] = ' ';
	
	/*Identificazione dei numeri presenti nel file*/
	for (i=0; i < 10000; i++) {
		fgets(s, 6, fd);
		if(s!=NULL)
			/*printf("%s\n",s);*/
			((thread_arg *)arg)->list[atoi(s)] = 'X';
	}
	
	printf("Thread %d: ho terminato\n", ((thread_arg *)arg)->t);	
	pthread_exit((void *) 0);
}


/*main thread*/
int main(int argc, char *argv[])
{
	pthread_t *thread;				/*puntatore alle threads*/
	pthread_attr_t attr;			/*attibuto per le thread*/
	int rc, t, status, i, j, f;	
	char **lists;							/*array di liste per memorizzare i numeri*/
	FILE *fout;								/*file di output*/

	if(argc < 3) { 
		printf("Inserire almento 2 file come parametri.\n");
		exit(-1);
	}

	fout = fopen("./out.txt", "w");

	/*allocazione dello spazio per le thread*/
	thread = (pthread_t *) malloc ((argc-1)*sizeof(pthread_t));
	
	/*allocazione dello spazio per le liste*/
	lists = (char **) malloc((argc-1)*sizeof(char *));
	for(i=0; i<argc; i++)
		lists[i] = (char *) malloc(10000*sizeof(char));

	/*allocazione dello spazio per l'argomento delle threads*/	
	arg = (thread_arg *) malloc((argc-1)*sizeof(thread_arg));

	/* Attributo Joinable per le threads */
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	/*creazione delle threads*/
	for(t=0; t < argc-1; t++)
	{
		arg[t].filename = argv[t+1];
		arg[t].list = lists[t];
		arg[t].t = t;
		printf("Creazione del thread n° %d\n", t);
		rc = pthread_create(&thread[t], &attr, sort, &arg[t]); 
		if (rc)
		{
			printf("ERRORE; codice di errore della pthread_create(): %d\n", rc);
			exit(-1);
		}
	}

	/* Distruzione dell'attributo */
	pthread_attr_destroy(&attr);
	
	/*attesa della terminazione delle threads*/
	for(t = 0; t < argc-1; t++)
	{
		rc = pthread_join(thread[t], (void **)&status);
		if (rc)
		{
			printf("ERROR return code from pthread_join() is %d\n", rc);
			exit(-1);
		}
		printf("Join completato con il thread %d, status = %d\n",t, status);
	}
	
	/*scrittura del risultato su file*/
	for(j = 0; j < 10000; j++) {
		fprintf(fout, "%5d:  ", j);
		
		for(f=0; f<argc-1; f++) {
			fprintf(fout, "%c    ", lists[f][j]);
		}
		
		fprintf(fout, "\n" );
	}	
	pthread_exit(NULL);
}
