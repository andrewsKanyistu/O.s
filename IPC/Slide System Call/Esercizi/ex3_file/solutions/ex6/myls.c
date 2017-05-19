/*
  SCOPO: Emulare il comando ls -l
*/

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/dir.h>
#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <time.h>
#include "mylib.h"
#include <string.h>
int main(int argc, char *argv[]) {

    DIR *d; /* puntatore alla directory */
    struct dirent *dentry; /* puntatore alla entry della directory*/
    char *name = (char *) malloc(sizeof(char) * 1024); /* nome della entry della directory */
    struct stat *file_stat = (struct stat *) malloc(sizeof(struct stat));

    if (argc != 2) {
        printf("usage: myls <dirname>");
        exit(1);

    } else {

        /*
          Apertura delle directory argv[1]
        */
        if((d = opendir(argv[1])) == NULL)
            syserr("myls", "opendir");

        /*
          Ciclo per tutte le entry della directory
        */
        for(dentry = readdir(d); dentry != NULL; dentry = readdir(d)) {

            /*
              Recupero il nome della entry
            */
            strcpy(name, argv[1]);
            strcat(name, "/");
            strcat(name, dentry->d_name);
            printf("%s\n", name);

            /*
              Recupero la struttura stat per la entry definita da name.
              La struttura stat contiene tutte le informazioni che vengono
              visualizzate dal comando ls -l.
              Vedere il manuale di stat per i dettagli: man 2 stat
            */
            stat(name, file_stat);

            if (file_stat == NULL) continue;

            /*
              Controllo il tipo del file (metodo NON POSIX)
            */
            if((file_stat->st_mode & S_IFMT) == S_IFDIR) { /* e' una directory? POSIX: S_ISDIR(file_stat->st_mode) */
                printf("d ");
            } else if ((file_stat->st_mode & S_IFMT) == S_IFBLK) { /* e' un device a blocchi? */
                printf("b ");
            } else if ((file_stat->st_mode & S_IFMT) == S_IFCHR) { /* e' un device a caratteri? */
                printf("c ");
            } else if ((file_stat->st_mode & S_IFMT) == S_IFREG) { /* e' un file normale? */
                printf("- ");
            }

            /* stampa le protezioni*/
            printf("%o ", file_stat->st_mode & 0777);

            /* stampa il numero di hard link*/
            printf("%d ", (int)file_stat->st_nlink);

            /* stampa lo userd id */
            printf("%d ", file_stat->st_uid);

            /* stampa il group id */
            printf("%d ", file_stat->st_gid);

            /* stampa la dimensione in bytes */
            printf("%d ", (int)file_stat->st_size);

            /* stampa il tempo di ultima modifica*/
            printf("%s\n", ctime(&(file_stat->st_ctime)));
        }
    }

    free(file_stat);
    free(name);

    return 0;
}
