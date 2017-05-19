/******************************************
MODULO: sig_to_brother.c
SCOPO: il figlio 1 invia un segnale al 
figlio 2 passando per il padre.
******************************************/
#include <stdio.h>
#include <limits.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

/* 
	funzione per la gestione del segnale. 
	Viene chiamata dalla funzione signal.
*/
void hup_handler(int s)
{
	printf ("**SIGNAL**: Ho ricevuto il segnale numero %d",s);
	if (s==SIGUSR1)
		printf(" dal figlio 1, spero!\n");
	else
		printf(" dal figlio 2, spero!\n");
}


int main (int argc, char **argv)
{
	pid_t pid1, pid2;
	
	/* il padre si registra per catturare il segnale SIGUSR1 */
	printf("Sono il padre, mi registro per catturare il segnale SIGUSR1\n");
	
	/*
		la funzione signal registra il processo padre per intercettare il segnale
		SIGUSR1.
		I parametri per la signal sono 2:
		1) valore del segnale da intercettare
		2) funzione per la gestione del segnale che può essere SIG_DFL, SIG_IGN
		oppure una funzione definita dall'utente. In questo codice la funzione è
		definita dall'utente e si chiama hup_handler. 
		Vedere man 2 signal e man 7 signal.
	*/
	signal (SIGUSR1,hup_handler);

	/* creazione primo figlio */
	pid1=fork();
	
	if (pid1 == -1) {
		/* errore */
		perror("fork: figlio 1\n");
		exit(1);
		
	/* codice del figlio */	
	} else if (pid1 == 0) {
		/* 
			il primo figlio invia il segnale SIGUSR1 al padre e termina. 
			il segnale viene inviato usando la system call kill che 
			vuole come parametri il pid del processo a cui inviare il segnale
			e il nome del segnale da inviare. Vedere man 2 kill. 
		*/
		printf("Sono il primo figlio e invio il segnale\n");
		
		/*
			la sleed "da' tempo" al padre di mettersi in pausa con la pause (vedi codice
			del padre).
			N.B l'uso della sleep per ritardare il segnale del padre non garantisce 
			una sincronizzazione corretta. Infatti siamo certi che 2 secondi siano 
			sufficienti???
			Morale: i segnali non vanno usati per sincronizzare processi. 
		*/
		
		sleep(2);
		
		kill(getppid(),SIGUSR1);
		exit(0);
	
	/* codice del padre */
	} else {
		/* creazione secondo figlio */
		pid2=fork();
		
		if (pid2 == -1) {
			/* errore */
			perror("fork: figlio 2\n");
			exit(1);
			
		/* codice del secondo figlio */	
		} else if (pid2 == 0) {
			/* secondo figlio */
			printf("Sono il secondo figlio e mi registro per catturare il segnale\n");
			signal (SIGUSR1,hup_handler);

			/* aspetto il segnale. Vedere man pause */
			pause();
			printf("Sono il secondo figlio e ho gestito il segnale\n");
			exit(0);
			
		/* codice del padre */	
		} else {
			/* padre */
			printf("Sono il padre e ho già creato i due figli\n");

			/* il padre va in pausa e viene svegliato dal segnale del primo figlio. 
			   Esiste però un problema: se la pause() viene eseguita dopo che il segnale del 
				 primo figlio è già arrivato al padre ed è già stato gestito, 
				 il padre entra in pausa infinita.
				 Tale evento non è così improbabile dal momento che la pause viene chimata dopo
				 che il padre ha "perso del tempo" per creare il secondo figlio. Nel frattempo
				 il figlio 1 potrebbe aver già iviato il segnale al padre.
				 Per cercare di fare quadrare i tempi usiamo una sleep dopo la pause in modo
				 da ritardare l'invio del segnale.
			*/

			pause();
			
			
			/* la sleed "da' tempo" al figlio 2 di registrarsi e di mettersi in pausa 
				 N.B l'uso della sleep per ritardare il segnale del padre non garantisce 
				 una sincronizzazione corretta. Infatti siamo certi che 2 secondi siano 
				 sufficienti???
				 Morale: i segnali non vanno usati per sincronizzare processi. 
			*/
			sleep(2);

			/* invia segnale */
			printf("Sono il padre, invio il segnale al figlio numero 2\n");
			kill (pid2,SIGUSR1);

			printf("Sono il padre, attendo la terminazione del figlio numero 2\n");
			wait(NULL);
			printf("Sono il padre, esco.\n");
		}
	}
	exit(0);	
}
