#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include "def.h"

#define MSGKEY   90		/* chiave per la coda */
#define MSGTYPE  1		/* tipo per il messaggio da ricevere */

int msgid;		/* identificatore della coda */

/*
	funzione che cattura il segnale inviato dal timer
	il timer scatta se per 30 secondi non arrivano
	nuovi messaggi dal processo client
*/
void exit_fnc() {

	/* la coda viene rimossa. Vedere man msgctl */
	msgctl(msgid, IPC_RMID, NULL);
	printf("Tempo di attesa scaduto. Bye!\n");
	exit(1);
}


int main(int argc, char *argv[]) {

	/*
		puntatore alla struttura che rappresenta il messaggio.
		Il messaggio è definito nel file def.h
	*/
	message *m;

	/* allocazione memoria per il messaggio */
	m = (message *) malloc(sizeof(message));

	/*
		Creazione della coda di messaggi.
		La flag 0666|IPC_CREAT|IPC_EXCL fa sì che la
		coda venga creata, con permessi di lettura e
		scrittura per tutti, solo se non esiste già
		un'altra coda associata alla chiave MSGKEY.
		Vedere man msgget
	*/
	if((msgid = msgget(MSGKEY, (0666|IPC_CREAT|IPC_EXCL))) == -1) {
		perror("Creazione della coda di messaggi fallita");
		exit(1);
	}

	/*
		il processo si registra per catturare il segnale
		SIGALRM inviato dalla alarm
	*/
	signal(SIGALRM, exit_fnc);


	/*
		Il processo si mette in attesa per leggere dalla coda
	*/
	while(1) {

		/* viene impostato l'allarme che scatta dopo 30 secondi */
		alarm(30);

		/*
			la system call msgrcv riceve i messaggi dalla coda msgid e li
			memorizza nel buffer m.
			Il terzo parametro della funzione rappresenta la dimensione
			del messaggio ad esclusione dello spazio necessario a contenere
			il campo mtype della struttura.
			Il quarto parametro indica che devono essere letti solo i messaggi
			con message type MSGTYPE.
			L'ultimo parametro indica che il processo deve rimanere in attesa
			se non ci sono messaggi nella coda.
			Vedere man msgrcv.
		*/
		if(msgrcv(msgid, m, sizeof(message) - sizeof(m->mtype), MSGTYPE, 0) == -1) {
			perror("msgrcv");
			exit(1);
		}

		/*
			ogni volta che viene letto un messaggio l'allarme deve
			essere disattivato per evitare che scatti impropriamente
		*/
		alarm(0);

		/* stampa a video del contenuto del messaggio */
		printf("mtype = %ld\n", m->mtype);
		printf("first_name = %s\n", m->first_name);
		printf("last_name = %s\n", m->last_name);
		printf("age = %d\n", m->age);
		printf("email = %s\n", m->email);
		printf("\n\n *** ***\n\n");
	}

}

