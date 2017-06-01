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
#define SIZE 4096   // dimensione in bytes da leggere da file
#endif

typedef struct {
	char operazione;    		//messaggio da mandare ai figli
	int riga;
	int colonna;
	int dimensione;
	int *ptr_matriceA;
	int *ptr_matriceB;
	int *ptr_matriceC;
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
const int dim=atoi(argv[4]); 					// dimensione delle matrici

//matrici dove verrano salvati i dati letti da disco
int matriceA[dim][dim];
int matriceB[dim][dim];
int matriceC[dim][dim];
// inizializzazione
for (i = 0; i < dim; i++) {
	for ( j = 0; j < dim; j++) {
		matriceA[i][j]=0;
		matriceB[i][j]=0;
		matriceC[i][j]=0;
	}
}


// generazione della chiave
key_t shm_key;
if((shm_key=ftok(argv[0],getpid()))==-1){
printf("Error nella creazione della chiave\n");
}


//Verifico se i parametri inseriti sono corretti
if(argc < 5){
	printf(" Numero di argomenti insufficienti o non corretti\n" );
	exit(1);
}

const int nProc=atoi(argv[5]);

// apro i filese
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
	printf("i vale ==>%d, j vale ==>%i token vale ==>%s\n",i,j,token );
	j++;
	if (i==dim){
		i++;
		j=0;
	}
	token=strtok(NULL,";\n");
}


//========== PARSE MATRICE B============
i=0;j=0;
token=strtok(bufferB,";\n");		// tokenizzo la stringa con spazio e a capo
 i =0, j=0;
while(token != NULL){
	matriceB[i][j]=atoi(token);
	printf("i vale ==>%d, j vale ==>%i token vale ==>%s\n",i,j,token );
	j++;
	if (i==dim){
		i++;
		j=0;
	}
	token=strtok(NULL,";\n");
}




// == sezione shared memory=====
int shmid_A=0,shmid_B=0,shmid_C=0;
if((shmid_A=shmget(shm_key,sizeof(matriceA),IPC_CREAT|0666))==-1)
	printf("Shared memory failed\n");

if((shmid_B=shmget(shm_key,sizeof(matriceA),IPC_CREAT|0666))==-1)
	printf("Shared memory failed\n");

if((shmid_C=shmget(shm_key,sizeof(matriceA),IPC_CREAT|0666))==-1)
	printf("Shared memory failed\n");

for (i = 0; i < dim; i++) {
	for ( j = 0; j < dim; j++) {
		printf("->%i",matriceB[i][j] );
	}
}

return (0);
}
