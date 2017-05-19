/*
SCOPO: Emulare il comando ls -l
*/

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>  
#include <sys/dir.h>
#include <dirent.h>
#include <time.h>
#include "mylib.h"

int main(int argc, char *argv[]) {
	
	DIR *d; /* puntatore alla directory */
	
	struct dirent *dentry; /* puntatore alla entry della directory*/
	
	char *name; /* nome della entry della directory */
	
	struct stat *file_stat; /* puntatore alla struttura contente le
														 tutte le informazioni riguardanti una
														 entry di una directory */

	if (argc != 2) {
		printf("usage: myls <dirname>");
		exit(1);
		
		
	} else {
	
		/*
			Aprire la directory argv[1]
		*/

		d = ... ...
			
		/* 
			Ciclare per tutte le entry della directory
			in modo da visualizzare i dati di ognuna.
			I dati da visualizzare sono quelli mostrati
			dal comando ls -l
		*/	
		for(... ... ; ... ... ; ... ... ;) {

			/* 
				Recuperare e stampare il nome della entry
			*/
			name = ... ...
			printf("%s\n", name);			
			
			/* 
				Recuperare la struttura "stat" per la entry definita da "name".
				La struttura stat contiene tutte le informazioni che vengono
				visualizzate dal comando ls -l.
				Vedere il manuale di stat per i dettagli: man 2 stat
			*/

			... ...
			
			/*
				Controllare il tipo della entry usando il valore 
				del campo st_mode della struct stat fornita dalla funzione stat.
				Il valore del campo st_mode deve essere messo in AND BITWISE
				(usando &) con la flag S_IFMT per mascherare il valore dei bit
				di protezione (da 0 a 8) e mantenere solo i valori dei bit che determinano
				il tipo della entry (da 9 a 15). Il risultato dell'AND deve essere
				confrontato con le flags S_IFDIR, S_IFBLK, S_IF_CHR, ...
				Leggere attentamente il manuale della stat: man 2 stat
				
				Il tipo della entry deve essere distinto come segue:
				- directory, devono essere indicati da una "d"
				- file regolari, devono essere indicati da un "-"
				- device a blocchi, devono essere indicati da una "b"
				- device a caratteri, devono essere indicati da una "c"
			*/
			
			
			if(... ...) { /* E'una directory? */
				printf("d ");
			} else if (... ...) { /* E'un device a blocchi? */
				printf("b ");
			} else if (... ...) { /* E'un device a caratteri? */
				printf("c ");
			} else if (... ...) { /* E'un file normale? */
				printf("- ");
			}
			
			/* 
				Stampare i valori dei bit di protezione.
				Usare il campo st_mode della struct stat mettendolo
				in AND BITWISE con 0777 per mascherare il valore dei 
				bit dal 9 al 15.
				Stampare il valore ottale!
			*/
			printf(...);
			
			/* 
				Stampare il numero di hard link
			*/
			printf(...);
			
			/* 
				Stampare lo user id
			*/
			printf(...);
			
			/*
				Stampare il group id
			*/
			printf(...);
			
			/*
				Stampare la dimensione in bytes
			*/
			printf(...);	
			
			/*
				Stampare il tempo di ultima modifica
			*/
			printf(...);		

		}	
	}	
} 
