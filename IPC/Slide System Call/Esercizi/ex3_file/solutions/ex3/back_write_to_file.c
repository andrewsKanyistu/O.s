/******************************************
MODULO: back_write.c
SCOPO:  Scrivere un file al contrario
        dall'ultima alla prima riga invertendo
        anche l'ordine dei caratteri per ogni
        riga. Ridirigere l'output su un altro
        file.
******************************************/
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <stdlib.h>
#include "mylib.h"

void backward(char *from, char *to);

int main (int argc, char *argv[])
{

    if (argc != 3) {
        fprintf(stderr,"Usage: back_write <source> <dest>.\n");
        exit(1);
    }

    backward(argv[1], argv[2]);

    return 0;
}


void backward(char *from, char* to) {
    char c; /* buffer per leggere un carattere alla volta*/
    int fd_from; /* file descriptor dell'origine */
    int fd_to; /* file descriptor della destinazione */
    int nread; /* numero di byte letti */
    long where; /* posizione all'interno del file */

    /*
      Apro il file origine
    */
    if ((fd_from = open(from, O_RDONLY)) == -1)
        syserr("backward", "open from");

    /*
      Creo il file destinazione con permessi 666
    */
    if ((fd_to = creat(to, 0666)) == -1)
        syserr("backward", "creat to");

    /*
      Mi posiono davanti all'ultimo carattere del file
      origine
    */
    if ((where = lseek(fd_from, -1L, SEEK_END)) == -1)
        syserr("backward", "lseek");

    /*
      Leggo un carattere alla volta da file
    */
    while ((nread = read(fd_from, &c, 1)) == 1) {

        /*
          Scrivo il carattere letto nel file destinazione
        */
        write(fd_to, &c, nread);


        /* Se sono all'inizio del file esco perchè ho terminato */
        if (where == 0)
            break;

        /*
          Altrimenti torno indietro di 2 byte, quello appena letto
          e il prossimo che voglio leggere
        */
        if ((where = lseek(fd_from, -2L, SEEK_CUR)) == -1)
            syserr("backward", "lseek");
    }

    if (nread == -1)
        syserr("backward", "read");

    /*
      Chiusura dei file
    */
    if (close(fd_from) == -1)
        syserr("backward", "close from");
    if (close(fd_to) == -1)
        syserr("backward", "close to");
}
