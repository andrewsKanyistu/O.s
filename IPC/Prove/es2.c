#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#ifndef SIZE
#define SIZE 512
#endif



int main (int argc, char *argv[]){
	int fd_out;
	char buffer[4096];			// buffer di 4096bytes
	char *token;
	const int dim=3;
	key_t chiave;
	int shmid;

	if(!(fd_out=open(argv[1],O_RDONLY))){
		printf("Errore on Open\n");
	}

	// leggo tutto il blocco
	if(!read(fd_out,buffer,4096))
		printf("file non letto correttamente\n");

	printf("%s\n",buffer );
	token=strtok(buffer,";\n");		// tokenizzo la stringa con spazio e a capo
	printf("token prima della tokenizzazione>>%s\n", token);
	
	int i =0, j=0;
	int matriceA [dim][dim];

	while(token != NULL){
		matriceA[i][j]=atoi(token);
		printf("i vale ==>%d, j vale ==>%i token vale ==>%s\n",i,j,token );
		i++;
		if (i==dim)
		{
			j++;
			i=0;
		}

		token=strtok(NULL,";\n");
	}
	
	if((chiave=ftok("es2.c",getpid()))==-1){
		printf("Error nella creazione della chiave\n");
	}



	if((shmid=shmget(chiave,2 * sizeof(matriceA),IPC_CREAT|0666|IPC_EXCL))==-1){
		printf("Failed memory failed\n");
	}


	int * attachPoint= shmat(shmid,0,0);
	int * attach2= shmat(shmid,0,0);
	if(attachPoint==(int *)-1){
		printf("Errore nella shmat\n");
	}
	*attachPoint=1000;
	attachPoint++;
}
	

