/******************************************
MODULO: pipe_exec.c
SCOPO: il padre scrive una frase in una pipe,
il figlio carica un programma che legge dalla pipe
e stampa i dati in output.
******************************************/
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main (int argc, char *argv[])
{
    int status;

    int p[2]; /* file descriptor per la pipe */

    char fd[10]; /* quando il file descriptor della pipe viene fornito alla
                    exec deve essere una stringa, ma gli elementi di p sono
                    interi. Utilizzo fd per copiare il valore di p[0] sul
                    vettore di caratteri fd. */

    /* la pipe deve essere creata prima di chimare la fork */
    if(pipe (p) == -1) {
        perror("pipe");
        exit(1);
    }

    printf("Sono il padre e creo un figlio\n");

    /* Creazione del figlio */
    if ((status=fork()) == -1) {
        perror("fork() fallita\n");
        exit(2);
    } else if (status == 0) { /* codice del figlio */

        /*
          il figlio chiude la pipe in scrittura perchè utilizza la pipe solo
          per leggere
        */
        if(close (p[1]) == -1) {
            perror("figlio -- close");
            exit(3);
        }

        /*
          copio il valore del file descriptor di lettura nel vettore
          di caratteri fd che verrà fornito come argomento alla exec.
        */
        sprintf(fd, "%d", p[0]);

        /*
          il figlio carica il programma pread.x
          pread.x legge dal file descriptor fornitogli come
          argomento alla linea di comando.
        */
        execlp("./pread.x", "pread.x", fd, NULL);
        perror("execlp");
        exit(4);

    } else  {  /* codice del padre */

        /*
          il padre chiude la pipe in lettura perchè utilizza la pipe
          solo per scrivere
        */
        if(close (p[0]) == -1) {
            perror("padre -- close");
            exit(5);
        }

        /*
          il padre scrive nella pipe utilizzando la system call
          write
        */
        if(write (p[1], "In bocca al lupo", 17) == -1) {
            perror("padre -- write");
            exit(6);
        }

        wait (NULL);
        exit (0);
    }
}
