#include <stdio.h> 
#include <stdlib.h> 
#include <sys/types.h> 
#include <sys/ipc.h> 
#include <sys/sem.h> 

#define KEY 74

int main(int arcg, char *argv[]) {
	
	int semid;
	struct sembuf * sops = (struct sembuf *) malloc (2*sizeof(struct sembuf));
	int i;
	
	/* Recupero il semaforo creato dal processo A */
	if((semid = semget(KEY, 3, 0666)) == -1) {
		perror("semget");
		exit(1);
	}

	for(i=0; i<3; i++) {

		/*Acquisisco la risorsa*/	
		
		printf("Sono in attesa di ricevere la risorsa\n");		
		
		sops[0].sem_num = 1; /*semaforo 1*/
		sops[0].sem_op = 0;	/*attende che il semaforo valga zero*/
		sops[0].sem_flg = 0;

		sops[1].sem_num = 1; /*semaforo 1*/
		sops[1].sem_op = 1;	/*acquisisce la risorsa*/
		sops[1].sem_flg = 0;

		/*la chiamata è atomica !!!*/
		semop(semid, sops, 2);

		printf("Processo B: ho acquisito la risorsa\n");
		sleep(5);
		printf("Processo B: sto per rilasciare la risorsa al processo C\n");

		sops[0].sem_num = 2;  /* semaforo 2 */
		sops[0].sem_op = -1;	/* rilascia la risorsa al processo C */
		sops[0].sem_flg = 0;

		semop(semid, sops, 1);
	}
}
