#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#define SEMKEY   90
#define SHMKEY   91	

#define EMPTY 0 /* definizioni numeri dei semafori per la gestione del buffer */
#define FULL 1  
#define MUTEX 2

/* struttura per l'inserimento dei dati */
typedef struct record_anag {
	char first_name[20];
	char last_name[20];
	int age;
	char email[40];			
} message;
			 
struct sembuf wait_b,signal_b;  /* strutture per le operazioni sui semafori */			 
int semid; /* id per i semafori */
int shmid; /* id per la memoria condivisa */		 
			 
void sem_wait (int sem_number) {

/* inserire la procedura per l'esecuzione di una wait 
sul semaforo numero sem_number come fatto nell'esercizio ex2 */


}

void sem_signal (int sem_number) {

/* inserire la procedura per l'esecuzione di una signal sul 
semaforo numero sem_number come fatto per l'esercizio ex2 */


}			 		 
			 
int main(int argc, char *argv[]) {

	message *m;
	char c = 'S';	
	message *buf;
	
	
	wait_b.sem_num=signal_b.sem_num=0;	/* inizializzazione struttura operazioni */
	wait_b.sem_op=-1;										/* la 'wait' sui semafori richiede sem_op < 0 */
	signal_b.sem_op=1;									/* la 'signal' sui semafori richiede sem_op > 0 */


	/* creare spazio in memoria per la struttura message */

	/* recuperare l'identificatore del vettore di semafori creato dal server */

	/*recupero l'identificatore della memoria condivisa creata dal server */

	/* montare l'area condivisa nell'area dati del processo
	a partire dal primo indirizzo disponibile */

	/* Ripetere ciclicamente finche' l'utente vuole inserire un
	   nuovo record dati rispondendo 's' o 'S' alla domanda "Altro nominativo?"*/
	while(c == 'S' || c == 's') {	

		/* Leggere da tastiera i dati da inserire nella struttura di tipo message */
	
		/* scrivere il messaggio nella memoria condivisa.
			 Proteggere la scrittura tramite l'uso dei semafori EMPTY, FULL e mutex
		   come nel caso del problema del produttore-consumatore 
		*/

			
	do {
			c = getchar();	
			printf("Altro nominativo? [S/N] ");		
			c = getchar();
		} while (c != 's' && c != 'n' && c != 'S' && c != 'N');
		
	}
	
	/* smontare l'area comune dall'area dati del padre */
		
}
