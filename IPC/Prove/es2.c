#include <unistd.h>
#include <stdio.h>
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
#define READ 0
#define WRITE 1
#ifndef SIZE
#define SIZE 4096   // dimensione in bytes da leggere da file
#endif

typedef struct {
	char operazione;    		//messaggio da mandare ai figli
	int riga;
	int colonna;
	int dimensione;
	//int *ptr_matriceA;
	//int *ptr_matriceB;
	//int *ptr_matriceC;
}msg;


int main(int argc, char *argv[]){
int fd_matriceA;
int fd_matriceB;
int fd_matriceC;
int i=0,j=0,k=0;											// variabili contatore
char *token;
char bufferA[SIZE];
char bufferB[SIZE];
char bufferC[SIZE];
int shmid_A=0,shmid_B=0,shmid_C=0;		// id per modificare la memoria condivisa
const int dim=atoi(argv[3]); 					// dimensione delle matrici

//matrici dove verranno salvati i dati letti da disco
int matriceA[dim][dim];
int matriceB[dim][dim];
int matriceC[dim][dim];

// generazione della chiave per la memoria condivisa A;B;C
key_t shm_key,shm_keyB,shm_keyC;
if((shm_key=ftok(argv[0],'a'))==-1){
	perror("Error nella creazione della chiave\n");
}
if((shm_keyB=ftok(argv[0],'b'))==-1){
	perror("Error nella creazione della chiave\n");
}
if((shm_keyC=ftok(argv[0],'c'))==-1){
	perror("Error nella creazione della chiave matrice c\n");
}

//Verifico se i parametri inseriti sono corretti
if(argc < 5){
	printf(" Numero di argomenti insufficienti o non corretti\n" );
	exit(1);
}

const int nProc=atoi(argv[4]);



// apro i files
if(!(fd_matriceA=open(argv[1],O_RDONLY))){
	printf("Errore on Open matrice A\n");
}

if(!(fd_matriceB=open(argv[2],O_RDONLY))){
	printf("Errore on Open matrice A\n");
}

if(!(fd_matriceC=open(argv[3],O_RDONLY))){
	printf("Errore on Open matrice A\n");
}

// leggo i file e li carico in buffer
if(!read(fd_matriceA,bufferA,SIZE))
	printf("file A  non letto correttamente\n");

if(!read(fd_matriceB,bufferB,SIZE))
	printf("file B non letto correttamente\n");


// NB la matrice C non deve essere VUOTA
if(!read(fd_matriceC,bufferC,SIZE)){
	printf("file C  non letto correttamente\n");
}

//==========PARSE MATRICE A===================
token=strtok(bufferA,";\n");		// tokenizzo la stringa con spazio e a capo
i =0, j=0;
while(token != NULL){
	matriceA[i][j]=atoi(token);
	j++;
	if (i==dim){
		i++;
		j=0;
	}
	token=strtok(NULL,";\n");
}

//========== PARSE MATRICE B============

token=strtok(bufferB,";\n");		// tokenizzo la stringa con spazio e a capo
i =0, j=0;
while(token != NULL){
	matriceB[i][j]=atoi(token);
	j++;
	if (i==dim){
		i++;
		j=0;
	}
	token=strtok(NULL,";\n");
}


// == sezione shared memory=====

if((shmid_A=shmget(shm_key,sizeof(matriceA),IPC_CREAT|0666))==-1)
	printf("Shared memory failed on A\n");

if((shmid_B=shmget(shm_keyB,sizeof(matriceB),IPC_CREAT|0666))==-1)
	printf("Shared memory failed on B\n");

if((shmid_C=shmget(shm_keyC,sizeof(matriceC),IPC_CREAT|0666))==-1)
	printf("Shared memory failed\n");

int (*attach_A)[dim];
int (*attach_B)[dim];
int (*attach_C)[dim];
attach_A= shmat(shmid_A,NULL,0);
attach_B=shmat(shmid_B,NULL,0);
attach_C=shmat(shmid_C,NULL,0);

// copio le matrici A e B in memoria condivisa
for ( i = 0; i < dim; i++) {
	for(j=0;j<dim;j++){

		attach_A[i][j]=matriceA[i][j];
		attach_B[i][j]=matriceB[i][j];
		attach_C[i][j]=0;
		//printf("%i\n", attach_A[i][j]);
	}
}



pid_t children;
int pipes[nProc-1][2];
printf("Numero processi==>%i\n",nProc);
// read==>0 write===>1

for (i=0;i<nProc;i++){
	pipe(pipes[i]);
	children=fork();
	if(children==0){
		// codice del figlio
		msg msg_parent;
		close(pipes[i][WRITE]);

			//printf("figlio %i\n",i );
			// chiudo la pipe in scrittura
			int status;
			while(1){
				if ((status =read(pipes[i][READ],&msg_parent,sizeof(msg))==-1)) {
					perror("read failed on pipe");
				};

				switch (msg_parent.operazione) {
					case 'k':{
						printf("Operazione ==>%c figlio %i\n", msg_parent.operazione,i);
						printf("figlio %i terminato\n",i );
						exit(0);
						break;
					}
					case 'm':{
						int cRiga=msg_parent.riga;
						int cCol=msg_parent.colonna;

						printf("Operazione ==>%c figlio %i\n", msg_parent.operazione,i);
						printf("riga==> %i, colonna ==> %i, \n",msg_parent.riga,msg_parent.colonna);
						int matRiga[dim];
						int num=0;

						for(j=0;j<dim;j++){
							attach_C[cRiga][cCol]+=attach_A[cRiga][j]*attach_B[j][cCol];
							//num=attach_C[cRiga][cCol];

						}

						break;
					}
				}

		}
	}

}

if(children >0){
	int n=0,riga=0,colonna=0;

msg msg_to_child;
//for(i=0;i<dim;i++)
	//printf("attach_A==> %i\n",attach_B[i][i] );


// chiudo i pipes alla in lettura
for ( i = 0; i < nProc-1; i++) {
	close(pipes[i][READ]);
}
// MOLTIPLICAZIONE

//printf("==================MOLTIPLICAZIONE================\n");
for (i=0;i<dim;i++){

	for ( j = 0,n=0; j < dim; j++) {
		msg_to_child.operazione='m';
		msg_to_child.riga=i;
		msg_to_child.colonna=j;
		//printf("riga%i colonna%i\n",i,j );
		write(pipes[n][WRITE],&msg_to_child,sizeof(msg_to_child));
		if(n==nProc-1){
			n=0;
		}else{
			n++;
		}


	}

}
//printf("==================MOLTIPLICAZIONE================\n");
sleep(5);
	for ( i = 0; i < dim; i++) {
		for(k=0;k<dim;k++){
			printf("matrice C ==>%i\n",attach_C[i][k] );

		}
	}


	for ( i = 0; i < nProc-1; i++) {
		wait(NULL); // aspetto la terminazione dei figli
	}

}




	//sleep(2);
	//wait(NULL);




printf("Arrivo fin qui??\n");

// elimino la memoria condivisa;
struct shmid_ds sharedmemory;
shmctl(shmid_A,IPC_RMID,&sharedmemory);
shmctl(shmid_B,IPC_RMID,&sharedmemory);
shmctl(shmid_C,IPC_RMID,&sharedmemory);



}
