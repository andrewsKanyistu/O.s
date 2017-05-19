/******************************************
MODULO: pipe_father_to_child.c
SCOPO: il padre scrive una frase in una pipe,
il figlio la legge e la stampa in output.
******************************************/
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main (int argc, char *argv[])
{
  int status;
	int p[2]; /* file descriptor per la pipe */
	
  char buf[64]; /* buffer per le operazioni di lettura e scrittura sulla pipe */


	/* 
		Creazione della pipe. Alla system call pipe deve essere passato un
		argomento di tipo vettore di 2 interi. Nell'elemento 0 del vettore 
		viene inserito il valore del file descriptor necessario per leggere
		i dati presenti nella pipe, nell'elemento 1 invece viene inserito il
		valore del file descriptor necessario per scrivere dei dati nella pipe.
		La funzione pipe ritorna il valore 0 se tutto va bene, -1 altrimenti.
		Vedere man pipe.
		La pipe permette di far comunicare solo processi che sono imparentati.
		La chiamata alla pipe deve essere effettuata prima di chiamare la
		fork.
	*/
  if(pipe(p) == -1) {
		perror("pipe");
		exit(1);
	}
	
	printf("Sono il padre e creo un figlio\n");
	
	if ((status=fork()) == -1) {
    perror("fork() fallita\n");
		exit(1);
	
	} else if (status == 0) { /* codice del figlio */ 
  	
		/*
			il figlio chiude la pipe in scrittura perchè utilizza 
			la pipe solo per leggere i dati che vengono inseriti dal padre
		*/
		if(close (p[1]) == -1){
			perror("close p[1] del figlio\n");
			exit(1);
		} 
	
		/*
			il figlio legge dalla pipe e memorizza i dati nel buffer buf.
		*/
    if (read(p[0],buf,sizeof(buf)) == -1) {
    	perror("read() del figlio fallita\n");
			exit(1);
		}			
		
	  /* 
			il figlio scrive su stdout quanto letto dalla pipe
		*/
		printf ("figlio -- ricevuto: %s\n", buf);
		exit (0);
		
	} else  {  /* codice del padre */
	
		/*
			il padre chiude la pipe in lettura perchè la utilizza solo
			per scrivere i dati che devono essere letti dal figlio.
		*/
		if(close (p[0]) == -1) {
			perror("close p[0] del padre\n");
			exit(1);
		}
		
		printf("Sono il padre, scrivo nella pipe.\n");
		
		/* il padre scrive nella pipe la stringa "In bocca alla pipe" */
		if(write (p[1], "In bocca alla pipe", 19) == -1) {
			perror("write() del padre fallita\n");
			exit(1);
		}
		
		/*
			il padre attende la terminazione del figlio
		*/
	  wait (NULL);
  	exit (0);
	}	
}
