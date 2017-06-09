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
#include <time.h>
#include <sys/msg.h>
#ifndef SIZE
#define SIZE 512
#endif
// struttura per la pipe
typedef struct {
	char operazione;    		//messaggio da mandare ai figli
	int riga;
	int colonna;
	int dimensione;
	int *ptr_matriceA;
	int *ptr_matriceB;
	int *ptr_matriceC;
}msg;


int main (int argc, char *argv[]){
	int fd_out;
	char buffer[4096];			// buffer di 4096bytes
	char *token;
	const int dim=3;
	key_t chiave;
	int shmid;


	// messaggio ai figli
	msg msg_a_figlio;


	if(!(fd_out=open(argv[1],O_RDONLY))){
		printf("Errore on Open\n");
	}

	// leggo tutto il blocco
	if(!read(fd_out,buffer,4096))
	printf("file non letto correttamente\n");


	token=strtok(buffer,";\n");		// tokenizzo la stringa con spazio e a capo
	//if (token == NULL){
		printf("%s\n",buffer );
	//}
	int i =0, j=0;
	int matriceA [dim][dim];

	while(token != NULL){
		matriceA[i][j]=atoi(token);
		printf("i vale ==>%d, j vale ==>%i token vale ==>%s\n",i,j,token );
		j++;
		if (j==dim)
		{
			i++;
			j=0;
		}

		token=strtok(NULL,";\n");
	}

	/*for (i = 0; i < dim; i++) {
		for ( j = 0; j < dim; j++) {
			printf("mat A=%i\n",matriceA[i][j] );

		}
	}*/

	if((chiave=ftok("es2.c",getpid()))==-1){
		printf("Error nella creazione della chiave\n");
	}



	if((shmid=shmget(chiave,sizeof(matriceA),IPC_CREAT|0666))==-1)
		printf("Shared memory failed\n");



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

	pid_t children;
	//int wpid;
	//int status;
	//pid_t pid;
	attachPoint[1][1]=10;


//printf("Sono il padre col mio pid %i\n",getpid() );
int numerodiPipe=5;
int pipeArr [numerodiPipe][2];

//char buff[15];

//sleep(30);



// ===========coda di messaggi============//
key_t msgkey= ftok("es2.c",'b');
int msgid;
if((msgid=msgget(msgkey,IPC_CREAT|0666))==-1){
	printf("Error la coda esiste gia\n" );
	exit(1);
}


struct message{
	long type;
	char op;
} message;
message.op='k';
message.type=1;

// invio del codice al figlio
if (msgsnd(msgid,&message,sizeof(message)-sizeof(long),0)==-1){
	printf("error in sending message\n");
	exit(1);
}

int temp;
char stringa[15];


// FORK MULTIPLE CHILDREN
for ( i = 0; i < 5; i++) {
		pipe(pipeArr[i]);
		children=fork();
		/*
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
		*/
		if(children==0){
			// ricevo il messaggio dal padre
			//struct message msg_figlio;20





			while (temp < 5){
			close(pipeArr[i][1]);
			read(pipeArr[i][0],&stringa,sizeof(stringa));
			//strlen(stringa)??0:printf("%s\n",stringa);
			//srand(time(NULL));
			///sleep((rand()%5)+1);
			printf("8==========>%s\n",stringa );
			/*

			if(msgrcv(msgid,&msg_figlio,sizeof(msg_figlio)-sizeof(long),0,0)==-1){
				printf("son recieve error\n");
			}
			if(msg_figlio.op=='k'){
				printf("Ho ricevuto il messaggio %c\n", msg_figlio.op);
			}
			for(j=0;j<10;j++){
				printf("sono il figlio numero %i col pid ====>%i ciclo nr %i\n",i,getpid(),j );
				srand(time(NULL));

			}*/

			printf("figlio comincia ad eseguire pid==>%i %i\n",i,getpid() );
			temp++;
			}
			exit(0);
		}
	}

//pipe lettura=0,scrittura=1
//padre a figli ==> pipe
//temp++;
if(children>0){
	for(i=0;i<25;i++){
		close(pipeArr[i][0]);
		//inizializzo la struttura da usare
		//msg_a_figlio.op
		write(pipeArr[i][1], "hello world",15);
	}
	printf("\n\n=============padre dopo l'esecuzione dei figli=============\n");
	//sleep(2);
	//for ( i = 0; i < 5; i++) {
		wait(NULL);
	//}
}




//============ CODICE DEL PADRE==========




//printf("attachPoint vale %i\n",attachPoint[1][1] );						// per la memoria condivisa







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
