/****************************************************************************/
/*                                                                          */
/* QUESTO PROGRAMMA E' UN SEMPLCE ESEMPIO DI ``PRODUTTORE CONSUMATORE''     */
/* REALIZZATO MEDIANTE I SEMAFORI E LA MEMORIA CONDIVISA. L'AREA DI MEMORIA */
/* CONTIENE FISICAMENTE IL DATO DA SCAMBIARE, MENTRE IL SEMAFORO SERVE PER  */
/* LA PROTEZIONE DEL DATO STESSO (REGIONE CRITICA).                         */
/*                                                                          */
/****************************************************************************/


#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* definizioni numeri dei semafori per la gestione del buffer */
#define EMPTY 0     /* il semaforo 0 indica quando la coda è vuota */
#define FULL 1      /* il semaforo 1 indica quando la coda è piena */
#define MUTEX 2     /* il semaforo 2 è il mutex per gestire la regione critica */

struct sembuf wait_b;
struct sembuf signal_b;     /* strutture per le operazioni sui semafori */

int semaforo;                           /* identificatore del vettore di semafori */


/*
  procedura per l'esecuzione di una wait (P) sul semaforo numero
  sem_number. La procedura emula utilizzando i semafori di linux
  il comportamento di una P (ricordare teoria sui semafori).
*/
void sem_wait(int sem_number) {

    wait_b.sem_num = sem_number;                                /* setto il numero del semaforo */

    if (semop(semaforo, &wait_b, 1) == -1) {        /* eseguo la wait */

        perror("WAIT abortita. TERMINO.\n");
        exit(1);

    }
}


/*
  procedura per l'esecuzione di una signal (V) sul semaforo numero
  sem_number. La procedura emula utilizzando i semafori di linux
  il comportamento di una V (ricordare teoria sui semafori).
*/
void sem_signal(int sem_number) {

    signal_b.sem_num=sem_number;                                /* setto il numero del semaforo */

    if (semop(semaforo, &signal_b, 1) == -1) {  /* eseguo la signal */

        perror("SIGNAL abortita. TERMINO.\n");
        exit(1);

    }
}


