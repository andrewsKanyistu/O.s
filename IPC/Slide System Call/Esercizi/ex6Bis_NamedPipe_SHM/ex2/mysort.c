#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {
	
	int f_from_P_to_F_read; /* file descriptor per la lettura dalla fifo f_from_P_to_F */
	int f_from_P_to_F_write; /* file descriptor per la scrittura nella fifo f_from_P_to_F */
	int f_from_F_to_P_read; /* file descriptor per la lettura dalla fifo f_from_F_to_P */
	int f_from_F_to_P_write; /* file descriptor per la scrittura nella fifo f_from_F_to_P */
	
	int fd;
	int nread;
	char buf[512];

	/*
		creare le 2 fifo con la funzione mknod
		Vedere man 2 mknod
	*/

	... ...

	switch(fork()) {
		case -1:							/*errore*/
			perror("fork");
			exit(1);

		case 0:								/* codice del figlio */
		
		/* 
			Aprire la fifo da cui il figlio deve leggere e la fifo
			in cui il figlio deve scrivere. Utilizzare la system
			call open come per i file.
			Vedere man 2 open
		*/

		... ...

		/*
			Utilizzare il trucchetto della dup per duplicare i file descriptor
			delle fifo nel file descriptor 0 e 1. Vedere ex1 fatto con le pipe.
			Il trucco è lo stesso ma va fatto usando i descrittori delle fifo.
		*/

		... ...

		/*
			Usare la execlp per invocare il comando sort.
			sort si aspetta di leggere i dati da ordinare dal descrittore 0
			e di dover scrivere i dati ordinati nel descrittore 1. 
		*/

		... ...
	}

		/* codice del padre */

		/* 
			Aprire la fifo in cui il padre deve scrivere e la fifo
			da cui il padre deve leggere i dati ordinati inviatigli
			dal figlio. Utilizzare la system call open come per i file.
			Vedere man 2 open
		*/

		... ...

		/*
			Aprire il file contenente i dati con la open.
			Il file deve essere il primo elemento della linea
			di comando. Quindi il suo nome sarà contenuto in
			argv[1]
		*/

		... ...

		/*
			Leggere i dati dal file e memorizzarli in buf
		*/
			
		... ...


		/*
			Scrivere i dati da buf verso la pipe
		*/

		... ...

		
		/*
			Leggere i dati ordinati dalla pipe e memorizzarli in buf
		*/
		
		... ...
		
		/* scrivere i dati ordinati in outup*/
		
		... ...


		/*
			Chiudere i file descriptor delle pipe
		*/

		... ...
		
		/* attendere la terminazione del figlio */
		
		... ...		
		
		/*
			rimuovere le fifo dal file system usando la system call
			unlink.
			Vedere man 2 unlink
		*/
	
}
