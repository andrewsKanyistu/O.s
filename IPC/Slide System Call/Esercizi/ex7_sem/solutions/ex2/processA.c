#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#define KEY 74

int main(int arcg, char *argv[]) {

    int semid;  /* identificatore dell'arrey di semafori */

    /* creo lo spazio per 2 operazioni sull'array di semafori */
    struct sembuf * sops = (struct sembuf *) malloc (2*sizeof(struct sembuf));

    int i;

    /*
      Creazione di un array di 3 semafori con permessi di lettura e scrittura
      per tutti. Le flag ICP_CREAT e IPC_EXCL fanno si che la funzione semget
      ritorni errore se esiste gi� un vettore di semafori con chiave KEY.
      Vedere man semget.

      Il semaforo 0 sincronizza il processo A
      Il semaforo 1 sincronizza il processo B
      Il semaforo 2 sincronizza il processo C
    */
    if((semid = semget(KEY, 3, IPC_CREAT | IPC_EXCL | 0666)) == -1) {
        perror("semget");
        exit(1);
    }

    /*Inizializzo i semafori dei processi B e C per bloccarli */

    /*
      Per eseguire operazioni ATOMICHE sui semafori si usa la funzione
      semop(). Vedere man semop.
      Alla funzione semop vengono passati 3 argomenti:
      - l'identificatore dell'array di semafori su cui eseguire l'operazione
      - il puntatore alla struttura sembuf necessaria per eseguire le operazioni
      - il numero di operazioni da eseguire

      Per ogni operazione da eseguire � necessario creare una struttura di tipo
      semop. La struttura contiene 3 campi:
      - il numero del semaforo da utilizzare. Ricordare che la semget ritorna
      array di semafori.
      - un intero N che rappresenta l'operazione da eseguire.
      Se l'intero � N > 0 il valore del semaforo viene incrementato di tale
      quantit�. Se � N = 0 la semop blocca il codice in attesa che il valore
      del semaforo diventi 0. Se � N < 0 la semop blocca il codice in attesa
      che il valore del semaforo meno N sia maggiore o uguale a 0.
    */

    sops[0].sem_num = 1; /* semaforo 1 */
    sops[0].sem_op = 1;  /*
                           incrementa di uno il valore del semaforo 1
                           Il processo B deve eseguire una semop con
                           operazione di tipo sem_op = 0.
                           In tal modo il processo B si mette in attesa
                           che il suo semaforo assuma valore 0.
                           Fintantoch� il semaforo 1 ha valore 1 il
                           processo B rimane bloccato sulla sua semop.

                         */
    sops[0].sem_flg = 0;
    semop(semid, sops, 1);

    sops[0].sem_num = 2; /* semaforo 2 */
    sops[0].sem_op = 1;  /*
                           incrementa di uno il valore del semaforo 2
                           Il processo C deve eseguire una semop con
                           operazione di tipo sem_op = 0.
                           In tal modo il processo C si mette in attesa
                           che il suo semaforo assuma valore 0.
                           Fintantoch� il semaforo 2 ha valore 1 il
                           processo C rimane bloccato sulla sua semop.

                         */
    sops[0].sem_flg = 0;
    semop(semid, sops, 1);

    /* cicla per 3 volte */
    for(i=0; i<3; i++) {

        /* Acquisizione della risorsa */

        printf("Processo A: sono in attesa di ricevere la risorsa\n");

        sops[0].sem_num = 0; /* semaforo 0 */
        sops[0].sem_op = 0;  /* attende che il semaforo valga zero */
        sops[0].sem_flg = 0;

        sops[1].sem_num = 0;    /* semaforo 0 */
        sops[1].sem_op = 1;     /* incrementa il valore del semaforo per evitare che
                                   al ciclo successivo il processo A possa utilizzare
                                   nuovamente la risorsa senza attendere che anche
                                   il processo B e il processo C abbiamo usufruito
                                   della risorsa. Sar� poi il processo C che
                                   abiliter� nuovamente il processo A portando il valore
                                   del semaforo 0 al valore 0.
                                */
        sops[1].sem_flg = 0;

        /*
          la chiamata � atomica !!!
          In tal modo entrambe le 2 operazioni precedenti vengono eseguite in modo
          "non interrompibile". Ci� significa che appena il processo A riesce
          ad acquisire la risorsa (perch� il valore del semaforo 0 diventa 0 e la
          semop sbocca il processo A) immediatamente il valore del semaforo 0
          viene incrementato per garantire la ciclicit� nell'utilizzo della
          risorsa come descritto precedentemente.
        */
        semop(semid, sops, 2);

        printf("Processo A: ho acquisito la risorsa\n");
        sleep(5); /* Supponiamo che il processo A detenga la risorsa per 5 secondi */
        printf("Processo A: sto per rilasciare la risorsa al processo B \n");

        sops[0].sem_num = 1;  /* semaforo 1 */
        sops[0].sem_op = -1;    /* rilascia la risorsa del processo B decrementando
                                   il valore del semaforo 1. In tal modo il processo B
                                   che � in attesa sulla semop che il valore del semforo
                                   diventi 0 si sblocca ed acquisisce la risorsa*/
        sops[0].sem_flg = 0;


        semop(semid, sops, 1);
    }

}
