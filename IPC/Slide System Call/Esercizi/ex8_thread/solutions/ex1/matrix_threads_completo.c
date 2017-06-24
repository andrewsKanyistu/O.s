/********************************************************
 * An example source module to accompany...
 *
 * "Using POSIX Threads: Programming with Pthreads"
 *     by Brad nichols, Dick Buttlar, Jackie Farrell
 *     O'Reilly & Associates, Inc.
 *
 ********************************************************
 * matrix_threads.c --
 *
 * A master thread spawns separate child threads to compute each
 * element in the resulting array. Each of the child threads is passed
 * a pointer to a structure that contains the element indices and
 * pointers to starting and resulting arrays.
 *
 * The master thread joins to each thread, prints out the result and
 * exits.
 */

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include <pthread.h>

#define ARRAY_SIZE 10

/* definizione del tipo matrice */
typedef int matrix_t[ARRAY_SIZE][ARRAY_SIZE];

/*
  definizione dell'argomento che viene passato
  alla funzione di startup delle thread
*/
typedef struct {
    int       id;       /* id della thread */
    int       size; /* dimensione della matrice */
    int       Arow; /* indice della riga */
    int       Bcol; /* indice della colonna */
} package_t;

matrix_t MA,MB,MC;  /* definizione delle matrici A, B e C
                       l'operazione è C = A * B
                    */

/*
  Funzione che moltiplica una riga per una colonna e
  posiziona il risultato nella matrice risultante
*/
void mult(int size, int row, int column) {

    int position;

    MC[row][column] = 0;

    for(position = 0; position < size; position++) {
        MC[row][column] = MC[row][column] + ( MA[row][position]  * MB[position][column] ) ;
    }
}


/*
  funzione di startup per le threads
*/
void *mult_worker(void *arg) {

    /* l'argomento della funzione deve essere castato al tipo desiderato */
    package_t *p = (package_t *)arg;

    printf("MATRIX THREAD %d: processing A row %d, B col %d\n", p->id, p->Arow, p->Bcol );

    /* chiamata alla funzione che esegue il prodotto riga * colonna */
    mult(p->size, p->Arow, p->Bcol);

    printf("MATRIX THREAD %d: complete\n", p->id);

    /* liberato lo spazio allocato per p */
    free(p);

    return(NULL);

}

/*
  Il main alloca le matrici, assegna i loro valori
  e crea le thread per eseguire la moltiplicazione
  riga * colonna
*/
int main(int argc, char *argv[]) {

    int       size, row, column, num_threads, i;

    pthread_t *threads;       /* per tenere traccia degli identificatori delle thread */

    package_t *p;             /* argomento per le thread */


    size = ARRAY_SIZE;              /* dimensione della matrice */

    /*
      viene creata una thread per ogni elemento della matrice,
    */
    threads = (pthread_t *) malloc(size * size * sizeof(pthread_t));

    /*
      Inizializzazione dei valori delle matrici A e B.
      Questi valori potrebbero essere letti da file o
      ottenuti da altri processi.
      Dal momento che lo scopo principale dell'esempio
      è quello di mostrare il funzionamento delle thread
      non ci preoccupiamo di come vengono ottenuti
      i dati per le matrici A e B.
    */

    /* la matrice A contiene tutti 1 */
    for (row = 0; row < size; row++) {
        for (column = 0; column < size; column++) {
            MA[row][column] = 1;
        }
    }

    /*
      ogni elemento della matrice B è uguale a
      1 + la somma dei sui indici di riga e colonna
    */
    for (row = 0; row < size; row++) {
        for (column = 0; column < size; column++) {
            MB[row][column] = row + column + 1;
        }
    }

    /* stampa dei valori degli elementi della matrice A */
    printf("MATRIX MAIN THREAD: The A array is ;\n");
    for(row = 0; row < size; row ++) {
        for (column = 0; column < size; column++) {
            printf("%5d ",MA[row][column]);
        }
        printf("\n");
    }

    /* stampa dei valori degli elementi della matrice B */
    printf("MATRIX MAIN THREAD: The B array is ;\n");
    for(row = 0; row < size; row ++) {
        for (column = 0; column < size; column++) {
            printf("%5d ",MB[row][column]);
        }
        printf("\n");
    }


    /*
      Viene creato un thread per ogni elemento della matrice
      risultato C
    */
    num_threads = 0;
    for(row = 0; row < size; row++) {
        for (column = 0; column < size; column++) {

            p = (package_t *)malloc(sizeof(package_t));
            p->id = num_threads;
            p->size = size;
            p->Arow = row;
            p->Bcol = column;

            /*
              creazione della thread.
              il primo argomento è il puntatore all'id del thread.
              il secondo rappresenta gli attributi. NULL = attributi di default (joinable)
              il terzo è il puntatore alla funzione di startup
              il quarto è l'argomento per la funzione di startup.
              Vedere man pthread_create
            */
            pthread_create(&threads[num_threads], NULL, mult_worker, p);

            printf("MATRIX MAIN THREAD: thread %d created\n", num_threads);

            num_threads++;

        }
    }


    /*
      Sincronizzazione delle thread
      Il main attende che tutte abbiano terminato la loro elaborazione
      usando la funzione pthread_join.
      vedere man pthread_join
    */
    for (i = 0; i < (size*size); i++) {
        pthread_join(threads[i], NULL);
        printf("MATRIX MAIN THREAD: child %d has joined\n", i);
    }


    /* Stampa dei risultati */
    printf("MATRIX MAIN THREAD: The resulting matrix C is;\n");
    for(row = 0; row < size; row ++) {
        for (column = 0; column < size; column++) {
            printf("%5d ",MC[row][column]);
        }
        printf("\n");
    }

    return 0;
}

