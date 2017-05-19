/******************************************
MODULO: myshell.c
SCOPO: Mini-shell
******************************************/
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#define MAXARG 20

void execute(int argc, char *argv[], int mode);
char getargs(int *argcp, char *argv[], int max);

int main (){
    char *argv[MAXARG+1];
    char *prompt;
    int argc;

    while(1) {
        printf(">");
        if(!getargs(&argc, argv, MAXARG)|| argc == 0)
            continue;

        if(strncmp(argv[0], "exit", 4) == 0)
            exit(0);

        if(strncmp(argv[argc-1], "&", 1) == 0) {    /*background execution*/
            argv[argc-1] = NULL;
            argc--;
            execute(argc, argv, 0);
        } else {                                    /*foreground execution*/
            execute(argc, argv, 1);
        }
    }
}


char getargs(int *argcp, char *argv[], int max){
    static char cmd[100];
    char *cmdp;
    int i;

    if(gets(cmd) == NULL)
        exit(0);

    cmdp = cmd;

    for(i=0; i<=max; i++){

        if((argv[i] = strtok(cmdp, " \t")) == NULL)
            break;

        cmdp = NULL;
    }

    if(i > max){
        printf("Too many arguments\n");
        return -1;
    }

    *argcp = i;
    return(1);
}


void execute(int argc, char *argv[], int mode) {

    int i, j;
    int p[2];
    char *t_argv[MAXARG+1];

    switch (fork()) {

        case -1:
            printf("Can't create new process\n");
            return;

        case 0: /* Codice del figlio. */
            fflush(stdout);

            /* Cerca una eventuale simbolo di pipe (|) tra gli argomenti del comando. */
            for(i=1; i < argc; i++) {
                if(strncmp(argv[i], "|", 1) == 0) {

                    /* E' stato trovato il simbolo | quindi creo la pipe. */
                    if(pipe(p) == -1) {
                        perror("pipe");
                        exit(1);
                    }

                    /* Il processo si forca per eseguire la pipe di comandi. */
                    switch (fork()) {
                        case -1:
                            printf("Can't create new process\n");
                            return;
                        case 0 :/* codice del figlio che esegue il primo comando */

                            /* Chiudo il file descriptor 1 (stdout) per redirigerlo su pipe. */
                            if (close(1) == -1) {
                                perror("close");
                                exit(1);
                            }

                            /*
                             * La dup duplica il descrittore di file p[1]
                             * nel descrittore di file di valore più basso partendo
                             * da 0. In questo caso il valore piu' basso e' 1 visto che ho
                             * appena chiuso il file descriptor 1 e lo 0 e' ancora assegnato
                             * allo stdout.
                             */
                            if (dup(p[1]) == -1) {
                                perror("dup");
                                exit(1);
                            }

                            /*
                             * Chiudo i descrittori di file ritornati dalla pipe.
                             * Non mi servono piu' dal momento che il descrittore per la
                             * lettura non viene usato (il figlio vuole solo scrivere)
                             * mentre quello per la scrittura e' stato rediretto sul
                             * file descriptor 1 con la dup.
                             */
                            if ((close(p[0]) == -1) || (close(p[1]) == -1)) {
                                perror("close2");
                                exit(1);
                            }

                            /* Riconoscimento del comando che precede il simbolo di pipe. */
                            for(j = 0; j < i; j++) {
                                t_argv[j] = argv[j];
                            }
                            /*
                             * Inseirsco NULL come ultimo elemento del vettore
                             * fornito alla exec
                             */
                            t_argv[j] = NULL;

                            /*
                             * Eseguo il comando. L'output del comando scrivera'
                             * sul file descriptor 1 credendo di scrivere sullo
                             * stdout. In realta' visto che il file descriptor 1 e' stato
                             * assegnato alla pipe, il comando scrive sulla pipe
                             * */
                            execvp(t_argv[0], t_argv);


                            /* Se il figlio ritorna allora si e' verificato un errore. */
                            printf("Can't execute\n");
                            perror("execvp");
                            exit(1);

                        default:

                            /* Codice del padre. Esegue il comando dopo la pipe.
                             * La gestione della pipe e'analoga a quella effettuata per il
                             * processo figlio. Il padre esegue il comando che sta dopo il
                             * simbolo |. Dal momento che il comando cerca di leggere i
                             * dati da elaborare dallo stdin bisogna ingannarlo facendolo
                             * leggere dalla pipe. Per fare ciò basta fare il trucchetto
                             * della duplicazione del file descriptor della pipe di lettura
                             * sul file descriptor 0.
                             */

                            /* Chiudo il file descriptor 0 (stdin) per redirigerlo sulla pipe. */
                            if (close(0) == -1) {
                                perror("close");
                                exit(1);
                            }

                            /* La dup ritorna il descrittore di file 0. */
                            if (dup(p[0]) == -1) {
                                perror("dup");
                                exit(1);
                            }

                            /* Chiudo i descrittori di file ritornati dalla pipe
                             * perche' ormai non li uso piu'.
                             */
                            if ((close(p[0]) == -1) || (close(p[1]) == -1)) {
                                perror("close2");
                                exit(1);
                            }

                            /* Riconosco il comando che segue il simbolo di pipe. */
                            for(j = i+1; j < argc; j++) {
                                t_argv[j-i-1] = argv[j];
                            }

                            /* Tappo per la exec. */
                            t_argv[j] = NULL;

                            /* Eseguo il comando leggendo l'input dalla pipe. */
                            execvp(t_argv[0], t_argv);

                            /* Se il padre ritorna allora si e' verificato un errore. */
                            printf("Can't execute\n");
                            perror("execvp");
                            exit(1);
                    }
                }
            }

            /* Se eseguo questo codice allora non ho trovato il simbolo di pipe. */
            execvp(argv[0], argv);
            printf("Can't execute\n");
            perror("execvp");
            exit(1);

        default: /* padre */
            if(mode) { /* Attendo la terminazione del figlio solo se il comando
                        *   viene eseguito in foreground.
                        */
                if(wait(NULL) == -1)
                    perror("wait");
            }
    }
}
