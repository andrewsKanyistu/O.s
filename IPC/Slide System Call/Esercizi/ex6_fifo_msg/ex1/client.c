#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "def.h"

#define MSGKEY   90		/* chiave per la coda */
#define MSGTYPE  1		/* tipo per il messaggio da inviare */
	 
			 
int main(int argc, char *argv[]) {

	/*
		dichiarare un puntatore alla struttura che rappresenta il messaggio.
		Il messaggio va definito nel file def.h
	*/

	/*
		dichiarare l'identificatore per la coda
	*/
	
	/*
		Alloracare dinamicamente la memoria per il messaggio tramite la malloc
	*/


	/* 
		aprire la coda di messaggi corrispondente 
		alla chiave MSGKEY usando la funzione msgget
	*/

	/*
		fare in modo che il processo cicli finchè l'utente vuole
		inviare nuovi messaggi al server
	*/
	
	/* 
		creare il messaggio da inviare leggendo i dati
		da tastiera e inserendoli nei campi del messaggio
	*/

	
		/* 
			usare la system call msgsnd per spedire il messaggio 
			memorizzato verso la coda 
			Il terzo parametro della funzione rappresenta la dimensione
			del messaggio ad esclusione dello spazio necessario a contenere
			il campo mtype della struttura.
			L'ultimo parametro va lasciato a 0 in modo che il processo 
			rimanga in attesa	se la coda è piena finche' non si libera 
			spazio per inserire il messaggio.
			Vedere man msgsnd.			
		*/

		
		/*
			chiedere all'utente se vuole inviare 
			un nuovo messaggio 
		*/
}
