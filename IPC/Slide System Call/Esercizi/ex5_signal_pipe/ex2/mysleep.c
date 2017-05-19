#include <stdio.h>
#include <unistd.h>
#include <signal.h>


/*
	Definire un funzione che gestisca il segnale SIGALRM
	dicendo che è suonata la sveglia.
	E' la funzione che deve essere passata come 2° parametro
	alla signal. Vedere man 2 signal.
 */
 
... ...


/* 
	definire la funzione che emula la sleep
*/
void mysleep(int secs) {
	
	/* 
		registrare il processo per catturare il segnale SIGALRM inviato
		dalla alarm alla scadenza del timer.
	*/

	... ...
	
	/* 
		imposto l'allarme a secs secondi.
		la funzione alarm invia un segnale SIGALRM quando
		scade il timer.
		vedere man alarm
	*/
	
	... ...
	
	/* 
		attendere la ricezione di SIGALRM
	*/
	
	... ...
	
	/* 
		disabilitare l'allarme per evitare che scatti se il processo si 
		sveglia prima che l'allarme scatti.
		Ciò può succedere se mentre attendo che il timer svegli, il processo si
		sblocca dalla attesa per l'arrivo di un segnale diverso da SIGALRM 
		(magari inviato dal kernel o da qualche altro processo).
	*/

	... ...
		
}


int main(int argc, char *argv[]) {
	

	/* 
		gestire l'argomento della linea di comando.
		l'argomento rappresenta il numero di secondi
		per i quali il processo deve essere messo a dormire.
	*/

	... ...
	
	/* invocare la funzione mysleep */
	
	... ...
	
	/* inserire nel codice alcune printf per mostrare che il processo
	si addormenta e poi si sveglia */
	
}
