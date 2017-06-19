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

typedef struct record_anag {
	char first_name[20];
	char last_name[20];
	int age;
	char email[40];			
} message;
			 
struct sembuf wait_b,signal_b;  /* strutture per le operazioni sui semafori */			 
int semid;			
int shmid; 
			 
/*
	procedura per l'esecuzione di una wait sul semaforo numero
	sem_number
*/
void sem_wait (int sem_number) {
	wait_b.sem_num=sem_number;						/* setto il numero del semaforo */
	if (semop(semid,&wait_b,1)==-1) {	/* eseguo la wait */
		perror("WAIT abortita. TERMINO.\n");
		exit(1);
	}
}

/* 
	procedura per l'esecuzione di una signal sul semaforo numero
	sem_number 
*/
void sem_signal (int sem_number) {
	signal_b.sem_num=sem_number;						/* setto il numero del semaforo */
	if (semop(semid,&signal_b,1)==-1) {	/* eseguo la signal */
		perror("SIGNAL abortita. TERMINO.\n");
		exit(1);
	}
}	

/* funzione che cattura il segnale inviato dal timer */
void exit_fnc() {
	semctl(semid, 0, IPC_RMID, 0);
	shmctl(shmid, IPC_RMID, NULL);
	printf("Tempo di attesa scaduto. Bye!\n");
	exit(1);	
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

	
	/* creo il semaforo */
	if ((semid=semget(SEMKEY,3,0666 | IPC_CREAT | IPC_EXCL)) == -1) {
		perror("semget");
		exit(1);					
  }

	sem_stato=semctl(semid,EMPTY,SETVAL,st_sem);
	if (sem_stato==-1) {
		perror ("Semaforo EMPTY non inizializzato");
		semctl(semid, 0, IPC_RMID, 0);
		exit(1);
	}

	st_sem.val=1;													/* assegnazione valore iniziale dei semafori */		
	sem_stato=semctl(semid,MUTEX,SETVAL,st_sem);
	if (sem_stato==-1) {
		perror ("Semaforo MUTEX non inizializzato");
		exit(1);
	}

	st_sem.val=0;
	sem_stato=semctl(semid,FULL,SETVAL,st_sem);
	if (sem_stato==-1) {
		perror ("Semaforo FULL non inizializzato");
		exit(1);
	}

	/*creo la memoria condivisa*/
	if ((shmid = shmget(SHMKEY,1,0666 | IPC_CREAT | IPC_EXCL)) == -1)	{
		perror("shmget");
		exit(1);
	}

	/* montaggio dell'area condivisa nell'area dati del processo
	a partire dal primo indirizzo disponibile */
	if ((buf=(message *)shmat(shmid,0,0666))==(message *)-1) {
		perror("shmat");
		exit(1);
	}

	signal(SIGALRM, exit_fnc);

	/*Si mette in attesa per leggere dalla memoria condivisa */
	while(1) {
		alarm(30);
		
		/* lettura dalla memoria condivisa */
		sem_wait(FULL);
		sem_wait(MUTEX);
		*m = *buf;
		sem_signal(MUTEX);
		sem_signal(EMPTY);	
		
		alarm(0);
		printf("\nfirst_name = %s\n", m->first_name);		
		printf("last_name = %s\n", m->last_name);						
		printf("age = %d\n", m->age);		
		printf("email = %s\n", m->email);
		printf("\n\n *** ***\n\n");
	}

}

