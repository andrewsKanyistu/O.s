/******************************************
MODULO: back_write.c
SCOPO:  Scrivere un file al contrario
		dall'ultima alla prima riga su stdout
******************************************/
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include "mylib.h"

void backward(char *from); /* dichiarazione della funzione di inversione */

int main (int argc, char *argv[])
{

	if (argc != 2) {
		fprintf(stderr,"Usage: back_write <sourcefile>.\n");
		exit(1);
	}

	backward(argv[1]); /* chiamata alla funzione di inversione */

    return 0;
}


/* definizione della funzione di inversione*/
void backward(char *from)
{
	char s[101]; /* vettore che contiene una riga del file*/
	char c; /* buffer per leggere un solo carattere alla volta*/
	int i; /* indice per il vettore di caratteri s */
	int fd_from; /* file descriptor */
	int nread; /* numero di caratteri letti */
	long where; /* posizione all'interno del file */

	/*
		Apertura del file
	*/
	if ((fd_from = open(from, O_RDONLY)) == -1)
		syserr("backward", "open from");

	/*
		Mi posiziono davanti all'ultimo carattere del file (i.e. EOF)
	*/
	if ((where = lseek(fd_from, -1, SEEK_END)) == -1)
		syserr("backward", "lseek");

	/*
		Chiudo il vettore di caratteri inserendo il "tappo"
		nell'ultima posizione del vettore
	*/
	printf("Dimensione buffer %ld\n", sizeof(s));

	i = sizeof(s) - 1;
	s[i] = '\0';

	/*
		Leggo un carattere alla volta a partire dall'ultimo.
		Ogni volta che leggo il carattere "\n" significa che ho
		letto tutta una riga e quindi posso scriverla sullo stdout.
	*/
	while ((nread = read(fd_from, &c, 1)) == 1) {

		if (c == '\n') {
			printf("%s", &s[i]); /* stampo la stringa s a partire dall'elemento i */
			i = sizeof(s) - 1; /* riporto l'indice del vettore s all'ultimo valore */
		}

		/* Se l'indice i diventa 0 ho finito lo spazio sul vettore s */
		if (i == 0) {
			printf("Line too long.\n");
			exit(1);
		}

		/*
			Memorizzo in s il carattere letto dal file dopo aver aggiornato il
			valore per l'indice i.
			Notare che i caratteri vengono inseriti nel vettore s a partire dalla
			posizione finale per preservare l'ordine dei caratteri nella stampa.
			Infatti le righe del file devono essere invertite
			(l'ultima sara' la prima, etc...) ma i caratteri all'interno di una
			riga devono rimanere nell'ordine iniziale.
			N.B: --i significa che prima decremento il valore di i e poi lo uso
			     i-- significa che prima uso il valore di i e poi lo decremento
		*/
		s[--i] = c;

		/*
			Se lseek ritorna 0 significa che sono arrivato all'inizio del file
			e quindi ho finito di leggere ed esco dal while
		*/
		if (where == 0)
			break;

		/*
			Dopo aver letto un carattere devo risalire nel file di 2 posizioni:
			quella che ho appena letto e quella che voglio leggere ora
		*/
		if ((where = lseek(fd_from, -2L, SEEK_CUR)) == -1)
			syserr("backward", "lseek");
	}

	if (nread == -1)
		syserr("backward", "read");

	/*
		Scrivo l'ultima riga (la prima del file originale)
	*/
	printf("%s", &s[i]);


	/*
		Chiundo il file
	*/
	if (close(fd_from) == -1)
		syserr("backward", "close from");
}
