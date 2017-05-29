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
#include <unistd.h>
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



	if((shmid=shmget(chiave,sizeof(matriceA),IPC_CREAT|0666))==-1){
		printf("Shared memory failed\n");
	}
	printf("%i\n",shmid );

	int (*attachPoint)[dim];
	attachPoint=  shmat(shmid,0,0);
	//int (*attachPoint)[dim]=(int *) shmat(shmid,0,0);



	//if(attachPoint== (char *)-1){
	//	printf("Errore nella shmat\n");
	//}

	//sprintf(attachPoint,"100 blargon 101010 posso tokenizzare;");
	//printf("Scrittura eseguita nella memoria condivisa su %p\n",attachPoint );
	//printf("Valore trovato nella attachPoinnt==>%s\n", attachPoint);
	//shmdt(attachPoint);

	pid_t children[5];
	//int wpid;
	//int status;
	//pid_t pid;
	attachPoint[1][1]=10;


printf("Sono il padre col mio pid %i\n",getpid() );
int pipeArr [2];
char buff[15];
pipe(pipeArr);
//sleep(30);
for ( i = 0; i < 5; i++) {
		children[i]=fork();
		if(children[i]==0){
			printf("sono il figlio ==> %i, mio padre ==> %i\n",getpid(),getppid() );
			//sleep(2);
			exit(0);
		}
		if (i==2){
			printf("sono il figlio %i e attachPoint vale %i\n",i,attachPoint[1][1] );
			// modifico attachpoints
			read(pipeArr[0],buff,15);
			printf("questo è il messaggio======%s\n", buff);
			attachPoint[1][1]=3;
			return 1;
		}
}

//============ CODICE DEL PADRE==========
write(pipeArr[1],"Hello World",15);
sleep(5);
printf("attachPoint vale %i",attachPoint[1][1] );



struct shmid_ds sharedmemory;
shmctl(shmid,IPC_RMID,&sharedmemory);



//while((wpid=wait(&status)) > 0){
//	printf("padre terminato\n" );
//}
/* molitplicazione
int matriceB[dim][dim];
int somma[dim][dim];
for ( i = 0; i < dim; i++) {
	for ( j = 0; j < dim; j++) {
		matriceB[i][j]=10;
		somma[i][j]=5;

	}

}
int multiply [dim][dim];
for (size_t i = 0; i < dim; i++) {
	for (size_t j = 0; j < dim; j++) {
		multiply[i][j]=0;
	}
}
int k,sum=0;
for(i=0; i<dim; ++i){
	for(j=0; j<dim; ++j)

    	for(k=0; k<dim; ++k){
        	multiply[i][j]+=matriceA[i][k]*matriceB[k][j];
					//printf("mul==>%i\n",multiply[i][j] );
    	}
}


	for ( i = 0; i < dim; i++) {
		for ( j = 0; j < dim; j++) {
			printf("%i ",multiply[i][j] );
		}

	}
	*/
return(0);
/*
	sprintf(attachPoint,"100 blargon 101010 ahahaha ;");
	printf("Scrittura eseguita nella memoria condivisa su %p\n",attachPoint );
	printf("Valore trovato nella attachPoinnt==>%s\n", attachPoint);
	shmdt(attachPoint);
*/

}
