#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/wait.h>
#ifndef SIZE
#define SIZE 512
#endif

void work();

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



	if((shmid=shmget(chiave,3*sizeof(matriceA),IPC_CREAT|0666))==-1){
		printf("Shared memory failed\n");
	}
	printf("%i\n",shmid );


	char * attachPoint=(char *)  shmat(shmid,NULL,0);


	if(attachPoint==(char *)-1){
		printf("Errore nella shmat\n");
	}
	sprintf(attachPoint,"100 blargon 101010 posso tokenizzare;");
	printf("Scrittura eseguita nella memoria condivisa su %p\n",attachPoint );
	printf("Valore trovato nella attachPoinnt==>%s\n", attachPoint);
	shmdt(attachPoint);

	//pid_t children[5];
	//int wpid;
	int status;
	pid_t pid;
for ( i = 0; i < 5; i++) {
	pid = fork();
	if (pid==0) {
		printf("sono il figio %i mio padre==>%i\n",getpid(),getppid() );
		return 1;
	}else{
		wait(NULL);
		printf("========PROCESSO GENITORE=======\n");
	}
}





//while((wpid=wait(&status)) > 0){
//	printf("padre terminato\n" );
//}
}
