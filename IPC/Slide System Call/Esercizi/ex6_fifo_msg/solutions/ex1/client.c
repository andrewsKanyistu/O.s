#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include "def.h"

#define MSGKEY   90		/* chiave per la coda */
#define MSGTYPE  1		/* tipo per il messaggio da inviare */


int main(int argc, char *argv[]) {

	/*
		puntatore alla struttura che rappresenta il messaggio.
		Il messaggio è definito nel file def.h
	*/
	message *m;

	int msgid;		/* identificatore della coda */

	char c = 'S';

	/* allocazione memoria per il messaggio */
	m = (message *) malloc(sizeof(message));

	/*
		apertura  della coda di messaggi corrispondente
		alla chiave MSGKEY
	*/
	if((msgid = msgget(MSGKEY, 0666)) == -1) {
		perror("Apertura della coda di messaggi fallita.\n");
		exit(1);
	}

	/*
		il processo cicla finchè l'utente vuole
		inviare nuovi messaggi al server
	*/
	while(c == 'S' || c == 's') {

		/* creazione del messaggio da inviare */
		m->mtype = MSGTYPE;

		printf("Digitare il nome: ");
		scanf("%s", (m->first_name));

		printf("Digitare il cognome: ");
		scanf("%s", (m->last_name));

		printf("Digitare l'età: ");
		scanf("%d", &(m->age));

		printf("Digitare l'indirizzo email: ");
		scanf("%s", (m->email));

		/*
			la system call msgsnd spedisce il messaggio
			memorizzato nel buffer m verso la coda msgid.
			Il terzo parametro della funzione rappresenta la dimensione
			del messaggio ad esclusione dello spazio necessario a contenere
			il campo mtype della struttura.
			L'ultimo parametro indica che il processo deve rimanere in attesa
			se la coda è piena finche' si libera spazio per inserire il
			messaggio.
			Vedere man msgsnd.
		*/
		if(msgsnd(msgid, m, sizeof(message) - sizeof(m->mtype), 0) == -1) {
			perror("msgsnd");
			exit(1);
		};

		/*
			all'utente viene chiesto se vuole inviare
			un nuovo messaggio
		*/
		do {
			c = getchar();
			printf("Altro nominativo? [S/N] ");
			c = getchar();
		} while (c != 's' && c != 'n' && c != 'S' && c != 'N');

	}

	return 0;
}
