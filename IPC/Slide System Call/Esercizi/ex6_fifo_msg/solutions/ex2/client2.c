#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "def.h"

#define MSGKEY   90
#define MSGTYPE  2

int main(int argc, char *argv[]) {

	message *m;
	int msgid;
	char c = 'S';

	m = (message *) malloc(sizeof(message));


	if((msgid = msgget(MSGKEY, 0666)) == -1) {
		perror("Apertura della coda di messaggi fallita");
		exit(1);
	}


	while(c == 'S' || c == 's') {
		printf("NUOVO FORNITORE\n");
		m->mtype = MSGTYPE;
		printf("Digitare il nome: ");
		scanf("%s", (m->first_name));
		printf("Digitare il cognome: ");
		scanf("%s", (m->last_name));
		printf("Digitare l'età: ");
		scanf("%d", &(m->age));
		printf("Digitare l'indirizzo email: ");
		scanf("%s", (m->email));

		if(msgsnd(msgid, m, sizeof(message) - sizeof(m->mtype), 0) == -1) {
			perror("msgsnd");
			exit(1);
		};

		do {
			c = getchar();
			printf("Altro nominativo? [S/N] ");
			c = getchar();
		} while (c != 's' && c != 'n' && c != 'S' && c != 'N');

	}
}
