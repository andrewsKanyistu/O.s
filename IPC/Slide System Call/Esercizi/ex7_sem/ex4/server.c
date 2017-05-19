#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <signal.h>


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


void exit_fnc() {
  /* definire la funzione che cattura il segnale inviato dal timer.
	La funzione deve rimuovere coda di messaggi e semafori
	e far terminare il programma. */
}
			 
int main(int argc, char *argv[]) {

	message *m;
	message *buf;
	int sem_stato;          /* stato del semaforo */
	union semun {           /* union per la gestione del controllo sui semafori */
				int val;
				struct semid_ds *buf;
				ushort *array;
	} st_sem;
	
	m = (message *) malloc(sizeof(message));
	
	wait_b.sem_num=signal_b.sem_num=0;	/* inizializzazione struttura operazioni */
	wait_b.sem_op=-1;										/* la 'wait' sui semafori richiede sem_op < 0 */
	signal_b.sem_op=1;									/* la 'signal' sui semafori richiede sem_op > 0 */

	wait_b.sem_flg=signal_b.sem_flg=SEM_UNDO; /* flag di undo */
	st_sem.val=1;															/* assegnazione valore iniziale dei semafori */

	
	/* creare 3 semafori (EMPTY, FULL E MUTEX) con la semget() */


	/* assegnare i valori iniziali dei semafori 
		 MUTEX = 1
		 FULL = 0
	*/		


	/*creare la memoria condivisa usando la shmget() */

	/* montare l'area di memoria condivisa nell'area dati del processo
	a partire dal primo indirizzo disponibile tramite la shmat() */

	/* registrare il server per la esecuzione della funzione exit_fnc()
	   al momento della ricezione del segnale SIGALRM.
		 Usare la funzione signal() */


	/*Leggere ciclicamente dalla memoria condivisa il messaggio inserito
	  dal client */
	while(1) {
		/* far partire l'allarme con la alarm() */
		
		/* leggere dalla memoria condivisa proteggendo 
		   la lettura con i semafori FULL, MUTEX e EMPTY come
			 nel caso del produttore-consumatore */

		/* disarmare l'allarme */		
		
		/* stampare a video il messaggio letto nella memoria condivisa */
	}

}

