/******************************************
MODULO: back_write.c
SCOPO:  Scrivere un file al contrario su un
altro file senza usare ne' fprintf
ne' write, ma solo printf.
******************************************/
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include "mylib.h"

void backward(char *from, char* to);

int main (int argc, char *argv[])
{

    if (argc != 3) {
        fprintf(stderr,"Usage: back_write <sourcefile> <destfile>.\n");
        exit(1);
    }

    backward(argv[1], argv[2]);
    return 0;
}


void backward(char *from, char* to) {
    char s[101], c;
    int i, fd_from, fd_to, nread;
    long where;

    /*
      Apro il file origine
    */
    if ((fd_from = open(from, 0)) == -1)
        syserr("backward", "open from");

    /*
      Creo il file destinazione
    */
    if ((fd_to = creat(to, 0666)) == -1)
        syserr("backward", "creat to");


    /*
      Chiudo lo stdout. Il file descriptor 1, notoriamente
      assegnato allo stdout, viene rilasciato ed è quindi
      disponibile per essere utilizzato.
    */
    if (close(1) == -1)
        syserr("backward", "close stdout");

    /*
      La dup assegna un nuovo file descriptor al file
      collegato a "fd_to" pescando dal mazzo dei file
      descriptor non utilizzati. Per sua natura la dup
      assegna il primo file descriptor libero partendo dallo
      0. Lo 0 e' assegnato a stdin, quindi il primo libero e' 1
      dal momento che l'ho chiuso con l'istruzione precedente.
      Al termine della dup, sia il file descriptor 1 che il
      file descriptor "fd_to" saranno assegnati allo stesso file.
    */
    dup(fd_to);

    /*
      Posso chiudere il file descriptor fd_to tanto il file
      destinazione ora e' attaccato anche al file descriptor 1
    */
    if (close(fd_to) == -1)
        syserr("backward", "close fd_to");


    /*
      Da qui in avanti il codice e' analogo a quello di
      back_write.c ma ora la printf non scrive piu' su
      stdout bensi' sul file destinazione. Per sua natura
      infatti la printf scrive sul file descriptor 1 che
      ora non e' piu' assegnato allo stdout, ma al file
      destinazione.
    */
    if ((where = lseek(fd_from, -1L, SEEK_END)) == -1)
        syserr("backward", "lseek");
    i = sizeof(s) - 1;
    s[i] = '\0';
    while ((nread = read(fd_from, &c, 1)) == 1) {
        if (c == '\n') {
            printf("%s", &s[i]);
            i = sizeof(s) - 1;
        }
        if (i == 0) {
            printf("Line too long.\n");
            exit(1);
        }
        s[--i] = c;
        if (where == 0)
            break;
        if ((where = lseek(fd_from, -2L, SEEK_CUR)) == -1)
            syserr("backward", "lseek");
    }

    if (nread == -1)
        syserr("backward", "read");

    printf("%s", &s[i]);

    /*
      printf e' bufferizzata. Quindi prima
      di chiudere il file, dobbiamo assicurarci di aver svuotato i buffer:
    */
    fflush( NULL );

    if (close(fd_from) == -1)
        syserr("backward", "close from");
}