int main(int argc, char *argv[]) {
    int i;                                          /* variabile di servizio */

    pid_t child;                                /* conterra' il valore ritornato dalla fork() */

    int sem_stato;                          /* stato del semaforo */
    key_t semkey;                               /* chiave del semaforo */

    key_t memkey;                               /* chiave della memoria condivisa */
    int shared_memory = -1;         /* identificatore della memoria condivisa */

    char *buffer_comune;                /* risorsa condivisa */
    char cons, prod;                        /* variabili per il consumo e la produzione */

    union semun {                               /* union per la gestione del controllo sui semafori */
        int val;
        struct semid_ds *buf;
        ushort *array;
    } st_sem;

    struct shmid_ds buf_mem;        /* struttura per il controllo della memoria condivisa */


    /*
      assegnazione delle due chiavi per i semafori e la memoria condivisa.
      Viene usata la funzione ftok che dato un nome di file e un carattere
      restituisce una chiave univoca.
      Vedere man ftok.
    */
    semkey = ftok("prod_cons.c", 'a');
    memkey = ftok("prod_cons.c", 'b');


    wait_b.sem_num = 0;     /* inizializzazione struttura per operazioni di wait */
    signal_b.sem_num = 0;   /* inizializzazione struttura per operazioni di signal */

    wait_b.sem_op = -1;     /* la 'wait' decrementa il valore del semaforo */
    signal_b.sem_op = 1;    /* la 'signal' incrementa il valore del semaforo */


    wait_b.sem_flg = SEM_UNDO;      /* flag di undo */
    signal_b.sem_flg = SEM_UNDO;    /* flag di undo */


    /* creazione del vettore di 3 semafori EMPTY, FULL e MUTEX */
    if ((semaforo = semget(semkey, 3, 0777|IPC_EXCL|IPC_CREAT)) == -1) {
        perror("Creazione semaforo non riuscita");
        exit(1);

    } else {

        /*
          assegnazione del valore iniziale al semaforo EMPTY.
          Il semaforo EMTPY viene inizializzato al valore 8
          dal momento che il buffer ha 8 elementi disponibili.
          In tal modo la semop sul semaforo EMPTY si bloccherà
          quando il suo valore scenderà a 0.
          Per impostare il valore del semaforo viene usata
          la struttura st_sem e la system call semctl.
          Alla semctl vengono forniti 4 parametri:
          - identificatore del vettore di semafori
          - numero del semaforo all'interno del vettore
          - comando: in questo caso SETVAL
          - union contenente il valore da settare
          Vedere man semctl.
        */
        st_sem.val = 8;
        sem_stato = semctl(semaforo, EMPTY, SETVAL, st_sem);

        /*
          Se non si riesce a inizializzare il semaforo
          allora si rimuove il vettore di semafori e
          si esce dal programma.
        */
        if (sem_stato==-1) {
            perror ("Semaforo EMPTY non inizializzato");
            semctl(semaforo, 0, IPC_RMID, 0);
            exit(1);
        }

        /*
          assegnazione del valore iniziale al semaforo MUTEX.
          MUTEX viene impostato al valore 1 in modo da
          garantire la mutua esclusione della regione
          critica che è rappresentata dalle operazionid di
          lettura e scrittura nella memoria condivisa.
          Il primo processo che esegue una wait su MUTEX
          entra nella regione critica impedendone l'accesso
          agli altri processi fino a che non esce.
        */
        st_sem.val = 1;
        sem_stato = semctl(semaforo, MUTEX, SETVAL, st_sem);

        if (sem_stato == -1) {
            perror ("Semaforo MUTEX non inizializzato");
            exit(1);
        }

        /*
          assegnazione del valore iniziale al semaforo FULL.
          Il semaforo FULL viene inizializzato al valore 0
          dal momento che inizialmente il buffer è vuoto.
        */
        st_sem.val = 0;
        sem_stato=semctl(semaforo, FULL, SETVAL, st_sem);

        if (sem_stato == -1) {
            perror ("Semaforo FULL non inizializzato");
            exit(1);
        }
    }


    /*
      creazione della memoria condivisa.
      La dimensione della memoria è di 8 byte
    */
    if ((shared_memory = shmget(memkey, 8, IPC_CREAT|0666)) == -1)  {
        perror("Allocazione memoria condivisa non riuscita\n");
        exit(1);
    }


    /*
      Arrivati a questo punto termina la fase di inizializzazione.
      E' stato creato l'insieme di semafori necessari alla sincronizza-
      zione di produttore e consumatore. Inoltre è stato creato dello
      spazio nella memoria condivisa per contenere fino ad 8 caratteri.

      Ora il programma prevede la creazione di un processo figlio
      che opera da consumatore, mentre il padre funge da produttore
    */
    child=fork();

    if (child < 0) {                /* errore nella fork */

        perror("Fork non riuscita\n");

        /* rimozione semafori */
        semctl(semaforo,0,IPC_RMID,&st_sem);

        /* rimozione memoria condivisa */
        shmctl(shared_memory,IPC_RMID,&buf_mem);

        /* esco */
        exit(1);
    }

    if (child == 0) {               /* Codice del figlio: consumatore */

        printf("FIGLIO: Sono il consumer.\n\n");

        srand(time(NULL));  /* inizializzazione del generatore di numeri random */

        /*
          la memoria condivisa viene mappata nell'area dati del figlio
          a partire dal primo indirizzo disponibile.
          Alla funzione shmat devono essere passati 3 parametri:
          - identificatore della memoria condivisa
          - indirizzo di partenza per mappare la memoria condivisa nell'area dati
          del processo. Se si fornisce il valore 0 la memoria viene mappata a partire
          dal primo indirizzo disponibile.
          - Eventuale flag di sola lettura (SHM_RDONLY) in OR con i permessi.
          La funzione shmat ritorna -1 in caso di errore.
          Vedere man shmat.
        */

        if ( (buffer_comune = (char *)shmat(shared_memory, 0, 0666) ) == (char *)-1) {
            perror("Memoria condivisa non attaccata all'area dati del processo figlio\n");
            exit(1);
        }

        i=0;
        do {
            /*
              il figlio si blocca se nella memoria condivisa non c'è
              niente da leggere, ovvero se il semaforo FULL vale 0,
              altrimenti il processo figlio si propone per iniziare
              la lettura dalla memoria condivisa
            */
            sem_wait(FULL);

            /*
              il figlio si blocca se il padre si trova nella regione critica,
              ovvero se il semaforo MUTEX vale 0, altrimenti il figlio
              entra nella regione critica e inizia la lettura vera e propria.
            */
            sem_wait(MUTEX);

            /* inizio regione critica*/
            cons = *(buffer_comune+i);  /* lettura dalla memoria condivisa */
            printf ("CONSUMER: ho letto %c :\n",cons);
            fflush(stdout);
            /* fine regione critica*/

            /*
              il figlio esegue una signal (V) sul semaforo MUTEX per rilasciare
              la regione critica
            */
            sem_signal(MUTEX);

            /*
              il figlio esegue una signal (V) sul semaforo EMPTY per indicare
              che ha liberato una posizione nel buffer dal momento che ne ha
              letto il contenuto. In tal modo se il produttore è in
              attesa che il valore del semaforo EMPTY diventi positivo, può
              terminare l'attesa e procedere con la scrittura di un nuovo dato.
            */
            sem_signal(EMPTY);


            /*
              A questo punto il consumatore potrebbe effettuare una operazione
              ipoteticamente molto onerosa in termini di tempo.
              Simuliamo tale comportamento addormentando il processo per un tempo variabile
              tra 0 e 4 secondi.
              la funzione rand() ritorna un numero casuale tra 0 e RAND_MAX (costante
              definita dal sistema).
            */
            sleep(rand() % 5);

            printf ("CONSUMER: ho consumato %c\n\n", cons);
            fflush(stdout);

            /* aggiornamento indice i necessario alle lettura nella memoria condivisa */
            i++;
            if(i == 8)
                i = 0;

        } while (cons!='r');    /* quando il consumatore legge la lettera r termina */


        /*
          Al termine dell'utilizzo, la memoria condivisa
          deve essere scollegata dall'area dati del processo
          figlio.
          vedere man shmdt
        */
        shmdt(buffer_comune);
        exit(0);


    } else { /* Codice del padre: produttore */

        printf("PADRE: Sono il producer.\n\n");

        srand(time(NULL)); /* inizializzazione del generatore di numeri random */

        /*
          la memoria condivisa viene mappata nell'area dati del padre
          a partire dal primo indirizzo disponibile come è stato fatto
          per il figlio.
        */
        if ((buffer_comune = (char *)shmat(shared_memory, 0, 0666)) == (char *)-1) {
            perror("Memoria condivisa non attaccata all'area dati del processo padre\n");

            /* in caso di errore rimuovo l'area comune  e i semafori */
            if (shmctl(shared_memory, IPC_RMID, &buf_mem) == -1)
                perror ("Non posso rimuovere la memorica condivisa: ");

            if (semctl(semaforo, 0, IPC_RMID, &st_sem) == -1) {
                printf ("Non posso rimuovere il semaforo %d\n", semaforo);
                perror ("");
            }
            exit(1);
        }


        /*
          se non si verificano errori il padre inizia la
          produzione dei dati e li scrive nel buffer di 8
          caratteri allocato nella memoria condivisa.
          I dati prodotti sono i caratteri da 'a' a 'r'
        */
        i = 0;
        for (prod='a'; prod <= 'r'; prod++) {

            /*
              il padre si blocca se nella memoria condivisa non c'è
              più spazio per scrivere, ovvero se il semaforo EMPTY vale 0,
              altrimenti il processo padre si propone per iniziare
              la scrittura dalla memoria condivisa
            */
            sem_wait(EMPTY);

            /*
              il padre si blocca se il figlio si trova nella regione critica,
              ovvero se il semaforo MUTEX vale 0, altrimenti il padre
              entra nella regione critica e inizia la scrittura vera e propria.
            */
            sem_wait(MUTEX);

            /*inizio regione critica*/

            *(buffer_comune + i)=prod;  /* scrittura nella memoria condivisa */

            /*
              A questo punto la produzione del dato potrebbe essere una operazione
              ipoteticamente molto onerosa in termini di tempo.
              Simuliamo tale comportamento addormentando il processo per un tempo variabile
              tra 0 e 1 secondo.
            */
            sleep(rand() % 2);
            printf ("\n\nPRODOTTO: %c\n\n",prod);
            fflush(stdout);

            /*fine regione critica*/

            /*
              il padre esegue una signal (V) sul semaforo MUTEX per rilasciare
              la regione critica
            */
            sem_signal(MUTEX);

            /*
              il padre esegue una signal (V) sul semaforo FULL per indicare
              che ha riempito una posizione nel buffer.
              In tal modo se il consumatore è in
              attesa che il valore del semaforo FULL diventi positivo, può
              terminare l'attesa e procedere con la lettura di un nuovo dato.
            */
            sem_signal(FULL);


            i++;
            if(i == 8)
                i = 0;
        }


        /* attesa terminazione processo figlio */
        wait(NULL);

        /*
          Al termine dell'utilizzo, la memoria condivisa
          deve essere scollegata dall'area dati del processo
          padre.
        */
        shmdt(buffer_comune);
        printf ("PADRE: Finalmente ho terminato di scrivere e mio figlio ha letto tutto.\n");

        /* eliminazione memoria condivisa */
        if (shmctl(shared_memory, IPC_RMID, &buf_mem) == -1)
            perror ("Non posso rimuovere la memorica condivisa: ");

        /* eliminazione semafori */
        if (semctl(semaforo, 0, IPC_RMID, &st_sem) == -1) {
            printf ("Non posso rimuovere il semaforo %d\n", semaforo);
            perror ("");
        }

        exit(0);
    }
}
