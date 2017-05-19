#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>


/* funzione che non fa nulla */
void nullfcn(int a) {
	printf("Sveglia, bip bip bip ... bip!!!\n");
}


/* funzione che imita la sleep */
void mysleep(int secs) {


	/* mi registro per catturare il segnale SIGALRM */
	signal(SIGALRM, nullfcn);

	/*
		imposto l'allarme a secs secondi.
		la funzione alarm invia un segnale SIGALRM quando
		scade il timer.
		vedere man alarm
	*/
	alarm(secs);

	/* aspetto al ricezione di SIGALRM */
	pause();

	/*
		disabilito l'allarme per evitare che scatti se il processo si
		sveglia dalla pause prima che l'allarme scatti.
		Ciò può succedere se mentre attendo che il timer si svegli, la pause
		si sblocca per l'arrivo di un segnale diverso da SIGALRM (magari inviato dal
		kernel o da qualche altro processo).
	*/
	alarm(0);

}


int main(int argc, char *argv[]) {

	if(argc != 2) {
		printf("Usage: mysleep.x <secs>\n");
		exit(1);
	}

	printf("Vado a dormire per %d secondi.\n", atoi(argv[1]));
	fflush(stdout);

	/* chima la funzione che emula la sleep */
	mysleep(atoi(argv[1]));

	printf("Che bella dormita, come sono riposato!\n");

}
