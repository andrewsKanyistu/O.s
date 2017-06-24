/*
	 Realizzare il prodotto riga per colonna di due
   matrici quadrate usando una thread separata per calcolare
   ogni elemento della matrice risultante
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
  int       id;		/* id della thread */
  int       size;	/* dimensione della matrice */
  int       Arow;	/* indice della riga */
  int       Bcol; /* indice della colonna */
} package_t;

matrix_t MA,MB,MC;	/* definizione delle matrici MA, MB e MC
											 l'operazione è MC = MA * MB
										*/

/*
	Funzione che moltiplica una riga per una colonna e
	posiziona il risultato nella matrice risultante
*/
void mult(int size, int row, int column) {

	/*
		eseguire l'operazione riga * colonna
		e memorizzare il risultato in MC
	*/
}


/*
	funzione di startup per le threads
*/
void *mult_worker(void *arg) {

	/* l'argomento della funzione deve essere castato al tipo desiderato */

	... ...

	/* chiamata alla funzione che esegue il prodotto riga * colonna */

	... ...

	/* liberare lo spazio allocato per l'argomento della funzione */
	... ...

  return(NULL);

}

/*
	Il main alloca le matrici, assegna i loro valori
	e crea le thread per eseguire la moltiplicazione
	riga * colonna
*/
int main(int argc, char **argv) {

  int       size, row, column, num_threads, i;

  pthread_t *threads;       /* per tenere traccia degli identificatori delle thread */

  package_t *p;             /* argomento per le thread */


  size = ARRAY_SIZE;				/* dimensione della matrice */

  /*
		viene creato un thread per ogni elemento della matrice,
		allocare lo spazio per i puntatori degli identificatori delle thread
	*/
	... ...

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
		risultato C.
		Allocare lo spazio per l'argomento della funzione di startup
		e creare la thread per ogni elemento della matrice
		usando la funzione pthread_create.
		Vedere man pthread_create
	*/

	... ...


    }
  }


  /*
		Sincronizzazione delle thread
		Il main deve attendere che tutte abbiano terminato la loro elaborazione
		usando la funzione pthread_join.
		vedere man pthread_join
	 */
	... ...


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
