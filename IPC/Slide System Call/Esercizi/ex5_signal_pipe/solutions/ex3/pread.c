/******************************************
MODULO: pread.c
SCOPO: il programma legge i dati dal file
descriptor fornito come argomento nella linea
di comando e li scrive in output.
******************************************/
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {

	int fd; /* file descriptor*/
	int n;
	char s[100]; /* buffer per la read */


	/* converto il valore della stringa argv[1] in intero */
	fd = atoi(argv[1]);

	printf("Figlio -- Leggo dal file descriptor numero %d\n", fd);
	switch (n = read(fd, s, sizeof(s))) {
		case -1:
			perror("read");
		case 0:
			printf("end of file\n");
			break;
		default:
			printf("Figlio -- ho letto %d bytes: %s\n", n, s);
	}
}
