/******************************************
MODULO: mycat.c
SCOPO: Emulazione del comando cat
******************************************/
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include "mylib.h" /*Contiene la definizione della funzione syserr()*/

#ifndef SIZE
#define SIZE 512 /*dimensione per il buffer di lettura/scrittura*/
#endif

int main (int argc, char **argv)
{
	int fd; /*file descriptor*/
	int i, n;
	char buf[SIZE]; /*buffer di lettura/scrittura*/

	if (argc < 2) {
		fprintf(stderr,"Specify at least one argument.\n");
		exit(1);
	}
	
	for(i=1; i < argc; i++){
	
		printf("********** file: %s ***********\n", argv[i]);

		/* 
			Apertura del file da leggere.
			Utilizzare la funzione open per leggere il file
			indicato in argv[i].
		*/
		
		fd = ... ...
		
		/*
			Controllare che l'apertura del file sia andata a buon
			fine usando la funzione syserr definita in mylib.h mylib.c
		*/
		
		... ...
		
		/*
			Leggere il contenuto del file a blocchi di SIZE bytes e
			memorizzarli all'interno del buffer buf.
			Quindi scrivere il contenuto del buffer nello stdout.
			Suggerimento: utilizzare un ciclo while che continua a
			leggere e scrivere i dati finche'la read indica
			che non e' piu' in grado di leggere nulla perche' si e' giunti
			alla fine del file.
			Ricordare che il file descriptor per lo stdout e' 1.
		*/
		
		... ...
		
		printf("*************************************\n");
	}
}

