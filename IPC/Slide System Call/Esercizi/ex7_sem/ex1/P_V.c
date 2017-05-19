#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* definizioni numeri dei semafori */
#define PADRE 0
#define FIGLIO 1

int semid;                                  /* identificatore del vettore di semafori */

void sem_wait(int sem_number) {

    /* definire il corpo della sem_wait in funzione
       della system call semop()
       Si ricorda che la sem_wait (ovvero la P) deve bloccare
       il processo chiamante nel caso in cui si tenti di portare il
       valore del semaforo ad avere un valore < 0
       L'argomento sem_number e' il numero di semaforo dell'arrey
       semid su cui eseguire la sem_wait
    */

}


void sem_signal(int sem_number) {

    /* definire il corpo della sem_signal in funzione
       della system call semop()
       Si ricorda che la sem_signal (ovvero la V) deve incrementare
       della quantita' 1 il valore del semaforo
       L'argomento sem_number e' il numero di semaforo dell'arrey
       semid su cui eseguire la sem_signal
    */
}


int main(int argc, char *argv[]) {
    int i;                                          /* variabile di servizio */
    pid_t pid;                                  /* conterra' il valore ritornato dalla fork() */
    key_t semkey;                               /* chiave del semaforo */


    union semun {                               /* union per la gestione del controllo sui semafori */
        int val;
        struct semid_ds *buf;
        ushort *array;
    } st_sem;

    /*
      assegnazione della chiavi per i semafori.
      Viene usata la funzione ftok che dato un nome di file e un carattere
      restituisce una chiave univoca.
      Vedere man ftok.
    */
    semkey = ftok("P_V.c", 'a');

    /* creazione di un vettore di 2 semafori PADRE (0) e FIGLIO (1) */
    if ((semid = semget(semkey, 2, 0666|IPC_EXCL|IPC_CREAT)) == -1) {
        perror("Creazione semaforo non riuscita.\n");
        exit(1);
    }

    /*
      assegnazione del valore iniziale ai semafori PADRE e FIGLIO.
      PADRE inizializzato a 1, FIGLIO inizializzato a 0.
      Per impostare il valore dei semafori viene usata
      la struttura st_sem e la system call semctl.
      Alla semctl vengono forniti 4 parametri:
      - identificatore del vettore di semafori
      - numero del semaforo all'interno del vettore
      - comando: in questo caso SETVAL
      - union contenente il valore da settare
      Vedere man semctl.
    */
    st_sem.val = 1;
    if (semctl(semid, PADRE, SETVAL, st_sem) == -1) {
        perror ("Semaforo PADRE non inizializzato.\n");
        semctl(semid, 0, IPC_RMID, 0);
        exit(1);
    }

    st_sem.val = 0;
    if (semctl(semid, FIGLIO, SETVAL, st_sem) == -1) {
        perror ("Semaforo FIGLIO non inizializzato.\n");
        semctl(semid, 0, IPC_RMID, 0);
        exit(1);

    }

    /* Creazione di un processo figlio che deve sincronizzarsi con il padre
       Padre e figlio devono eseguire l'operazione A in modo mutuamente
       esclusivo e alternativamente.
    */
    if ((pid = fork()) == -1) {
        perror("fork");
        semctl(semid, 0, IPC_RMID, 0);
        exit(1);

    } else if (pid == 0) { /*figlio*/

        for(i=0;i<5;i++){
            sem_wait(FIGLIO);
            printf("FIGLIO[%d]: Sbloccato. Eseguo A.\n", i);
            sleep(rand()%3);    /* dormo per un numero di secondi tra 0 e 2 */
            printf("FIGLIO[%d]: Ora sblocco mio padre.\n", i);
            sem_signal(PADRE);
            printf("FIGLIO[%d]: Ora mi blocco.\n", i);
        }
        printf("FIGLIO: Bye.\n");
        exit(0);

    } else { /*padre*/

        for(i=0;i<5;i++){
            sem_wait(PADRE);
            printf("\t\t\t\tPADRE[%d]: Sbloccato. Eseguo A.\n", i);
            sleep(rand()%3);    /* dormo per un numero di secondi tra 0 e 2 */
            printf("\t\t\t\tPADRE[%d]: Ora sblocco mio figlio.\n", i);
            sem_signal(FIGLIO);
            printf("\t\t\t\tPADRE[%d]: Ora mi blocco.\n", i);
        }
        wait(NULL);
        semctl(semid, 0, IPC_RMID, 0);
        printf("\t\t\t\tPADRE: Bye.\n");
        exit(0);
    }
}
