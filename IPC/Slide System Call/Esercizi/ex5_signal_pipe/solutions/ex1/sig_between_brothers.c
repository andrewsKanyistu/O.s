/******************************************
MODULO: sig_to_brother.c
SCOPO: il figlio 1 invia un segnale al figlio 2
il quale, alla ricezione del segnale del fratello,
gli risponde con un altro segnale. Il passaggio
dei segnali deve essere effettuato passando
tramite il padre.
******************************************/
#include <stdio.h>
#include <limits.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
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

	/* il padre si registra per catturare i segnali SIGUSR1 e SIGUSR2*/
	printf("PADRE     : mi registro per catturare i segnali dei miei figli.\n");

	/*
		le 2 chiamate alla funzione signal registrano il processo padre per
		intercettare i segnali SIGUSR1 e SIGUSR2.
		I parametri per la signal sono 2:
		1) valore del segnale da intercettare
		2) funzione per la gestione del segnale che può essere SIG_DFL, SIG_IGN
		oppure una funzione definita dall'utente. In questo codice la funzione è
		definita dall'utente e si chiama hup_handler ed è definita sopra il main.
		Vedere man 2 signal e man 7 signal.
	*/
	signal (SIGUSR1,hup_handler);
	signal (SIGUSR2,hup_handler);


	/* creazione primo figlio */
	pid1=fork();

	if (pid1 == -1) {
		/* errore */
		perror("fork: figlio 1\n");
		exit(1);

	/* codice del figlio */
	} else if (pid1 == 0) {

		/*
			Il figlio 1 si registra per ricevere la risposta del fratello
		*/
		printf("FIGLIO 1  : mi registro per ricevere la risposta di mio fratello.\n");
		/* signal(SIGUSR2, hup_handler);			 */

		/*
			il primo figlio invia il segnale SIGUSR1 al padre e attende il segnale del fratello.
			il segnale viene inviato usando la system call kill che
			vuole come parametri il pid del processo a cui inviare il segnale
			e il nome del segnale da inviare. Vedere man 2 kill.
			Prima di iniviare il segnale, il figlio 1 viene addormentato per 5 secondi
			per dare tempo al padre di creare anche l'altro figlio e di mettersi in attesa
			del segnale. Vedere man 3 sleep.
		*/

		sleep(5);
		printf("FIGLIO 1  : invio il segnale SIGUSR1 a mio fratello passando per mio padre.\n");
		kill(getppid(),SIGUSR1);

		/*
			la pause mette in attesa il figlio per la ricezione di un segnale.
			Vedere man pause.
		*/
		printf("FIGLIO 1  : attendo la risposta da mio fratello.\n");
		pause();

		printf("FIGLIO 1  : sono stato svegliato dal segnale di ritorno di mio fratello, bye.\n");

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
			printf("FIGLIO 2  : mi registro per catturare il segnale di mio fratello.\n");
			/* signal (SIGUSR1,hup_handler); */

			/* aspetto il segnale. Vedere man pause */
			pause();

			/* dormo per 5 secondi prima di inviare la risposta */
			sleep(5);

			printf("FIGLIO 2  : ho gestito il segnale, ora rispondo a mio fratello passando per il padre, bye.\n");
			kill(getppid(), SIGUSR2);
			exit(0);

		/* codice del padre */
		} else {
			/* padre */
			printf("PADRE     : ho già creato i due figli. Aspetto che si parlino. \n");

			/* il padre va in pausa e viene svegliato dal segnale del primo figlio.
			   Esiste però un problema: se la pause() viene eseguita dopo che il segnale del
				 primo figlio è già arrivato al padre ed è già stato gestito,
				 il padre entra in pausa infinita.
				 Tale evento non è così improbabile dal momento che la pause viene chimata dopo
				 che il padre ha "perso del tempo" per creare il secondo figlio. Nel frattempo
				 il figlio 1 potrebbe aver già iviato il segnale al padre.
				 Morale: i segnali non vanno usati per sincronizzare processi. */

			pause();


			/* da' tempo al figlio 2 di registrarsi e di mettersi in pausa */
			sleep(5);

			/* invia segnale */
			printf("PADRE     : ho ricevuto il segnale dal figlio 1, lo invio al figlio 2 e aspetto la sua risposta.\n");
			kill (pid2,SIGUSR1);

			pause();

			printf("PADRE     : ho ricevuto la risposta del figlio 2, invio il segnale al figlio 1.\n");
			kill (pid1, SIGUSR2);

			printf("PADRE     : attendo la terminazione dei figli\n");
			wait(NULL);
			wait(NULL);
			printf("PADRE     : bye.\n");
		}
	}
	exit(0);
}
