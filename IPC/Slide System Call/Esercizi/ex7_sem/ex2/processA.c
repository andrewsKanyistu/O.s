#include <stdio.h> 
#include <stdlib.h>
#include <sys/types.h> 
#include <sys/ipc.h> 
#include <sys/sem.h> 

#define KEY 74

int main(int arcg, char *argv[]) {
	
	int semid;	/* identificatore dell'arrey di semafori */
	
	/* creare lo spazio per fare 2 operazioni atomiche 
	sull'array di semafori.
	Usare la funzione malloc e creare spazio per un vettore di due
	elementi di tipo struct sembuf (dato da passare alla system call semop()) */


	
	/* 
		Creare un array di 3 semafori con permessi di lettura e scrittura 
		per tutti usando la funzione semget().
		Le flag ICP_CREAT e IPC_EXCL fanno si che la funzione semget() 
		ritorni errore se esiste già un vettore di semafori con chiave KEY.
		Vedere man semget.
		
		Il semaforo 0 sincronizza il processo A
		Il semaforo 1 sincronizza il processo B
		Il semaforo 2 sincronizza il processo C				
	*/
	

	/* Inizializzare i semafori dei processi B e C a 1 per bloccarli 
	   Per inizializzare i semafori usare la funzione semop().
		 
		 Topo tale inizializzazione i processi B e C dovranno 
		 eseguire una semop con operazione di tipo sem_op = 0.
		 In tal modo i processi B e C si metteranno in attesa
		 che il proprio semaforo assuma valore 0.
		 Fintantochè il semaforo ha valore 1 i
		 processi B e C rimangono bloccati sulla semop().
		 
		  
		 Si ricorda che la semop() esegue operazioni ATOMICHE sui semafori.
		 Vedere man semop.
		 Alla funzione semop vengono passati 3 argomenti:
		 - l'identificatore dell'array	di semafori su cui eseguire l'operazione
		 - il puntatore alla struttura sembuf necessaria per eseguire le operazioni
		 - il numero di operazioni da eseguire
			
		 Per ogni operazione da eseguire è necessario creare una struttura di tipo
		 semop. La struttura contiene 3 campi:
		 - il numero del semaforo da utilizzare. Ricordare che la semget ritorna
		   array di semafori.
		 - un intero N che rappresenta l'operazione da eseguire.
		 	Se l'intero è N > 0 il valore del semaforo viene incrementato di tale
		 	quantità. Se è N = 0 la semop blocca il codice in attesa che il valore
		 	del semaforo diventi 0. Se è N < 0 la semop blocca il codice in attesa
		 	che il valore del semaforo meno N sia maggiore o uguale a 0.
	*/
	

	/* ciclare per 3 volte */

		/* Acquisizione della risorsa */	
		
		printf("Processo A: sono in attesa di ricevere la risorsa\n");
		
		/* Eseguire due operazioni atomiche sul semaforo di A (semaforo 0)
			 La prima serve per fare in modo che il processo A 
			 attenda che il semaforo 0 diventi 0 prima di accedere alla risorsa.
			 (Usare la funzione semop() con operazione sem_op=0)
			 La seconda imposta subito il semaforo al valore 1 
			 (usare la funzione semop() con operazione sem_op=1)
			 in modo che il processo A possa riottenere la risorsa 
			 solo se il processo C gli da il permesso 
			 (rimettendo il semaforo di A al valore 0). In caso contrario,
			 il processo A deve rimanere bloccato.
			 In altre parole, visto che la chiamata alla semop() è atomica,
			 le 2 operazioni vengono eseguite in modo
			 "non interrompibile". Ciò significa che appena il processo A riesce 
			 ad acquisire la risorsa (perchè il valore del semaforo 0 diventa 0 e la
			 semop() sbocca il processo A) immediatamente il valore del semaforo 0 
			 viene incrementato per garantire la ciclicità nell'utilizzo della
			 risorsa tra A, B e C.
		 */

		printf("Processo A: ho acquisito la risorsa\n");
		sleep(5); /* Supponiamo che il processo A detenga la risorsa per 5 secondi */
		printf("Processo A: sto per rilasciare la risorsa al processo B \n");


		/* rilasciare la risorsa al processo B decrementando
			 il valore del semaforo 1. 
			 In tal modo il processo B, che deve essere in attesa sulla semop() 
			 che il valore del semaforo diventi 0, si sblocca ed acquisisce 
			 la risorsa.
		*/
}
