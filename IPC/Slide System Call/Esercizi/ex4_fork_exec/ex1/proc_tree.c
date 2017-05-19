#include <stdio.h>
#include <sys/types.h>

/* Inserire eventuali altri include necessari
	 per utilizzare le system call desiderate.
*/

int main(int argc, char *argv[]) {
		
	/*
		Creare un nuovo processo con la funzione fork().
		La fork ritorna -1 in caso di errore e setta opportunamente
		la variabile errno. Al solito è possibile usare la funzione
		perror() o syserr() per investigare la natura dell'errore.
	*/

	
	/*
		Inserire il codice per il processo figlio numero 1. 
		La fork ritorna 0 al figlio e il valore del pid del figlio al padre.
		Addormentare il figlio per alcuni secondi usando la funzione sleep.
		Poi fare in modo che il figlio stampi il proprio pid e quello del padre
		tramite le funzioni getpid() e getppid()
	*/

		
	/* Inserire il codice del padre.
		 Il padre deve generare un secondo figlio
	*/	



	/* Inserire il codice del secondo figlio.
		 Deve fare le stesse cose del primo figlio, ma fate
		 in modo che attenda un numero di secondi diverso dal primo figlio.
	*/		


	/* Inserire ancora una sezione con il codice del padre.
		 Il padre deve stampare il proprio pid e quello dei due figli generati
	 */			

	/*
		 Operare analogamente per generare i nipoti.
	*/
			
	/* 
		Prima di terminare l'esecuzione i padri devono attendere i
		rispettivi figli tramite la funzione wait
	*/
}
