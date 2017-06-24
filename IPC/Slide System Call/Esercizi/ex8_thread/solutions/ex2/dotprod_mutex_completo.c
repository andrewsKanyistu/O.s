/*
Prodotto scalare tra due vettori di dimensione 100.
Creare 4 thread. Ogni thread calcola un risultato
parziale corrispondente al prodotto scalare tra 25 elementi dei vettori.
Usare i mutex per proteggere la scrittura sulla variabile sum che
conterra' il risultato finale.
*/

#include <pthread.h>
#include <stdio.h>
#include <malloc.h>

#define NUMTHRDS 4
#define VECLEN 100

/*Struttura che contiene i dati da usare per effettuare 
  il prodotto scalare tra il vettore a e il vettor b
	il risultato verra' messo in sum.
	veclen contiene la lunghezza dei vettori 
*/

typedef struct 
 {
   double      *a;
   double      *b;
   double     sum; 
   int     veclen; 
 } DOTDATA;


   DOTDATA dotstr; 

	/* definire il vettore di ID per le thread */
	/* definire la variabile mutex */

/*
Routine di startup per le thread. Esegue un prodotto parziale su
un intervallo dei vettori a e b.
*/

void *dotprod(void *arg) {
/*
Calcolare il prodotto scalare di un sottoinsieme degli
elementi dei vettori contenuti nella struttura dotsrt. 
L'argomento passato alla funzione rappresenta l'offset per 
i vettori a e b della struttura dotprod da cui iniziare 
a calcolare il prodotto parziale.
Ad esempio, se l'offset e' 0, la thread dovra' calcolare
il prodotto scalare per gli elementi di a e b compresi tra
l'indice 0 e l'indice 24, se e' 1 per gli elementi compresi
tra 25 e 49 e cosi' via.
*/

/*
Il risultato del prodotto parziale va scrittua nella variabile sum 
della struttura dotstr.
La scrittura su tale variabile deve essere protetta in una sezione critica 
usando pthread_mutex_lock() e pthread_mutex_unlock().
*/

}

/* 
Il main deve creare 4 thread e fargli eseguire la funzione
dot_prod su sezioni diverse dei vettori da moltiplicare.
Al termine, deve essere stampato il risultato del prodotto.
*/

int main (int argc, char *argv[]) {


/* Allocare la memoria per due vettori di dimensione VECLEN e inizializzarli
   come e' stato fatto in dotprod_serial.c */
	 
/* Inizializzare i campi della struttura dotprod in modo che i puntatori a e b
   puntino ai vettori appena dichiarati
*/
  
/* Inizializzare il mutex ai valori di default tramite la funzione
	 pthread_mutex_init
*/ 

/* Creare una thread joinable ogni 25 elementi dei vettori da moltiplicare */

/* Ogni thread deve eseguire la funzione dotprod */

/* L'argomento della dotprodo deve servire come offset per specificare
	 l'intervallo di elementi dei vettori da moltiplicare
*/

/* Attendere la terminazione delle thread usando la funzione 
   pthread_join() */

/* Stampare il risultato finale e rimuovere le risorse allocate */

}   

