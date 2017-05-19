#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <signal.h>
#include "def.h"

#define MSGKEY   90		/* chiave per la coda */
#define MSGTYPE  1		/* tipo per il messaggio da ricevere */


/* dichiarare l'identificatore della coda */

/* 
	dichiarare e definire la funzione che cattura il segnale 
	inviato dal timer.
	Il timer scatta se per 30 secondi non arrivano
	nuovi messaggi dal processo client.
	La funzione deve solo rimuovere la coda di messaggi tramite
	la system call msgctl alla quale bisogna passare come comando
	IPC_RMID.
	Vedere man msgctl
*/

			 
			 
int main(int argc, char *argv[]) {

	/*
		dichiarare un puntatore alla struttura che rappresenta il messaggio.
		Il messaggio va definito nel file def.h
	*/

	
	/* allocare memoria per il messaggio tramite la malloc*/
	
	/* 
		Creare la coda di messaggi tramite la funzione msgget.
		Usare la flag 0666|IPC_CREAT|IPC_EXCL in modo che la 
		coda venga creata, con permessi di lettura e 
		scrittura per tutti, solo se non esiste già
		un'altra coda associata alla chiave MSGKEY.
		Vedere man msgget
	*/

	/*
		registrare il processo per catturare il segnale
		SIGALRM inviato dalla alarm.
		Usare la funzione signal. Vedere man 2 signal e man 7 signal
	*/


	
		/* impostare l'allarme in modo che scatti dopo 30 secondi */
		
		/* 
			usare la system call msgrcv per ricevere i messaggi dalla coda.
			Il terzo parametro della msgrcv rappresenta la dimensione
			del messaggio ad esclusione dello spazio necessario a contenere
			il campo mtype della struttura.
			Il quarto parametro indica che devono essere letti solo i messaggi
			con message type MSGTYPE.
			L'ultimo parametro deve indicare che il processo deve rimanere in attesa
			se non ci sono messaggi nella coda.
			Vedere man msgrcv.			
		*/

		
		/*
			ogni volta che viene letto un messaggio l'allarme deve
			essere disattivato per evitare che scatti impropriamente
		*/

		
		/* stampare a video il contenuto del messaggio */

}

