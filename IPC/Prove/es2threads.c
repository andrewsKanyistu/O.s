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
#include <semaphore.h>
#include <time.h>
#include <pthread.h>
#include <sys/msg.h>
#define PADRE 0		// identificativo del padre nel vettore di semafori
#define FIGLIO 1	// identificativo del un qualunque figlio nel vettore di semafori
#define READ 0
#define WRITE 1
#ifndef SIZE
#define SIZE 4096   // dimensione in bytes da leggere da file
#endif
int semid;

typedef struct {
 char operazione;    		//messaggio da mandare ai figli
 int riga;
 int colonna;


 int (*matr)[dim];
 int (*matrB)[100];
 int (*matrC)[100];
}msg;

void * threadFiglio (void * args){
	 msg* daPadre=(msg * )args;
	 int riga= daPadre->riga,colonna=daPadre->colonna;
	 printf("thread uno, riga==%i, colonna==>%i",riga,colonna);
	// printf("atacch vale %i\n",attach_A[i][j] );
	 return NULL;
}


void * threadFiglio (void *args);


int main(int argc, char *argv[]){
int fd_matriceA;
int fd_matriceB;
int fd_matriceC;
int i=0,j=0;											// variabili contatore
char *token;


struct shmid_ds sharedmemory;

char bufferA[SIZE];
char bufferB[SIZE];
char bufferC[SIZE];
int shmid_A=0,shmid_B=0,shmid_C=0,shmid_sum;		// id per modificare la memoria condivisa
static const int dim=atoi(argv[4]); 					// dimensione delle matrici

//matrici dove verranno salvati i dati letti da disco
int matriceA[dim][dim];
int matriceB[dim][dim];
int matriceC[dim][dim];

// generazione della chiave per la memoria condivisa A;B;C e per la somma
key_t shm_key,shm_keyB,shm_keyC,shm_sum;
if((shm_key=ftok(argv[0],'a'))==-1){
	perror("Error nella creazione della chiave\n");
}
if((shm_keyB=ftok(argv[0],'b'))==-1){
	perror("Error nella creazione della chiave\n");
}
if((shm_keyC=ftok(argv[0],'c'))==-1){
	perror("Error nella creazione della chiave matrice c\n");
}
if((shm_sum=ftok(argv[0],'k'))==-1){
	perror("Error nella creazione della chiave somma c\n");
}





//Verifico se i parametri inseriti sono corretti
if(argc < 5){
	printf(" Numero di argomenti insufficienti o non corretti\n" );
	exit(1);
}





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
	printf("Shared memory failed on C\n");

if((shmid_sum=shmget(shm_sum,sizeof(int),IPC_CREAT|0666))==-1)
	printf("Shared memory failed on sum\n");

int (*attach_A)[dim];
int (*attach_B)[dim];
int (*attach_C)[dim];
int *shm_sumValue=0;
attach_A= shmat(shmid_A,NULL,0);
attach_B=shmat(shmid_B,NULL,0);
attach_C=shmat(shmid_C,NULL,0);
shm_sumValue= (int *)shmat(shmid_sum,NULL,0);




// copio le matrici A e B in memoria condivisa
for ( i = 0; i < dim; i++) {
	for(j=0;j<dim;j++){

		attach_A[i][j]=matriceA[i][j];
		attach_B[i][j]=matriceB[i][j];
		attach_C[i][j]=0;

	}
}


srand(time(NULL));


msg toParent;
toParent.operazione='s';
toParent.riga=1;
toParent.colonna=300;
toParent.matr=attach_A;
// CODICE DEL PADRE

// implementazione dei thread
pthread_t singola;

if (pthread_create(&singola,NULL,&threadFiglio,&toParent) != 0){

	perror("oops" );

}
else{
	printf("Thread created successfully" );
}

shmctl(shmid_sum,IPC_RMID,&sharedmemory);






// elimino la memoria condivisa;

shmctl(shmid_A,IPC_RMID,&sharedmemory);
shmctl(shmid_B,IPC_RMID,&sharedmemory);
shmctl(shmid_C,IPC_RMID,&sharedmemory);
//dealloco il semaforo


}
