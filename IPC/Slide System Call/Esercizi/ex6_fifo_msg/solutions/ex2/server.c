#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "def.h"

#define MSGKEY   90	/* chiave per la coda */
#define MSGTYPE1  1	/* type per i messaggi del client dei CLIENTI */
#define MSGTYPE2  2	/* type per i messaggi del client dei FORNITORI */

int msgid;

/*
	funzione che cattura il segnale inviato dal timer
	per il processo padre che riceve i messaggi dai
	fornitori
*/
void exit_fnc_F() {
	printf("Tempo di attesa scaduto per i fornitori. Bye!\n");

	/* attende eventuale terminazione del figlio */
	wait(NULL);

	/* rimozione della coda */
	msgctl(msgid, IPC_RMID, NULL);

	exit(0);
}

/*
	funzione che cattura il segnale inviato dal timer
	per il processo figlio che riceve i messaggi dai
	clienti
*/
void exit_fnc_C() {
	printf("Tempo di attesa scaduto per i clienti. Bye!\n");
	exit(0);
}


int main(int argc, char *argv[]) {

	FILE *f_clienti;		/* file per i messaggi dei clienti */
	FILE *f_fornitori;	/* file per i messaggi dei fornitori */

	message *m;


	if (argc != 3) {
		printf("\nUsage: server.x <file_clienti> <file_fornitori>\n\n");
		exit(1);
	}


	m = (message *) malloc(sizeof(message));

	/* Creazione  della coda di messaggi */
	if((msgid = msgget(MSGKEY, (0666|IPC_CREAT|IPC_EXCL))) == -1) {
		perror("Creazione della coda di messaggi fallita");
		exit(1);
	}


	/*
		Il processo si sdoppia per gestire separatamente i messaggi
		del tipo 1 (clienti) dai messaggi del tipo 2 (fornitori).
		Il padre legge i messaggi dei fornitori, mentre il figlio
		legge i messaggi dei clienti.
	*/
	switch(fork()) {

	case -1:							/* errore */
		perror("fork");
		exit(1);

	case 0:								/* codice del figlio */

		/*
			apertura del file per la memorizzazione dei
			messaggi dei clienti
		*/
		if ((f_clienti = fopen(argv[1], "w")) == NULL) {
			perror("server.c: Cannot open file.");
			exit(1);
		}


		/*
			il figlio si registra per catturare il segnale
			SIGALRM per scadenza del timer
		*/
		signal(SIGALRM, exit_fnc_C);

		/*
			Il figio legge dalla coda i messaggi di tipo MSGTYPE1 (clienti)
		*/
		while(1) {
			/*
				dopo 30 secondi di inattività scatta l'allarme per il figlio
				che termina la sua esecuzione
			*/
			alarm(30);


			/* lettura dei messaggi */
			if(msgrcv(msgid, m, sizeof(message) - sizeof(m->mtype), MSGTYPE1, 0) == -1) {
				perror("msgrcv");
				exit(1);
			}

			/* l'allarme viene disabilitato dopo la ricezione di ogni messaggio */
			alarm(0);

			/* scrittura del messaggio sul file */
			fprintf(f_clienti, "NUOVO CLIENTE\n");
			fprintf(f_clienti, "mtype = %d\n", m->mtype);
			fprintf(f_clienti, "first_name = %s\n", m->first_name);
			fprintf(f_clienti, "last_name = %s\n", m->last_name);
			fprintf(f_clienti, "age = %d\n", m->age);
			fprintf(f_clienti, "email = %s\n", m->email);
			fprintf(f_clienti, "\n\n *** ***\n\n");

			/*
				viene forzata la scrittura del file dopo la ricezione
				di ogni messaggio.
				vedere man fflush.
			*/
			fflush(f_clienti);
		}
	}

	/* codice del padre */

	/*
		apertura del file per la memorizzazione dei
		messaggi dei fornitori
	*/
	if ((f_fornitori = fopen(argv[2], "w")) == NULL) {
		perror("server.c: Cannot open file.");
		exit(1);
	}

	/*
		il padre si registra per catturare il segnale
		SIGALRM per scadenza del timer
	*/
	signal(SIGALRM, exit_fnc_F);

		/*
			Il figio legge dalla coda i messaggi di tipo MSGTYPE2 (fornitori)
		*/
	while(1) {

		/*
			dopo 30 secondi di inattività scatta l'allarme per il padre
			che termina la sua esecuzione
		*/
		alarm(30);

		/* lettura dei messaggi */
		if(msgrcv(msgid, m, sizeof(message) - sizeof(m->mtype), MSGTYPE2, 0) == -1) {
			perror("msgrcv");
			exit(1);
		}

		/* l'allarme viene disabilitato dopo la ricezione di ogni messaggio */
		alarm(0);

		/* scrittura del messaggio sul file */
		fprintf(f_fornitori, "NUOVO FORNITORE\n");
		fprintf(f_fornitori, "mtype = %d\n", m->mtype);
		fprintf(f_fornitori, "first_name = %s\n", m->first_name);
		fprintf(f_fornitori, "last_name = %s\n", m->last_name);
		fprintf(f_fornitori, "age = %d\n", m->age);
		fprintf(f_fornitori, "email = %s\n", m->email);
		fprintf(f_fornitori, "\n\n *** ***\n\n");

		/*
			viene forzata la scrittura del file dopo la ricezione
			di ogni messaggio.
			vedere man fflush.
		*/
		fflush(f_fornitori);
	}

}
