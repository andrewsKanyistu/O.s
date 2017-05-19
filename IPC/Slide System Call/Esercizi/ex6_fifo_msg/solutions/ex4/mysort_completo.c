#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {

    int f_from_P_to_F_read; /* file descriptor per la lettura dalla fifo f_from_P_to_F */
    int f_from_P_to_F_write; /* file descriptor per la scrittura nella fifo f_from_P_to_F */
    int f_from_F_to_P_read; /* file descriptor per la lettura dalla fifo f_from_F_to_P */
    int f_from_F_to_P_write; /* file descriptor per la scrittura nella fifo f_from_F_to_P */

    int fd;
    int nread;
    char buf[512];

    /*
      creo la fifo f_from_P_to_F per la comunicazione dal padre verso il figlio
      Si ricorda che le fifo possono essere utilizzate anche per fa comunicare
      processi non imparentati dal momento che il riferimento alla fifo viene
      fatto usando il nome e non solo il file descriptor!!!
    */
    mknod("/tmp/f_from_P_to_F", S_IFIFO | 0777, 0);

    /*
      creo la fifo f_from_P_to_F per la comunicazione dal figlio verso il padre
    */
    mknod("/tmp/f_from_F_to_P", S_IFIFO | 0777, 0);


    switch(fork()) {
        case -1:                            /*errore*/
            perror("fork");
            exit(1);

        case 0:                             /* codice del figlio */

            /*
              Apro la fifo f_from_P_to_F in lettura
              e la fifo f_from_F_to_P in scrittura
            */
            f_from_P_to_F_read = open("/tmp/f_from_P_to_F", O_RDONLY);
            f_from_F_to_P_write = open("/tmp/f_from_F_to_P", O_WRONLY);

            /* Chiudo lo standard input */
            if (close(0) == -1) {
                perror("close");
                exit(1);
            }

            /*
              Duplico il descrittore della fifo f_from_P_to_F di lettura
              nel descrittore 0 che è rimasto libero dopo aver chiuso stdin
            */
            if(dup(f_from_P_to_F_read) != 0) {
                perror("dup");
                exit(1);
            }

            /* Chiudo lo standard output */
            if(close(1) == -1) {
                perror("close");
                exit(1);
            }

            /*
              Duplico il descrittore della fifo f_from_F_to_P di scrittura
              nel descrittore 1 che è rimasto libero dopo aver chiuso stdout
            */
            if(dup(f_from_F_to_P_write) != 1) {
                perror("dup");
                exit(1);
            }

            /*
              Chiudo tutti i descrittori delle fifo del figlio
              A questo punto stdin, ovvero il descrittore 0, è collegato
              alla f_from_P_to_F in lettura, mentre il descrittore 1, è
              collegato alla f_from_F_to_P in scrittura.
            */
            if((close(f_from_P_to_F_read) == -1) || (close(f_from_F_to_P_write) == -1)) {
                perror("close");
                exit(1);
            }

            /*
              Invoco il comando sort.
              sort si aspetta di leggere i dati da ordinare dal descrittore 0
              e di dover scrivere i dati ordinati nel descrittore 1
            */
            execlp("sort", "sort", NULL);
            perror("execlp");
            exit(1);
    }

    /* codice del padre */

    /*
      Apro la fifo f_from_P_to_F in scrittura
      e la fifo f_from_F_to_P in lettura
    */
    f_from_P_to_F_write = open("/tmp/f_from_P_to_F", O_WRONLY);
    f_from_F_to_P_read = open("/tmp/f_from_F_to_P", O_RDONLY);


    /* Apro il file con i dati */
    if((fd = open(argv[1], O_RDONLY, 644)) == -1) {
        perror("open");
        exit(1);
    }

    /* Leggo i dati dal file e li metto in buf */
    while((nread = read(fd, buf, sizeof(buf))) != 0) {
        if(nread == -1) {
            perror("read");
            exit(1);
        }

        /* Scrivo i dati sulla fifo f_from_P_to_F */
        if(write(f_from_P_to_F_write, buf, nread) == -1) {
            perror("write");
            exit(1);
        }
    }

    /* Chiudo il file e il descrittore di scrittura della fifo f_from_P_to_F */
    if(close(fd) == -1 || close(f_from_P_to_F_write) == -1) {
        perror("close");
        exit(1);
    }

    /* Leggo i dati ordinati dalla fifo f_from_F_to_P li metto in buf */
    while((nread = read(f_from_F_to_P_read, buf, sizeof(buf))) > 0) {

        /* Scrivo i dati su stdout */
        write(1, buf, nread);
    }

    /* Chiudo il descrittore di lettura della fifo f_from_F_to_P */
    if(close(f_from_F_to_P_read) == -1) {
        perror("close");
        exit(1);
    }

    wait(NULL);

    /*
      Al termine del loro utilizzo le fifo devono essere rimosse
      esplicitamente. In caso contrario al termine del programma
      rimarra una fifo nel file system.
      Provate a rimuovere le 2 rige sottostanti e controllate cosa
      contiene la directory /tmp  dopo aver lanciato il programma.
    */
    unlink("/tmp/f_from_P_to_F");
    unlink("/tmp/f_from_F_to_P");
}
