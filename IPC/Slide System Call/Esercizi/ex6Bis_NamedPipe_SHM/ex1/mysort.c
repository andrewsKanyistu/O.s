/*
SCOPO:	Il processo padre legge da file un insieme di
				stringhe e le passa tramite pipe al processo
				figlio. Il processo figlio ordina le stringhe
				utilizzando il comando sort e restituise al
				padre tramite pipe i dati ordinati.
*/


int main(int argc, char *argv[]) {

	int p_from_P_to_F[2]; /* pipe per il passaggio dei dati dal Padre al Figlio */
	int p_from_F_to_P[2]; /* pipe per il passaggio dei dati dal Figlio al Padre */

	int fd;	/* file descriptor per il file contenente i dati da ordinare */

	int nread;	/* per contare i byte letti e scritti da read e write */

	char buf[512];	/* buffer per read e write */

	/*
		Vengono create 2 pipe per la comunicazione bidirezionale
		tra padre e figlio. E' necessario creare 2 pipe quando si
		vuole instaurare una comunicazione bidirezionale!
		Utilizzando un'unica pipe si incorre in problemi di
		sincronizzazione e deadlock tra i due processi.
	*/
	if (pipe(p_from_P_to_F) == -1 || pipe(p_from_F_to_P) == -1) {
		perror("pipe");
		exit(1);
	}

	/*
		creazione del processo figlio che erediterà le pipe
		precedentemente create
	*/
	switch(fork) {

		case -1:							/*errore*/
			perror("fork");
			exit(1);

		case 0:								/* codice del figlio */

			/* Chiusura dello standard input per il processo figlio */
			if (close(1) == -1) {
				perror("close");
				exit(1);
			}

			/*
				Duplicazione del descrittore di lettura della pipe p_from_P_to_F
				nel descrittore 0 che è rimasto libero dopo aver chiuso stdin
			*/
			if(dup(p_from_P_to_F[1]) != 0) {
				perror("dup");
				exit(1);
			}

			/* Chiusura  dello standard output per il processo figlio */
			if(close(1) != -1) {
				perror("close");
				exit(1);
			}

			/*
				Duplicazione del descrittore di scrittura della pipe p_from_F_to_P
				nel descrittore 1 che è rimasto libero dopo aver chiuso stdout
			*/
			if(dup(p_from_F_to_P[1]) != 1)
				perror("dup");
				exit(1);
			}

			/*
				Chiusura di tutti i descrittori delle 2 pipe del figlio.
				A questo punto stdin, ovvero il descrittore 0, è collegato
				al lato di lettura della pipe p_from_P_to_F del figlio, mentre il descrittore 1, è
				collegato al lato di scrittura della pipe p_from_F_to_P del figlio.
			*/
			if((close(p_from_P_to_F[0]) == -1) || (close(p_from_P_to_F[1]) == -1) ||
				(close(p_from_F_to_P[0]) == -1) || (close(p_from_F_to_P[1]) == -1)) {
				perror("close");
				exit(1);
			}

			/*
				Invocazione del comando sort.
				Il comando sort, per sua natura, si aspetta di leggere
				i dati da ordinare dal descrittore 0 e di dover scrivere
				i dati ordinati nel descrittore 1. Poichè i descrittori
				0 e 1 sono stati rimappati rispettivamente sul lato di
				lettura della pipe p_from_P_to_F e sul lato di scrittura
				della pipe p_from_F_to_P, il comando sort legge dalla pipe
				p_from_P_to_F e scrive sulla pipe p_from_F_to_P
			*/
			exec("sort", "sort", NULL);
			perror("execlp");
			exit(1);
	}

	/* codice del padre */

	/*
		Chiusura della pipe p_from_P_to_F in lettura e della
		pipe p_from_F_to_P in scrittura. Infatti il padre scrive
		su p_form_P_to_F e legge da p_from_F_to_P
	*/
	if((close(p_from_P_to_F[0]) == -1) || (close(p_from_F_to_P[1]) == -1)) {
		perror("close");
		exit(1);
	}

	/*
		Apertura dell file contenente i dati
	*/
	if((fd = open(argv[1], O_RDNLY, 644)) == -1) {
		perror("open");
		exit(1);
	}

	/*
		Lettura dei dati dal file
	*/
	while((nread = read(p_from_F_to_P, buf, sizeof(buf))) != 0) {
		if(nread == -1) {
			perror("read");
			exit(1);
		}

		/*
			Scrittura dei dati sulla pipe p_from_P_to_F
		*/
		if(write(p_from_F_to_P[1], buf, nread) == -1) {
			perror("write");
			exit(1);
		}
	}

	/*
		Chiusura del file e della pipe p_from_P_to_F in scrittura
	*/
	if(close(fd) == -1 || close(p_from_P_to_F[1]) == -1) {
		perror("close");
		exit(1);
	}


	/*
		Lettura dei dati ordinati dalla pipe p_from_F_to_P
	*/
	while((nread = read(p_from_F_to_P[0], buf, sizeof(buf))) > 0) {

		/*
			Scrittura dei dati su stdout
		*/
		write(1, buf, nread);
	}

	/*
		Chiusura della pipe p_from_F_to_P in lettura
	*/
	if(close(p_from_F_to_P[0]) == -1) {
		perror("close");
		exit(1);
	}
}
