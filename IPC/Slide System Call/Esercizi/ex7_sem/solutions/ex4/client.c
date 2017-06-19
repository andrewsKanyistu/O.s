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

typedef struct record_anag {
	char first_name[20];
	char last_name[20];
	int age;
	char email[40];
} message;

struct sembuf wait_b,signal_b;  /* strutture per le operazioni sui semafori */
int semid;

/* procedura per l'esecuzione di una wait sul semaforo numero
sem_number */
void sem_wait (int sem_number) {
	wait_b.sem_num=sem_number;						/* setto il numero del semaforo */
	if (semop(semid,&wait_b,1)==-1) {	/* eseguo la wait */
		perror("WAIT abortita. TERMINO.\n");
		exit(1);
	}
}

/* procedura per l'esecuzione di una signal sul semaforo numero
sem_number */
void sem_signal (int sem_number) {
	signal_b.sem_num=sem_number;						/* setto il numero del semaforo */
	if (semop(semid,&signal_b,1)==-1) {	/* eseguo la signal */
		perror("SIGNAL abortita. TERMINO.\n");
		exit(1);
	}
}

int main(int argc, char *argv[]) {

	message *m;
	int shmid;
	char c = 'S';
	message *buf;


	wait_b.sem_num=signal_b.sem_num=0;	/* inizializzazione struttura operazioni */
	wait_b.sem_op=-1;										/* la 'wait' sui semafori richiede sem_op < 0 */
	signal_b.sem_op=1;									/* la 'signal' sui semafori richiede sem_op > 0 */


	m = (message *) malloc(sizeof(message));


	/* recupero l'identificatore del semaforo */
	if ((semid=semget(SEMKEY,3,0666)) == -1) {
		perror("semget");
		exit(1);
  }

	/*recupero l'identificatore della memoria condivisa*/
	if ((shmid = shmget(SHMKEY,1,0666)) == -1)	{
		perror("shmget");
		exit(1);
	}

	/* montaggio dell'area condivisa nell'area dati del processo
	a partire dal primo indirizzo disponibile */
	if ((buf=(message *)shmat(shmid,0,0666))==(message *)-1) {
		perror("shmat");
		exit(1);
	}


	while(c == 'S' || c == 's') {
		printf("Digitare il nome: ");
		scanf("%s", &(m->first_name));
		printf("Digitare il cognome: ");
		scanf("%s", &(m->last_name));
		printf("Digitare l'età: ");
		scanf("%d", &(m->age));
		printf("Digitare l'indirizzo email: ");
		scanf("%s", &(m->email));

		/* scrittura del record sulla memoria condivisa */
		sem_wait(EMPTY);
		sem_wait(MUTEX);
		*buf = *m;
		sem_signal(MUTEX);
		sem_signal(FULL);

		do {
			c = getchar();
			printf("Altro nominativo? [S/N] ");
			c = getchar();
		} while (c != 's' && c != 'n' && c != 'S' && c != 'N');

	}

	/* smontaggio dell'area comune dall'area dati del padre */
	shmdt(buf);
}
