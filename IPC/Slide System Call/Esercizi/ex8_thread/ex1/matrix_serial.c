/* matrix_serial.c
 *
 * Moltiplicazione tram matrici senza thread
 * (Matrix_A  X  Matrix_B) => Matrix_C
 */

#include <stdio.h>
#define ARRAY_SIZE 10

/* definizione del tipo matrix_t per le matrici */
typedef int matrix_t[ARRAY_SIZE][ARRAY_SIZE];

/* definizione di 3 matrici */
matrix_t MA,MB,MC;


/*
* Routine che moltiplica una riga di MA per una colonna di MB e inserisce
  il risultato nella matrice MC
*/
void mult(int size, int row, int column, matrix_t MA, matrix_t MB, matrix_t MC){
  int position;

  MC[row][column] = 0;
  for(position = 0; position < size; position++) {
    MC[row][column] = MC[row][column] +
      ( MA[row][position]  *  MB[position][column] ) ;
  }
}


/*
 * Main: allocca e inizializza le matrici e calcola il prodotto
 */
int main(void) {
  int	size, row, column;

  /* dimensione delle matrici */
  size = ARRAY_SIZE;
  
  /* Gli elementi della matrice MA sono inizializzati a 1 */
  for (row = 0; row < size; row++) {
    for (column = 0; column < size; column++) {
      MA[row][column] = 1;
    }
  }
	/* Gli elementi della matrice MB sono inizializzati alla
	   somma ottenuta dagli indici di riga e colonna + 1 */
  for (row = 0; row < size; row++) {
    for (column = 0; column < size; column++) {
      MB[row][column] = row + column + 1;
    }
  }
  printf("MATRIX: The A array is;\n");
  for(row = 0; row < size; row ++) {
    for (column = 0; column < size; column++) {
      printf("%5d ",MA[row][column]);
    }
    printf("\n");
  }
  printf("MATRIX: The B array is;\n");
  for(row = 0; row < size; row ++) {
    for (column = 0; column < size; column++) {
      printf("%5d ",MB[row][column]);
    }
    printf("\n");
  }

  
  /* Moltiplicazione */
  for(row = 0; row < size; row++) {
    for (column = 0; column < size; column++) {
      mult(size, row, column, MA, MB, MC);
    }
  }

  /* Stampa dei risultati */
  printf("MATRIX: The resulting matrix C is;\n");
  for(row = 0; row < size; row ++) {
    for (column = 0; column < size; column++) {
      printf("%5d ",MC[row][column]);
    }
    printf("\n");
  }
  return 0;
}
