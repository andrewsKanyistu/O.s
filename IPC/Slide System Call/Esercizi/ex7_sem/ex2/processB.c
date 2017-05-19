#include <stdio.h> 
#include <stdlib.h>
#include <sys/types.h> 
#include <sys/ipc.h> 
#include <sys/sem.h> 

#define KEY 74

int main(int arcg, char *argv[]) {
	
	
	/* Recuperare il semaforo creato dal processo A tramite la semget() */


	for(i=0; i<3; i++) {

		/*Acquisire la risorsa agendo opportunamente sui semafori
		  come fatto per il processo A
		*/	
		
		printf("Sono in attesa di ricevere la risorsa\n");		
		
		/* Mettersi in attesa che il semaforo 1 diventi 0
		   quindi impostare atomicamente il semaforo 1 al valore 1
			 (per rispettare la ciclicita' nell'uso della risorsa) 
			 e proseguire.
		*/


		printf("Processo B: ho acquisito la risorsa\n");
		sleep(5);		/* risorsa detenuta 5 secondi */
		printf("Processo B: sto per rilasciare la risorsa al processo C\n");

		/* rilasciare la risorsa sbloccando il processo C. */
		
	}
}
